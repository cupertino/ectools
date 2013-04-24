// A first static model to test the interface
#include <sys/sysinfo.h>
#include <dirent.h>

#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/estimator/PEInverseCpu2.h>

namespace cea
{
  /** Constructor */
  InverseCpu2::InverseCpu2(PowerMeter* pow, PidStat* sensor)
  {
    _name = "INVERSE_CPU_POWER_ESTIMATOR";
    _alias = "PE_IC";
    _type = Float;
    _sensor = sensor;
    _power = pow;

    _isActive = pow->getStatus() && sensor->getStatus();

    _cpuTime = new CpuTime();
  }

  void
  InverseCpu2::update(pid_t pid)
  {
    if (_cpuTime->getValuePid(pid).U64 > 0)
      {
        if (_power->getType() == U64)
          _cValue.Float = _power->getValue().U64
              * (_sensor->getValuePid(pid).Float / _sensor->getValuePid(-1).Float);
        else
          _cValue.Float = _power->getValue().Float
              * (_sensor->getValuePid(pid).Float / _sensor->getValuePid(-1).Float);
      }
  }

  sensor_t
  InverseCpu2::getValue(pid_t pid)
  {
    update(pid);

    return _cValue;
  }
}
