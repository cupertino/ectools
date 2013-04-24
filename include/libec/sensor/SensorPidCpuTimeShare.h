/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

// CPU share = CPU Elapsed time of PID / CPU total elapsed time
//                   (usr + sys)              (usr + sys)
#ifndef SENSORPID_CPUTIMESHARE_H__
#define SENSORPID_CPUTIMESHARE_H__

#include "SensorPid.h"
#include "SensorPidCpuTime.h"

namespace cea
{
  /// \brief CPU time share PID sensor
  ///
  /// The CPU time share sensor uses the information from CPUTime sensor to
  /// compute the time share of each process
  class CpuTimeShare : public PIDSensor
  {
  public:
    /// Constructor
    CpuTimeShare();
    CpuTimeShare(CpuTime* cpuTime);

    /// Destructor
    ~CpuTimeShare();

    void
    update();

    /// Update CPU time share for a given PID
    void
    update(pid_t pid = -1);

    sensor_t
    getValue(pid_t pid = -1);

    void
    add(pid_t pid = -1);

  private:
    CpuTime* _cpuTime;
    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
    std::map<pid_t, sensor_t> _pvPIDMap; ///< Previous value of the sensor. The PID is used as the key.
  };

}
#endif
