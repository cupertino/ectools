#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidDiskIO.h>
#include <libec/tools/DebugLog.h>

#define IO_READ    "read_bytes:"
#define IO_WRITE   "write_bytes:"
#define IO_CWRITE  "cancelled_write_bytes:"

namespace cea
{
  ///////////////////////////////////////////////////////////////////
  // Static Members
  ///////////////////////////////////////////////////////////////////
  const char* DiskIO::ClassName = "CpuTime";

  ///////////////////////////////////////////////////////////////////
  // Public Members
  ///////////////////////////////////////////////////////////////////
  DiskIO::DiskIO(const char* dev)
  {
    _name = "DISK_IO";
    _alias = "DISK_IO";

    _type = U64;

    _isActive = (access("/proc/1/io", R_OK) == 0);
    if (!_isActive)
      DebugLog::writeMsg(DebugLog::WARNING, "DiskIO::DiskIO()",
          "The sensor is not active. Check /proc/[pid]/io file permissions.");

    sprintf(_macPath, "/sys/block/%s/stat", dev);
    _isActive &= (access(_macPath, R_OK) == 0);
  }

  DiskIO::~DiskIO()
  {
  }

  void
  DiskIO::add(pid_t pid)
  {
    iodata data =
      { 0, 0 };
    _pidValue.insert(std::pair<pid_t, iodata>(pid, data));
  }

  void
  DiskIO::remove(pid_t pid)
  {
    _pidValue.erase(pid);
  }

  void
  DiskIO::update()
  {
    std::ifstream ifs(_macPath);
    if (ifs.good())
      {
        ifs >> _macValue.read; // Field  1: # of reads completed
        ifs.ignore(' ', 254); // Filed 2: # of reads merged
        ifs.ignore(' ', 254); // Filed 3: # of sectors read
        ifs.ignore(' ', 254); // Field 4: # of milliseconds spent reading
        ifs >> _macValue.write; //Field 5: # of writes completed
      }
    ifs.close();
  }

  void
  DiskIO::updatePid(pid_t pid)
  {
    if (pid > 0)
      {
        char buff[2048];
        sprintf(buff, "/proc/%d/io", pid);
        std::ifstream ifs(buff);
        if (ifs.good())
          {
            while (ifs.good())
              {
                ifs.getline(buff, sizeof buff);
                if (memcmp(buff, IO_READ, sizeof IO_READ - 1) == 0)
                  {
                    sscanf(buff, "%*s %llu", &_pidValue[pid].read);
                    continue;
                  }
                if (memcmp(buff, IO_WRITE, sizeof IO_READ - 1) == 0)
                  {
                    sscanf(buff, "%*s %llu", &_pidValue[pid].write);
                    continue;
                  }
//            if (memcmp(buff, IO_CWRITE, sizeof IO_READ - 1) == 0)
//              {
//                sscanf(buff, "%*s %llu", &_pidValue[pid].cwrite);
//                continue;
//              }
              }
            ifs.close();
          }
        else
          {
            DebugLog::writeMsg(DebugLog::ERROR, "DiskIO::updatePid(pid_t)",
                "Could not open the /proc/[pid]/io file. Check permissions.");
          }
      }
  }

  sensor_t
  DiskIO::getValue()
  {
    _cValue.U64 = getReadBytes() + getWriteBytes();
    return _cValue;
  }

  sensor_t
  DiskIO::getValuePid(pid_t pid)
  {
    sensor_t val;
    val.U64 = getReadBytes(pid) + getWriteBytes(pid);
    return val;
  }

  u64
  DiskIO::getReadBytes()
  {
    return _macValue.read;
  }

  u64
  DiskIO::getReadBytes(pid_t pid)
  {
    return _pidValue[pid].read;
  }

  u64
  DiskIO::getWriteBytes()
  {
    return _macValue.write;
  }

  u64
  DiskIO::getWriteBytes(pid_t pid)
  {
    return _pidValue[pid].write;
  }

//  u64
//  DiskIO::getCancelWriteBytes(pid_t pid)
//  {
//    return _pidValue[pid].cwrite;
//  }

}
