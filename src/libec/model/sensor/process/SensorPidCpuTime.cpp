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

#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidCpuTime.h>
#include <libec/tools/DebugLog.h>

#if DEBUG
#include <libec/tools/Debug.h>
#endif

namespace cea
{
  const char* CpuTime::ClassName = "CpuTime";

  CpuTime::CpuTime(char cpuId)
  {
    _name = "CPU_TIME_JIFFIES";
    _alias = "CPU_JIFS";
    _type = U64;
    _cIdleTime = 0;
    _cValue.U64 = 0;
    _cpValue = 0;
    _latency = 0;
    _cpuId = cpuId;

    _isActive = (access("/proc/stat", R_OK) == 0);
    _isActive &= (_cpuId < checkCpus());
  }

  CpuTime::CpuTime(const std::string &xmlTag) :
      PIDSensor(xmlTag)
  {
    _type = U64;
    _cIdleTime = 0;
    _cValue.U64 = 0;
    _cpValue = 0;
    _cpuId = -1;

    _isActive = (access("/proc/stat", R_OK) == 0);
//    _cpus = checkCpus();
//    _ccValue = new u64[_cpus];
  }

  CpuTime::~CpuTime()
  {
//    delete[] _ccValue;
  }

  /// Checks the number of CPUs available on the /proc/stat file
  char
  CpuTime::checkCpus()
  {
    std::ifstream ifs("/proc/stat");
    std::string s;
    char cpus = -1;
    unsigned found = 0;

    if (ifs.good())
      {
        std::getline(ifs, s);

        do
          {
            cpus++;
            std::getline(ifs, s);
            found = s.find("cpu");
          }
        while (found == 0);
      }
    ifs.close();

    return cpus;
  }

  const char*
  CpuTime::getClassName()
  {
    return ClassName;
  }

  u64
  CpuTime::getTotalTime()
  {
    u64 val;

    val = getValue().U64 + _cIdleTime;

    return val;
  }

  sensor_t
  CpuTime::getValue()
  {
    return _cValue;
  }

  u64
  CpuTime::getIdleTime()
  {
    return _cIdleTime;
  }

  void
  CpuTime::update()
  {
    unsigned long int tuser, tsys, tnice, tidle;
    std::ifstream ifs("/proc/stat");

    if (ifs.good())
      {
        ifs.ignore(8, ' '); //"cpu"
        ifs >> tuser >> tnice >> tsys >> tidle;

        _cValue.U64 = tuser + tsys;
        _cIdleTime = tidle;
      }
    ifs.close();
  }

  sensor_t
  CpuTime::getValuePid(pid_t pid)
  {
    sensor_t val;

    val.U64 = _cpValue;

    return val;
  }

  void
  CpuTime::updatePid(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    unsigned long int tuser, tsys;

    if (pid > 0)
      {
        static char path[32];
        sprintf(path, "/proc/%d/stat", pid);
        std::ifstream ifs(path);
        if (ifs.good())
          {
            ifs.ignore(64, ' '); //pid
            ifs.ignore(256, ' '); //comm
            ifs.ignore(64, ' '); //state
            ifs.ignore(64, ' '); //ppid
            ifs.ignore(64, ' '); //pgrp
            ifs.ignore(64, ' '); //session
            ifs.ignore(64, ' '); //tty_nr
            ifs.ignore(64, ' '); //tpgid
            ifs.ignore(64, ' '); //flags
            ifs.ignore(64, ' '); //minflt
            ifs.ignore(64, ' '); //cminflt
            ifs.ignore(64, ' '); //majflt
            ifs.ignore(64, ' '); //cmajflt
            ifs >> tuser >> tsys;
//            _cvPIDMap[pid].U64 = tuser + tsys;
            _cpValue = tuser + tsys;
          }
        ifs.close();
      }
    else
      {
        DebugLog::cout << _name
            << " ERROR Invalid PID in updatePID of sensor.\n";
        throw 1; //INVALID PID EXCEPTION
      }

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
        << Debug::ElapsedTimeUsec() << "\n";
#endif
  }

}
