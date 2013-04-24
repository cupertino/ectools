#include <sys/sysinfo.h>
#include <cmath>

#include <libec/tools/DebugLog.h>
#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/device/SystemInfo.h>
#include <libec/estimator/PEMinMaxCpu.h>

namespace cea
{
  /** Constructor */
  MinMaxCpu::MinMaxCpu(float min, float max)
  {
    _name = "MIN_MAX_CPU_POWER_ESTIMATOR";
    _alias = "PE_MMC";
    _type = Float;
    _min = min;
    _delta = max - min;
    _isActive = _sensor.getStatus();
  }

  sensor_t
  MinMaxCpu::getValue()
  {
    return _cValue;
  }

  void
  MinMaxCpu::update()
  {
    _sensor.update();

    _cValue.Float = (float) ((_delta
        * ((float) _sensor.getValue().U64 / _sensor.getTotalElapsedTime()))
        + _min);
  }

  void
  MinMaxCpu::add(pid_t pid)
  {
  }

  void
  MinMaxCpu::remove(pid_t pid)
  {
  }

  sensor_t
  MinMaxCpu::getValuePid(pid_t pid)
  {
    updatePid(pid);

    return _cValue;
  }

  void
  MinMaxCpu::updatePid(pid_t pid)
  {
    if (pid > 0)
      {
        _cValue.Float = (_delta
            * ((float) _sensor.getValuePid(pid).U64
                / _sensor.getTotalElapsedTime()))
            + (_min / SystemInfo::countProc());
      }
    else
      {
        DebugLog::cout << _name << " ERROR ";
        throw 1;
      }

//    _cValue.Float = (float) floor(
//        10
//            * ((_delta * _sensor->getValue(pid).Float) / 100
//                + _min / getCountProc())) / 10;
  }
}
