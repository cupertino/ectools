/*
 * SensorCpuUsage.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Leandro
 */

#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <libec/sensor/SensorPidCpuTime.h>
#include <libec/sensor/SensorPidCpuTimeShare.h>

#if DEBUG
#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>
#endif

namespace cea
{
  bool deleteCpuTime;
  float totalTime;

  CpuTimeShare::CpuTimeShare()
  {
    _cpuTime = new CpuTime();
    deleteCpuTime = true;
  }

  CpuTimeShare::CpuTimeShare(CpuTime *cpuTime)
  {
    _name = "CPU_TIME_SHARE";
    _alias = "CPU_%";
    _type = Float;

    _cpuTime = cpuTime;

    _isActive = _cpuTime->getStatus();

#if DEBUG
    DebugLog::cout << this;
#endif

  }

  CpuTimeShare::~CpuTimeShare()
  {
    delete _cpuTime;
  }

  void
  CpuTimeShare::update()
  {
    update(-1);
  }

  void
  CpuTimeShare::update(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    if (needUpdate(timeNow, _cTimeval))
      {
        totalTime = _cpuTime->getTotalTime();
      }

    _cvPIDMap[pid].Float = _cpuTime->getValuePid(pid).U64 / totalTime;

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
    << Debug::ElapsedTimeUsec() << "\n";
#endif
  }

// TODO: why do I need this method here if its the same as the PidSensor one??
  void
  CpuTimeShare::add(pid_t pid)
  {
    sensor_t cval;//, pval;
    _cvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, cval));
//    _pvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, pval));
  }

  sensor_t
  CpuTimeShare::getValue(pid_t pid)
  {
    sensor_t val;

    update(pid);
    val.U64 = _cvPIDMap[pid].U64 - _pvPIDMap[pid].U64;

    return val;
  }

}
