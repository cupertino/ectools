/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

#ifndef SENSORPID_MEMRSS_H__
#define SENSORPID_MEMRSS_H__

#include "SensorPid.h"

namespace cea
{
  /// \brief Memory Resident Set Size Sensor
  ///
  /// The Memory RSS sensor uses the information from the /proc/[pid]/statm to
  /// retrieve the process' RSS in pages. The same information, but in Kb, can
  /// be colected by running the "free -k" command on a machine level.
  class MemRss : public PIDSensor
  {
  public:
    static const char* ClassName;

    /// Constructor
    MemRss();

    /// Destructor
    ~MemRss();

    /// Converts pages to Kb
    u64
    toKb(u64 pages);

    /// Converts Kb to pages
    u64
    toPages(u64 kb);

    /// Update the RSS for the entire machine by executing the "free -k" system call
    void
    update();

    /// Update RSS memory for the a given process.
    /// Retrieves this info from the /proc/[pid]/statm file
    void
    updatePid(pid_t pid);

    /// \brief Get's the name of the class
    const char*
    getClassName();

    /// Gets the last updated value for the process level RSS in pages
    sensor_t
    getValuePid(pid_t pid);

    /// Gets the last updated value for the machine level RSS in Kb
    sensor_t
    getValue();

  protected:
    /// Maps the pid into its RSS
    std::map<pid_t, u64> _memPid;

    unsigned pageToKbShift;

    /// The run-time acquired page size
    void
    getPageSize();
  };

}
#endif
