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
#include <perf.h>
#include <vector>
#include <map>

#include "Sensor.h"
#include "SensorPid.h"

namespace cea
{

  /**
   * @brief Performance Counter sensors
   * @author Leandro Fontoura Cupertino
   * @date Jun 1 2012
   *
   * Performance counters are hardware event counters that uses special file
   * descriptors to count. There's one file descriptor per virtual counter
   * used. Counters must not close the file descriptors, otherwise they will
   * stop counting and a random value will be initialized the next time the
   * descriptor is opened. This creates a problem when monitoring the counters
   * per PID, as the number of process grows the number of file descriptors
   * can surpass 1024 and then a segmentation fault will happen. Need to fix
   * this.
   */
  class PerfCount : public PIDSensor
  {

  public:
    enum TypeId
    {
      PERF_TYPE_HARDWARE = 0,
      PERF_TYPE_SOFTWARE = 1,
      PERF_TYPE_TRACEPOINT = 2,
      PERF_TYPE_HW_CACHE = 3,
      PERF_TYPE_RAW = 4,
      PERF_TYPE_BREAKPOINT = 5,
      PERF_TYPE_MAX
    };

    enum HwEventId
    {
      HW_CPU_CYCLES = 0,
      HW_INSTRUCTIONS = 1,
      HW_CACHE_REFERENCES = 2,
      HW_CACHE_MISSES = 3,
      HW_BRANCH_INSTRUCTIONS = 4,
      HW_BRANCH_MISSES = 5,
      HW_BUS_CYCLES = 6,
      HW_STALLED_CYCLES_FRONTEND = 7,
      HW_STALLED_CYCLES_BACKEND = 8,
      HW_REF_CPU_CYCLES = 9,
      HW_MAX
    };

    enum SwEventId
    {
      SW_CPU_CLOCK = 0,
      SW_TASK_CLOCK = 1,
      SW_PAGE_FAULTS = 2,
      SW_CONTEXT_SWITCHES = 3,
      SW_CPU_MIGRATIONS = 4,
      SW_PAGE_FAULTS_MIN = 5,
      SW_PAGE_FAULTS_MAJ = 6,
      SW_ALIGNMENT_FAULTS = 7,
      SW_EMULATION_FAULTS = 8,
      SW_MAX
    };

    enum CacheEventId
    {
      HW_CACHE_L1D = 0,
      HW_CACHE_L1I = 1,
      HW_CACHE_LL = 2,
      HW_CACHE_DTLB = 3,
      HW_CACHE_ITLB = 4,
      HW_CACHE_BPU = 5,
      HW_CACHE_NODE = 6,
      HW_CACHE_MAX
    };

    enum CacheOpId
    {
      READ = 0, WRITE = 1, PREFETCH = 2, OP_MAX
    };

    enum CacheOpResultId
    {
      ACCESS = 0, MISS = 1, OP_RESULT_MAX
    };

    /**
     * Constructor
     *
     * @param type Counter's type (PERF_TYPE_HARDWARE, PERF_TYPE_SOFTWARE or
     *  PERF_TYPE_HW_CACHE implemented)
     * @param config Counter's configuration. This parameter togheter with the
     *  type will define the performance counter to be created.
     * @param die_id Die (socket) identifier (-1 for all).
     * @param core_id Core identifier (-1 for all).
     * @param smt_id Simultaneous multithreading identifier (-1 for all).
     * */
    PerfCount(u32 type = (u32) 0, u64 config = (u64) 0, int die_id = -1,
        int core_id = -1, int smt_id = -1);

    /**
     * Updates performance counter's 'current value' for all processes.
     */
    void
    update();

    /**
     * Updates performance counter's 'current value'.
     *
     * @param pid Process identifier (-1 for all).
     */
    void
    update(pid_t pid = -1);

    /**
     * Gets performance counter's updated value.
     *
     * @param pid Process identifier (-1 for all).
     */
    sensor_t
    getValue(pid_t pid = -1);

  private:
    perf_event_attr _attr; // defines how the PC event will behave
    int _fd; // performance counter file descriptor
    int _core_id; // identifiers
    int _cpus_total; // number of CPUs present on the current machine

    // Current and previous values for a given PID
    std::map<pid_t, std::vector<int>*> _fdPIDMap;
    std::map<pid_t, std::vector<u64>*> _cvPIDMap, _pvPIDMap;

    /**
     * Opens all performance counter file descriptor needed.
     *
     * @param pid Process ID (-1 for all process)
     * @return A vector with all file descriptors
     */
    std::vector<int>*
    openfd(pid_t pid = -1);

    /**
     * Closes the file descriptors related to a pid.
     *
     * @param fd File descriptor vector to be closed
     */
    void
    closefd(std::vector<int>* fd);

    void
    add(pid_t pid = -1);

    /**
     * Removes the pid from this performance counter.
     * Closes all file descriptors and delete the pid entrance on all maps
     *
     * @param pid Process ID
     */
    void
    remove(pid_t pid = -1);
  };

}

#endif /* M_SENSOR_PC_H_ */
