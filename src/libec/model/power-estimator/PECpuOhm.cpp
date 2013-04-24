// A first static model to test the interface
#include <sys/sysinfo.h>
#include <dirent.h>

#include <libec/tools/DebugLog.h>
#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/estimator/PECpuOhm.h>

namespace cea
{
#define CPU_MAX 2
#define FREQ_MAX 6

  /** Constructor */
  CpuPowerEstimator::CpuPowerEstimator(double capacitance, u64 voltage[],
      u64 frequency[])
  {
    _name = "OHMSLAW_CPU_POWER_ESTIMATOR";
    _alias = "PE_OLC";
    _type = Float;

    _c = capacitance;

    gettimeofday(&_cTimeval, NULL);

    _isActive = true;

    for (int i = 0; i < CPU_MAX; i++)
      {
        _sf[i] = new CpuFreq(i);

        _isActive |= _sf[i]->getStatus();
      }

    _f = new u64[FREQ_MAX];
    _v2 = new u64[FREQ_MAX];
    for (int i = 0; i < FREQ_MAX; i++)
      {
        _f[i] = frequency[i];
        _v2[i] = voltage[i] * voltage[i];
        _fv2[frequency[i]] = frequency[i] * voltage[i] * voltage[i];
      }

    _latency = 400;
  }

  CpuPowerEstimator::~CpuPowerEstimator()
  {
    delete[] _f;
    delete[] _v2;
    delete[] _sf;
  }

  void
  CpuPowerEstimator::updatePid(pid_t pid)
  {
    u64 freq;

    freq = _sf[0]->getValue().U64;

    _cvPIDMap[pid].Float = _c * _fv2[freq]; //times cpu share
  }

  sensor_t
  CpuPowerEstimator::getValuePid(pid_t pid)
  {
    updatePid(pid);

    return _cValue;
  }
}
