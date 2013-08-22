#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <libec/sensor/SensorPidMemUsage.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>

#if DEBUG
#include <libec/tools/Debug.h>
#endif

namespace cea
{
  const char* MemUsage::ClassName = "MemUsage";

  MemUsage::MemUsage()
  {
    _name = "MEMORY_USAGE";
    _alias = "MEMu";
    _type = Float;
    _cValue.U64 = 0;
    _latency = 0;

    _memTotal = getAvailableMemory();

    _isActive = _rss.getStatus();
    _isActive &= (access("/proc/meminfo", R_OK) == 0);
  }

  MemUsage::~MemUsage()
  {
  }

  const char*
  MemUsage::getClassName()
  {
    return ClassName;
  }

  sensor_t
  MemUsage::getValue()
  {
    return _cValue;
  }

  void
  MemUsage::update()
  {
    _rss.update();

    _cValue.Float = (float) _rss.getValue().U64 / _memTotal;
  }

  sensor_t
  MemUsage::getValuePid(pid_t pid)
  {
    sensor_t val;

    val.Float = (float) _rss.toKb(_rss.getValuePid(pid).U64) / _memTotal;

    return val;
  }

  void
  MemUsage::updatePid(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    _rss.updatePid(pid);

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
    << Debug::ElapsedTimeUsec() << "\n";
#endif
  }

  u64
  MemUsage::getAvailableMemory()
  {
    u64 val;

    std::ifstream ifs("/proc/meminfo");
    if (ifs.good())
      {
        ifs.ignore(256, ' '); // ignore until space
        ifs >> val;
      }
    ifs.close();

    return val;
  }

}
