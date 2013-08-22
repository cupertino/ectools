#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidMemRss.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>

#if DEBUG
#include <libec/tools/Debug.h>
#endif

namespace cea
{
  const char* MemRss::ClassName = "MemRss";

  MemRss::MemRss()
  {
    _name = "MEMORY_RESIDENT_SET_SIZE";
    _alias = "MEM_RSS";
    _type = U64;
    _cValue.U64 = 0;
    _latency = 0;

    getPageSize();

    _isActive = (access("/proc/stat", R_OK) == 0);
  }

  MemRss::~MemRss()
  {
  }

  const char*
  MemRss::getClassName()
  {
    return ClassName;
  }

  sensor_t
  MemRss::getValue()
  {
    return _cValue;
  }

  void
  MemRss::update()
  {
    u64 _memTotal, _memUsed, _memFree, _memShared, _memBuffers, _memCached;

    std::stringstream ss;
    ss << cea::Tools::exec("free -k");
    ss.ignore(256, '\n');
    ss.ignore(256, ' '); // ignore until space
    ss >> _memTotal >> _memUsed >> _memFree >> _memShared >> _memBuffers
        >> _memCached;

    _cValue.U64 = (_memUsed - _memBuffers - _memCached);
  }

  sensor_t
  MemRss::getValuePid(pid_t pid)
  {
    sensor_t val;

    val.U64 = _memPid[pid];

    return val;
  }

  void
  MemRss::updatePid(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    char buff[32];

    sprintf(buff, "/proc/%d/statm", pid);
    std::ifstream ifs(buff);
    if (ifs.good())
      {
        ifs.ignore(256, ' '); // ignore until space
        ifs >> _memPid[pid];
      }

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
        << Debug::ElapsedTimeUsec() << "\n";
#endif
  }

  void
  MemRss::getPageSize()
  {
    unsigned pageSize;

    pageToKbShift = 0;

    // from top: get virtual page size -- nearing huge!
    pageSize = getpagesize();
    int i = pageSize;
//    pageSize /= 1024;
    while (i > 1024)
      {
        i >>= 1;
        pageToKbShift++;
      }
  }

  u64
  MemRss::toKb(u64 pages)
  {
    return (pages) << pageToKbShift;
  }

  u64
  MemRss::toPages(u64 kb)
  {
    return (kb) >> pageToKbShift;
  }

}
