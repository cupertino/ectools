/*
 * SensorCpuUsage.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

#ifndef SENSOR_CPU_TIME_USAGE_H_
#define SENSOR_CPU_TIME_USAGE_H_

#include "SensorPid.h"
#include "SensorPidCpuTimeElapsed.h"
#include <sys/time.h>

namespace cea
{

  /// \brief CPU Time Usage Sensor
  /// The CpuTimeUsage is a sensor based on the CPU elapsed time
  /// between two updates calls. The CPU usage is measured as follows
  /// \f[ CPU_usage = \frac{CPU_elapsed_time}{CPU_time} \f]
  class CpuTimeUsage : public PIDSensor
  {
  public:
    CpuTimeUsage();
    virtual
    ~CpuTimeUsage();

    void
    updatePid(pid_t pid);

    void
    update();

    sensor_t
    getValue();

    sensor_t
    getValuePid(pid_t pid);

    void
    add(pid_t pid);

  protected:
    void
    clean();

    void
    copy();

    CpuElapsedTime _cet;
    std::map<pid_t, struct timeval> _ctimePIDMap, _ptimePIDMap;
    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
    std::map<pid_t, sensor_t> _pvPIDMap; ///< Previous value of the sensor. The PID is used as the key.

    /// Previous value
    struct timeval _pTv;
    sensor_t _pValue;
  };

} /* namespace cea */
#endif /* SENSORCPUUSAGE_H_ */
