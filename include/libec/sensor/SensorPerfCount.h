//============================================================================
// Name        : SensorPerfCount.h
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.01
// Copyright   : Your copyright notice
// Description : Performance Counter Sensor header
//============================================================================

#ifndef M_SENSOR_PC_H_
#define M_SENSOR_PC_H_

#include <sstream>
#include <iostream>
#include <unistd.h>
//#include <perf.h>
#include <linux/perf_event.h>
#include <vector>
#include <map>

#include "Sensor.h"
#include "SensorPid.h"

namespace cea
{

  /// @brief Performance Counter sensors
  /// @author Leandro Fontoura Cupertino
  /// @date Jun 1 2012
  ///
  /// Performance counters are hardware event counters that uses special file
  /// descriptors to count. There's one file descriptor per virtual counter
  /// used. Counters must not close the file descriptors, otherwise they will
  /// stop counting and a random value will be initialized the next time the
  /// descriptor is opened. This creates a problem when monitoring the counters
  /// per PID, as the number of process grows the number of file descriptors
  /// can surpass 1024 and then a segmentation fault will happen. Need to fix
  /// this.

  class PerfCount : public PIDSensor
  {

  public:
    /// Constructor
    /// \param type Counter's type (PERF_TYPE_HARDWARE, PERF_TYPE_SOFTWARE or
    ///  PERF_TYPE_HW_CACHE implemented)
    /// \param config Counter's configuration. This parameter associated to the
    ///  type will define the performance counter to be created.
    /// \param core_id Core identifier (-1 for all).
    PerfCount(u32 type = 0ul, u64 config = 0ull, int core_id = -1);

    ~PerfCount();

    /// Updates performance counter's 'current value' for all processes.
    void
    update();

    sensor_t
    getValue();

    /// Updates performance counter's 'current value'.
    /// \param pid Process identifier (-1 for all).
    void
    updatePid(pid_t pid);

    /// Gets performance counter's updated value.
    /// \param pid Process identifier (-1 for all).
    sensor_t
    getValuePid(pid_t pid);

  private:
    struct hpc_ent // hardware performance counter entity
    {
      int fd; // file descriptor
      u64 cVal, pVal; // current and previour values
    };

    // read the performance counter from a file descriptor
    u64 readPC(int fd);

    //perf_event_attr _attr; // defines how the PC event will behave
    struct perf_event_attr _attr; // defines how the PC event will behave
    int _cpu_id; // identifiers
    int _cpus_total; // number of CPUs present on the current machine

    // Vector of node related performance counter entities
    hpc_ent* _nArr;

    // Map of process related performance counter entities
    std::map<pid_t, hpc_ent> _pidMap;

    /// Previous value
    sensor_t _pValue;

    /// Opens all performance counter file descriptor needed.
    /// \param pid Process ID (-1 for all process)
    /// \return A vector with all file descriptors
    int
    openfd(pid_t pid);

    void
    add(pid_t pid);

    /// Removes the pid from this performance counter.
    /// Closes all file descriptors and delete the pid entrance on all maps
    /// \param pid Process ID
    void
    remove(pid_t pid);
  };

}

#endif
