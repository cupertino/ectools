//============================================================================
// Name        : SensorPidStat.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.28
// Copyright   : Your copyright notice
// Description : Performance Counter Sensor header
//============================================================================

#include <libec/Globals.h>
#include <libec/tools.h>
#include <libec/sensor/SensorPidStat.h>

#include <cmath>
#include <fcntl.h>

namespace cea
{
  //dummy variables
  int d_int;
  char d_buf[200];
  char d_char;
  long d_long;
  unsigned long d_ulong;
  unsigned long long d_ulonglong;

  unsigned long long _pCpuTime, _cCpuTime;

  PidStat::PidStat(PidStat::TypeId type)
  {
    _fd = 0;
    _cTime = time(NULL);
    _pTime = _cTime;
    _typeId = type;
    _ps_sensor_id = 0;

    switch (_typeId)
      {
    case PidStat::CPU_USAGE:
      _type = Float;
      _name = "PIDSTAT_CPU_USAGE";
      _alias = "%CPU";
      break;
    case PidStat::CPU_LAST:
      _type = U64;
      _name = "PIDSTAT_LAST_CPU";
      _alias = "CPU#";
      break;
    case PidStat::VIRTUAL_MEM_SIZE:
      _type = U64;
      _name = "PIDSTAT_VIRTUAL_MEM_SIZE";
      _alias = "VSIZE";
      break;
    case PidStat::RESIDENT_SET_SIZE:
      _type = U64;
      _name = "PIDSTAT_RESIDENT_SET_SIZE";
      _alias = "RSS";
      break;
    case PidStat::PID_STATE:
      _type = U64;
      _name = "PIDSTATE_STATE";
      _alias = "S";
      break;

      }

    _isActive = Tools::fileExists("/proc/stat");

//    fd = open("/proc/stat", O_RDONLY, 0);
//    _isActive = (fd != -1);
//    close(fd);
  }

  PidStat::~PidStat()
  {
  }

  void PidStat::update(){

  }

  void
  PidStat::add(pid_t pid)
  {
    // if map does not contains key ..
    if (_cvPIDMap.find(pid) == _cvPIDMap.end())
      {
        /*        // Creates the values vectors for the current and previous data
         std::vector<u64> *val_tmp;
         val_tmp = new std::vector<u64>(TYPE_MAX * sizeof(u64), 0);
         _cvPIDMap.insert(std::pair<pid_t, std::vector<u64>*>(pid, val_tmp));
         val_tmp = new std::vector<u64>(TYPE_MAX * sizeof(u64), 0);
         _pvPIDMap.insert(std::pair<pid_t, std::vector<u64>*>(pid, val_tmp));
         */

        struct pid_stats cval, pval;
        _cvPIDMap.insert(std::pair<pid_t, struct pid_stats>(pid, cval));
        _pvPIDMap.insert(std::pair<pid_t, struct pid_stats>(pid, pval));

        struct timeval ctime, ptime;
        _ctimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ctime));
        _ptimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ptime));
      }
  }

  void
  PidStat::remove(pid_t pid)
  {
    _pvPIDMap.erase(pid);
    _cvPIDMap.erase(pid);

    _ctimePIDMap.erase(pid);
    _ptimePIDMap.erase(pid);
  }

  sensor_t
  PidStat::getValuePid(pid_t pid)
  {
    sensor_t retVal;

    updatePid(pid);

    switch (_typeId)
      {
    case PidStat::CPU_USAGE:
      float f1, f2;
      f1 = 100.0f
          * (_cvPIDMap[pid].utime + _cvPIDMap[pid].stime - _pvPIDMap[pid].utime
              - _pvPIDMap[pid].stime);

      //better precision, more cpu consumption
      f2 = _cValue.U64 - _pValue.U64;

      retVal.Float = f1 / f2;

      break;
    case PidStat::CPU_LAST:
      retVal.U64 = _cvPIDMap[pid].processor;
      break;
    case PidStat::VIRTUAL_MEM_SIZE:
      retVal.U64 = _cvPIDMap[pid].vsz;
      break;
    case PidStat::RESIDENT_SET_SIZE:
      retVal.U64 = _cvPIDMap[pid].rss;
      break;
    case PidStat::PID_STATE:
      retVal.U64 = _cvPIDMap[pid].state;
      break;
      }
    return retVal;
  }

  void
  PidStat::updatePid(pid_t pid)
  {
    int fd;
    char sbuf[1024];
    char pstate;
    unsigned long int putime, pstime;
    unsigned long int pvsize;
    long int prss;
    int processor;

    static char path[1024];

    struct timeval timenow;
    gettimeofday(&timenow, NULL);

    if (_ctimePIDMap[pid].tv_sec < timenow.tv_sec)
      {
        _ptimePIDMap[pid] = _ctimePIDMap[pid];
        _ctimePIDMap[pid] = timenow;

        if (pid > 0)
          {
            sprintf(path, "/proc/%d/stat", pid);

            fd = open(path, O_RDONLY, 0);
            if (fd == -1)
              return;
            read(fd, sbuf, 1023);
            close(fd);

            sscanf(sbuf,
                "%d (%s %c "
                    "%d %d %d %d %d "
                    "%lu %lu %lu %lu %lu "
                    "%lu %lu " // utime stime
                    "%ld %ld %ld %ld %ld %ld %llu "
                    "%lu %ld "// vsize rss
                    "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d "
                    "%d"// processor
                , &d_int, d_buf, &pstate, &d_int, &d_int, &d_int, &d_int,
                &d_int,
                &d_ulong, &d_ulong, &d_ulong, &d_ulong, &d_ulong, &putime,
                &pstime, &d_long, &d_long, &d_long, &d_long, &d_long, &d_long,
                &d_ulonglong, &pvsize, &prss, &d_ulong, &d_ulong, &d_ulong,
                &d_ulong, &d_ulong, &d_ulong, &d_ulong, &d_ulong, &d_ulong,
                &d_ulong, &d_ulong, &d_ulong, &d_ulong, &d_int, &processor);

            _pvPIDMap[pid] = _cvPIDMap[pid];

            _cvPIDMap[pid].state = pstate;
            _cvPIDMap[pid].utime = putime;
            _cvPIDMap[pid].stime = pstime;
            _cvPIDMap[pid].vsz = pvsize;
            _cvPIDMap[pid].rss = prss;
            _cvPIDMap[pid].processor = processor;

            //better precision, more cpu consumption
            if (time(NULL) > _cTime)
              {
                unsigned long tuser, tnice, tsys, tidle;

                sprintf(path, "/proc/stat");
                fd = open(path, O_RDONLY, 0);
                if (fd == -1)
                  return;
                read(fd, sbuf, 1023);
                close(fd);

                sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", // utime stime
                    &tuser, &tnice, &tsys, &tidle);

                _pValue.U64 = _cValue.U64;
                _cValue.U64 = tuser + tnice + tsys + tidle;

                _pCpuTime = _cCpuTime;
                _cCpuTime = tuser + tnice + tsys + tidle;

                _pTime = _cTime;
                _cTime = time(NULL);
              }
          }
        else if (pid == -1)
          {
            unsigned long tuser, tnice, tsys, tidle;

            sprintf(path, "/proc/stat");
            fd = open(path, O_RDONLY, 0);
            if (fd == -1)
              return;
            read(fd, sbuf, 1023);
            close(fd);

            sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", /* utime stime */
                &tuser, &tnice, &tsys, &tidle);

            _pvPIDMap[pid] = _cvPIDMap[pid];

            _cvPIDMap[pid].state = 'R';
            _cvPIDMap[pid].utime = tuser;
            _cvPIDMap[pid].stime = tsys;
            _cvPIDMap[pid].processor = 0;

            if (time(NULL) > _cTime)
              {
                _pValue.U64 = _cValue.U64;
                _cValue.U64 = tuser + tnice + tsys + tidle;

//                DebugLog::cout << "user time: " << tuser << "\n";
//                DebugLog::cout << "nice time: " << tnice << "\n";
//                DebugLog::cout << "sys  time: " << tsys << "\n";
//                DebugLog::cout << "idle time: " << tidle << "\n";
//                DebugLog::cout << "cpu  time: " << _cValue.U64 << "\n";

                //        _pCpuTime = _cCpuTime;
                //      _cCpuTime = tuser + tnice + tsys + tidle;

                _pTime = _cTime;
                _cTime = time(NULL);
              }
          }
      }
  }

} /* namespace cea */
