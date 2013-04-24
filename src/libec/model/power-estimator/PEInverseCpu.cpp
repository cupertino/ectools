// A first static model to test the interface
#include <sys/sysinfo.h>
#include <dirent.h>

#include <libec/tools/DebugLog.h>
#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/estimator/PEInverseCpu.h>

#if DEBUG
#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>
#endif

namespace cea
{
  /** Constructor */
  InverseCpu::InverseCpu(PowerMeter* pow, CpuElapsedTime* sensor,
      unsigned int latency)
  {
    _name = "INVERSE_CPU_POWER_ESTIMATOR";
    _alias = "PE_IC";
    _type = Float;
    _cet = sensor;
    _power = pow;

    _isActive = pow->getStatus() && sensor->getStatus();
    gettimeofday(&_cTimeval, NULL);

    _cet->getValue().U64;
    _totalInv = 1;
    _latency = latency;
    _lastPow = 0;

#if DEBUG
    DebugLog::cout << this;
#endif
  }

  void
  InverseCpu::update()
  {
    _power->update();
    _cet->update();
  }

  sensor_t
  InverseCpu::getValue()
  {
    return _power->getValue();
  }

  void
  InverseCpu::updatePid(pid_t pid)
  {
#if DEBUG
    Debug::StartClock();
#endif

    struct timeval timeNow;
    u64 ct = 0ULL;
    u64 ul = 0ULL;

    //only update the Nodes variables after the latency time
    gettimeofday(&timeNow, NULL);
    if (needUpdate(timeNow, _cTimeval))
      {
        if (_power->getType() == U64)
          _lastPow = (float) _power->getValue().U64;
        else
          _lastPow = _power->getValue().Float;

        ct = _cet->getValue().U64;

        if (ct > 0ULL)
          {
            _totalInv = _lastPow / ct;
            _cTimeval = timeNow;
          }

#if DEBUG
        DebugLog::cout << _name << " Updated node's CPU time\n";
        DebugLog::cout << _name << "   value:            " << _totalInv << "\n";
        DebugLog::cout << _name << "   power:            " << _lastPow << "\n";
        DebugLog::cout << _name << "   CPU time:         " << ct << "\n";
#endif
      }

    ul = _cet->getValuePid(pid).U64;
    _cValue.Float = ul * _totalInv;

#if DEBUG
    DebugLog::cout << _name << " Updated process' variables\n";
    DebugLog::cout << _name << "   pid:              " << pid << "\n";
    DebugLog::cout << _name << "   update time (us): "
    << Debug::ElapsedTimeUsec() << "\n";
    DebugLog::cout << _name << "   total Inverse:    " << _totalInv << "\n";
    DebugLog::cout << _name << "   last Power:       " << _lastPow << "\n";
    DebugLog::cout << _name << "   last CPU time:    " << ul << "\n";
    DebugLog::cout << _name << "   value:            " << _cValue.Float << "\n";
#endif
  }

  sensor_t
  InverseCpu::getValuePid(pid_t pid)
  {
    updatePid(pid);

#if DEBUG
    if (_cValue.Float != _cValue.Float)
    DebugLog::cout << _name << " value=" << _cValue.Float << "\n";
#endif

    return _cValue;
  }
}
