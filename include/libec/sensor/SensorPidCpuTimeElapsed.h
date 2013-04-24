/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

// REVIEWED (Dec 4 2012)
#ifndef SENSORPID_CPUTIMEELAPSED_H__
#define SENSORPID_CPUTIMEELAPSED_H__

#include "SensorPid.h"
#include "SensorPidCpuTime.h"

namespace cea
{
  /// \brief CPU elapsed time PID sensor
  ///
  /// Gets the cpu time (user + system) between two calls.
  class CpuElapsedTime : public PIDSensor
  {
  public:
    /// Constructor
    CpuElapsedTime();

    /// Destructor
    ~CpuElapsedTime();

    sensor_t
    getValue();

    /// Total CPU time between two consecutive calls
    u64
    getTotalElapsedTime();

    void
    update();

    /// Update CPU times (ANSI C implementation)
    void
    update2(pid_t pid = -1);

    sensor_t
    getValuePid(pid_t pid);

    /// Update CPU times (C++ implementation)
    void
    updatePid(pid_t pid);

    void
    add(pid_t pid);

  private:
    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
    std::map<pid_t, sensor_t> _pvPIDMap; ///< Previous value of the sensor. The PID is used as the key.
    CpuTime _ct;

    /// Previous value
    sensor_t _pValue;

    u64 _nctIdle;
    u64 _nptIdle;
  };

}
#endif
