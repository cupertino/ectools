#ifndef LIBEC_PE_INVERSE_CPU_H__
#define LIBEC_PE_INVERSE_CPU_H__

#include "../Globals.h"
#include "PowerEstimator.h"
#include "../sensor/SensorPidCpuTimeElapsed.h"

namespace cea
{
  /// @brief   Inverse Cpu Power Estimator
  /// @author  Leandro Fontoura Cupertino
  /// @date    Sept 11 2012
  class InverseCpu : public PowerEstimator
  {
  public:
    InverseCpu(PowerMeter* pow, CpuElapsedTime* sensor, unsigned int latency =
        200);

    void update();

    sensor_t
    getValue();

    void
    updatePid(pid_t pid);

    sensor_t
    getValuePid(pid_t pid);

  private:
    /// Determines if the elapsed time is greater than the sensor's latency
//    bool
//    needUpdate(const struct timeval *tv_now, const struct timeval *tv_prev);

    PowerMeter* _power;
    CpuElapsedTime* _cet;

    double _totalInv;
    float _lastPow;
  };

}

#endif
