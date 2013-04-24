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

#include <libec/sensor/SensorPidCpuTimeElapsed.h>
#include <libec/tools/DebugLog.h>

#if DEBUG
#include <libec/tools/Debug.h>
#endif

namespace cea
{
  CpuElapsedTime::CpuElapsedTime()
  {
    _name = "CPU_ELAPSED_JIFFIES";
    _alias = "EJIFS";
    _type = U64;

    _nptIdle = _nctIdle = 0;

    _latency = 190;

    _isActive = _ct.getStatus();

    gettimeofday(&_cTimeval, NULL);

    update();
    _pValue = _cValue;
  }

  CpuElapsedTime::~CpuElapsedTime()
  {
  }

  sensor_t
  CpuElapsedTime::getValue()
  {
    sensor_t val;

    val.U64 = _cValue.U64 - _pValue.U64;

    return val;
  }

  u64
  CpuElapsedTime::getTotalElapsedTime()
  {
    u64 val;

    val = getValue().U64 + (_nctIdle - _nptIdle);

    return val;
  }

  void
  CpuElapsedTime::update()
  {
#if DEBUG
    Debug::StartClock();
#endif
    _ct.update();

    _pValue.U64 = _cValue.U64;
    _nptIdle = _nctIdle;

    _cValue.U64 = _ct.getValue().U64;
    _nctIdle = _ct.getIdleTime();

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
        << Debug::ElapsedTimeUsec() << "\n";
#endif
  }

  void
  CpuElapsedTime::updatePid(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    if (pid > 0)
      {
        _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
        _cvPIDMap[pid].U64 = _ct.getValuePid(pid).U64;
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
//
//  void
//  CpuElapsedTime::update2(pid_t pid)
//  {
//#if DEBUG
//    Debug::StartClock();
//#endif
//
//    int fd;
//    char sbuf[1024];
//    unsigned long int putime, pstime;
//
//    //dummy variables
//    int d_int;
//    char d_buf[256];
//    char d_char;
//    long d_long;
//
//    static char path[32];
//
//    struct timeval timenow;
//    gettimeofday(&timenow, NULL);
//
//    if (pid > 0)
//      {
//        sprintf(path, "/proc/%d/stat", pid);
//
//        fd = open(path, O_RDONLY, 0);
//        if (fd == -1)
//          return;
//        read(fd, sbuf, 1023);
//        close(fd);
//
//        sscanf(sbuf, "%d (%s %c "
//            "%d %d %d %d %d "
//            "%lu %lu %lu %lu %lu "
//            "%lu %lu ", /* utime stime */
//        &d_int, d_buf, &d_char, &d_int, &d_int, &d_int, &d_int, &d_int, &d_long,
//            &d_long, &d_long, &d_long, &d_long, &putime, &pstime);
//
//        _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
//        _cvPIDMap[pid].U64 = putime + pstime;
//      }
//    else if ((pid == -1) || (pid == -2))
//      {
//        sprintf(path, "/proc/stat");
//
//        fd = open(path, O_RDONLY, 0);
//        if (fd == -1)
//          return;
//        read(fd, sbuf, 1023);
//        close(fd);
//
//        unsigned long tuser, tnice, tsys, tidle;
//
//        sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", /* utime stime */
//            &tuser, &tnice, &tsys, &tidle);
//
//        _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
//        _cvPIDMap[pid].U64 = tuser + tsys;
//        if (pid == -2)
//          _cvPIDMap[pid].U64 += tidle;
//      }
//#if DEBUG
//    DebugLog::cout << _name << "  update time (us): "
//    << Debug::ElapsedTimeUsec() << "\n";
//#endif
//  }

  void
  CpuElapsedTime::add(pid_t pid)
  {
    sensor_t cval, pval;
    _cvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, cval));
    _pvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, pval));

    updatePid(pid);
    _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
  }

  sensor_t
  CpuElapsedTime::getValuePid(pid_t pid)
  {
    sensor_t val;

    updatePid(pid);
    val.U64 = _cvPIDMap[pid].U64 - _pvPIDMap[pid].U64;

    return val;
  }

}
