#ifndef LIBEC_PE_INVERSE_CPU2_H__
#define LIBEC_PE_INVERSE_CPU2_H__

#include "../Globals.h"
#include "PowerEstimator.h"

namespace cea
{
  /// \brief   Inverse Cpu Power Estimator
  /// \author  Leandro Fontoura Cupertino
  /// \date    Sept 11 2012
  class InverseCpu2 : public PowerEstimator
  {
  public:
    InverseCpu2(PowerMeter* pow, PidStat* sensor);

    void
    update(pid_t pid = -1);

    sensor_t
    getValue(pid_t pid = -1);

  private:
    PowerMeter* _power;
    PIDSensor* _sensor;
    PIDSensor* _cpuTime;
  };

}

#endif
