///*
// * SensorCpuUsage.cpp
// *
// *  Created on: Aug 15, 2012
// *      Author: Leandro
// */
//
//#include <libec/sensor/SensorCpuUsage.h>
//#include <cmath>
//#include <errno.h>
//#include <fcntl.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/dir.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//
//namespace cea
//{
//
//  SensorCpuUsage::SensorCpuUsage()
//  {
//    int fd;
//
//    _cTime = time(NULL);
//    _pTime = _cTime;
//    _type = Float;
//    _alias = "%%cpu";
//    _name = "CPU_USAGE";
//
//    fd = open("/proc/stat", O_RDONLY, 0);
//    _isActive = (fd != -1);
//    close(fd);
//  }
//
//  SensorCpuUsage::~SensorCpuUsage()
//  {
//  }
//
//  void
//  SensorCpuUsage::updatePid(pid_t pid)
//  {
//  }
//
//  void
//  SensorCpuUsage::update()
//  {
//
//  }
//
////  void
////  SensorCpuUsage::add(pid_t pid)
////  {
////    sensor_t cval, pval;
////    _cvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, cval));
////    _pvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, pval));
////
////    struct timeval ctime, ptime;
////    _ctimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ctime));
////    _ptimePIDMap.insert(std::pair<pid_t, struct timeval>(pid, ptime));
////  }
//
//  long long c, p;
//
//  sensor_t
//  SensorCpuUsage::getValuePid(pid_t pid)
//  {
//    sensor_t val;
//    val.U64 = 0;
//    int fd;
//    char sbuf[1024];
//    unsigned long int putime, pstime;
//
//    //dummy variables
//    int d_int;
//    char *d_buf;
//    d_buf = (char*) malloc(200 * sizeof(char));
//    char d_char;
//    long d_long;
////    unsigned long d_ulong;
//
//    static char path[1024];
//
//    struct timeval timenow;
//    gettimeofday(&timenow, NULL);
//
//    if (_ctimePIDMap[pid].tv_sec < timenow.tv_sec)
//      {
//        if (pid > 0)
//          {
//            sprintf(path, "/proc/%d/stat", pid);
//
//            fd = open(path, O_RDONLY, 0);
//            if (fd == -1)
//              return val;
//            read(fd, sbuf, 1023);
//            close(fd);
//
//            sscanf(sbuf, "%d (%s %c "
//                "%d %d %d %d %d "
//                "%lu %lu %lu %lu %lu "
//                "%lu %lu ", /* utime stime */
//            &d_int, d_buf, &d_char, &d_int, &d_int, &d_int, &d_int, &d_int,
//                &d_long, &d_long, &d_long, &d_long, &d_long, &putime, &pstime);
//
//            _ptimePIDMap[pid] = _ctimePIDMap[pid];
//            _ctimePIDMap[pid] = timenow;
//
//            _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
//            _cvPIDMap[pid].U64 = putime + pstime;
//
//            //better precision, more cpu consumption
//            if (time(NULL) > _cTime)
//              {
//                unsigned long tuser, tnice, tsys, tidle;
//
//                sprintf(path, "/proc/stat");
//                fd = open(path, O_RDONLY, 0);
//                if (fd == -1)
//                  return val;
//                read(fd, sbuf, 1023);
//                close(fd);
//
//                sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", // utime stime
//                    &tuser, &tnice, &tsys, &tidle);
//
//                //              std::cerr << "user: " << tuser << " nice: " << tnice << " sys: "
//                //                << tsys << " idle: " << tidle << std::endl;
//
//                _pValue.U64 = _cValue.U64;
//                _cValue.U64 = tuser + tnice + tsys + tidle;
//
//                _pTime = _cTime;
//                _cTime = time(NULL);
//              }
//          }
//        else if (pid == -1)
//          {
//            sprintf(path, "/proc/stat");
//
//            fd = open(path, O_RDONLY, 0);
//            if (fd == -1)
//              return val;
//            read(fd, sbuf, 1023);
//            close(fd);
//
//            unsigned long tuser, tnice, tsys, tidle;
//
//            sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", /* utime stime */
//                &tuser, &tnice, &tsys, &tidle);
//
//            _ptimePIDMap[pid] = _ctimePIDMap[pid];
//            _ctimePIDMap[pid] = timenow;
//
//            _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
//            _cvPIDMap[pid].U64 = tuser + tsys;
//          }
//      }
//
//    delete d_buf;
//
//    float f1, f2;
//    f1 = 100.0f * (_cvPIDMap[pid].U64 - _pvPIDMap[pid].U64);
//
//    //f2 = _ctimePIDMap[pid].tv_sec - _ptimePIDMap[pid].tv_sec;
//    //f2 += (_ctimePIDMap[pid].tv_usec - _ptimePIDMap[pid].tv_usec) / (1000000);
//    //f2 *= sysconf(_SC_CLK_TCK);
//
//    //better precision, more cpu consumption
//    f2 = _cValue.U64 - _pValue.U64;
//
//    //round result
//    val.Float = (float) floor(10 * f1 / f2) / 10;
//
//    return val;
//  }
//
//} /* namespace cea */
