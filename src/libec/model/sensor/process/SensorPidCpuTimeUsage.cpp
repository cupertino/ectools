/*
 * SensorCpuUsage.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

#include <libec/sensor/SensorPidCpuTimeUsage.h>

namespace cea
{

  CpuTimeUsage::CpuTimeUsage()
  {
    clean();
  }

  CpuTimeUsage::~CpuTimeUsage()
  {
  }

  void
  CpuTimeUsage::updatePid(pid_t pid)
  {
    _cet.updatePid(pid);
    struct timeval tvNow;
    if (needUpdate(tvNow, _pTv))
      {
        _pTv = tvNow;
        update();
      }
  }

  void
  CpuTimeUsage::update()
  {
    _cet.update();

    _cValue.Float = ((float) _cet.getValue().U64 / _cet.getTotalElapsedTime());
  }

  void
  CpuTimeUsage::add(pid_t pid)
  {
    sensor_t cval, pval;
    _cvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, cval));
    _pvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, pval));

    struct timeval ctime, ptime;
    _ctimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ctime));
    _ptimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ptime));
  }

  long long c, p;

  sensor_t
  CpuTimeUsage::getValue()
  {
    return _cValue;
  }

  sensor_t
  CpuTimeUsage::getValuePid(pid_t pid)
  {
    sensor_t val;

    val.Float = (float) _cet.getValuePid(pid).U64 / _cet.getTotalElapsedTime();

    return val;
  }

  void
  CpuTimeUsage::clean()
  {
    PIDSensor::clean();

    _cTime = time(NULL);
    _pTime = _cTime;
    _type = Float;
    _alias = "CPUu";
    _name = "CPU_USAGE";
    _latency = 200;
    _isActive = (access("/proc/stat", R_OK) == 0);
  }

} /* namespace cea */
