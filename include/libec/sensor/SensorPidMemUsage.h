/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

#ifndef SENSORPID_MEMUSAGE_H__
#define SENSORPID_MEMUSAGE_H__

#include "SensorPid.h"
#include "SensorPidMemRss.h"

namespace cea
{
  /// \brief Memory Usage
   class MemUsage : public PIDSensor
  {
  public:
    static const char* ClassName;

    /// Constructor
    MemUsage();

    /// Destructor
    ~MemUsage();

    void
    update();

    void
    updatePid(pid_t pid);

    /// \brief Get's the name of the class
    const char*
    getClassName();

    /// Gets the last updated value for the process level memory usage
    sensor_t
    getValuePid(pid_t pid);

    /// Gets the last updated value for the machine level memory usage
    sensor_t
    getValue();

  protected:
    MemRss _rss;
    cea::u64 _memTotal;

    /// The run-time available memory in Kb
    u64
    getAvailableMemory();
  };

}
#endif
