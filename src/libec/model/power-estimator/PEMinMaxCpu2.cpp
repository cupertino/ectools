#include <sys/sysinfo.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/device/SystemInfo.h>
#include <libec/estimator/PEMinMaxCpu2.h>

namespace cea
{
  /** Constructor */
  MinMaxCpu2::MinMaxCpu2(CpuElapsedTime* sensor, float min, float max)
  {
    _name = "MIN_MAX_CPU_POWER_ESTIMATOR2";
    _alias = "PE_MMC2";
    _isActive = true;
    _type = Float;
    _min = min;
    _delta = max - min;
    _sensor = sensor;

    _cpuTime = new CpuElapsedTime();
    _procs = 1;
  }

  MinMaxCpu2::~MinMaxCpu2()
  {
    delete _cpuTime;
  }

  void
  MinMaxCpu2::add(pid_t pid)
  {
  }

  void
  MinMaxCpu2::remove(pid_t pid)
  {
  }

  void
  MinMaxCpu2::update()
  {
    _cValue.Float = (_delta
        * ((float) _sensor->getValue().U64 / _sensor->getTotalElapsedTime()))
        + _min;
  }

  void
  MinMaxCpu2::updatePid(pid_t pid)
  {
    if (pid > 0)
      {
        u64 val;
        val = _sensor->getValuePid(pid).U64;
        if (val > 0)
          {
            time_t now;
            now = time(NULL);
            if (now > _cTime)
              {
                _procs = SystemInfo::countActiveProc();
                _cTime = now;
              }

            _cValue.Float = (_delta
                * ((float) val / _sensor->getTotalElapsedTime()))
                + (_min / _procs);
          }
        else
          _cValue.Float = 0.0f;
//
//        if (val > 0)
//          {
//            time_t now;
//            now = time(NULL);
//            if (now > _cTime)
//              {
//                _procs = SystemInfo::countActiveProc();
//                _cTime = now;
//              }
//
//            _cValue.Float = (_delta
//                * ((float) val / _sensor->getTotalElapsedTime()))
//                + (_min / SystemInfo::countActiveProc());
//
//          }
//        else
//          _cValue.Float = 0.0;
      }
    else
      {
        DebugLog::cout << _name << " ERROR!\n";
        throw 1;
      }
  }

  sensor_t
  MinMaxCpu2::getValue()
  {
    return _cValue;
  }

  sensor_t
  MinMaxCpu2::getValuePid(pid_t pid)
  {
    updatePid(pid);

    return _cValue;
  }
}
