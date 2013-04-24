#ifndef LIBEC_PE_MIN_MAX_CPU2_H__
#define LIBEC_PE_MIN_MAX_CPU2_H__

#include "../Globals.h"
#include "PowerEstimator.h"

namespace cea
{

  /**
   * @brief   Linear Regression Power Estimator
   * @author  Leandro Fontoura Cupertino
   * @date    Aug 31 2012
   */
  class MinMaxCpu2 : public PowerEstimator
  {
  public:
    /// Differs from MinMaxCpu in relation to the computation of the power once it considers
    /// only the CPU active process and not all alive process.
    MinMaxCpu2(CpuElapsedTime* sensor, float min = 30, float max = 300);

    ~MinMaxCpu2();

    sensor_t
    getValue();

    void
    update();

    sensor_t
    getValuePid(pid_t pid);

    void
    updatePid(pid_t pid);

    void
    add(pid_t pid);

    void
    remove(pid_t pid);

  private:
    CpuElapsedTime* _sensor;
    PIDSensor* _cpuTime;
    float _min;
    float _delta;
    int _procs;

  };

}

#endif
