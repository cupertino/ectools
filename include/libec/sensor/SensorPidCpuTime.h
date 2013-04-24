/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

// REVIEWED (Dec 4 2012)
#ifndef SENSORPID_CPUTIME_H__
#define SENSORPID_CPUTIME_H__

#include "SensorPid.h"

namespace cea
{
  /// \brief CPU time PID sensor
  ///
  /// The CPU time sensor uses the information from the /proc/[pid]/stat to
  /// retrieve the process' CPU time:
  /// \f$t^{PID}_{CPU} = t^{PID}_{sys} + t^{PID}_{usr} \f$. The same information can
  /// be colected from /proc/stat on a machine level. The total time
  /// spent during an interval can be retrieved throught the getTotalTime method, this way,
  /// the \f$t^{node}_{CPU} = t^{node}_{sys} + t^{node}_{usr} + t^{node}_{idle} \f$.
  ///
  /// Information regarding the total and CPU time for each CPU is still missing.
  class CpuTime : public PIDSensor
  {
  public:
    static const char* ClassName;

    /// Constructor
    /// \param cpuId  CPU's identifier. -1 for all CPUs.
    CpuTime(char cpuId = -1);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    CpuTime(const std::string &xmlTag);

    /// Destructor
    ~CpuTime();

    /// Checks if the cpuId is good or not
    char
    checkCpus();

    void
    update();

    /// Update CPU times (C++ implementation)
    void
    updatePid(pid_t pid);

    /// \brief Get's the name of the class
    const char*
    getClassName();

    sensor_t
    getValuePid(pid_t pid);

    sensor_t
    getValue();

    /// Total CPU time between two consecutive calls
    u64
    getTotalTime();

    u64
    getIdleTime();

  private:
    /// Machine's current idle time
    u64 _cIdleTime;

    /// Current process' value
    u64 _cpValue;

    /// number of cpus
    char _cpuId;

    ///current cpu value
//    u64 *_ccValue;
  };

}
#endif
