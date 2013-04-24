/*
 * SensorCpuUsage.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Leandro
 */

#ifndef SENSORCPUUSAGE_H_
#define SENSORCPUUSAGE_H_

#include "SensorPid.h"
#include <sys/time.h>

namespace cea
{

  class SensorCpuUsage : public PIDSensor
  {
  public:
    SensorCpuUsage();
    virtual
    ~SensorCpuUsage();

    void
    updatePid(pid_t pid = -1);

    void
    update();

    sensor_t
    getValuePid(pid_t pid = -1);

//    void
//    add(pid_t pid = -1);

  private:
    std::map<pid_t, struct timeval> _ctimePIDMap, _ptimePIDMap;
    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
    std::map<pid_t, sensor_t> _pvPIDMap; ///< Previous value of the sensor. The PID is used as the key.

    /// Previous value
    sensor_t _pValue;
  };

} /* namespace cea */
#endif /* SENSORCPUUSAGE_H_ */
