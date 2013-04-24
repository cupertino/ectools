#ifndef LIBEC_PE_MIN_MAX_CPU_H__
#define LIBEC_PE_MIN_MAX_CPU_H__

#include "../Globals.h"
#include "PowerEstimator.h"
#include "../sensor/SensorPidCpuTimeElapsed.h"

namespace cea
{

  /**
   * @brief   Linear Regression Power Estimator
   * @author  Leandro Fontoura Cupertino
   * @date    Aug 31 2012
   */
  class MinMaxCpu : public PowerEstimator
  {
  public:
    MinMaxCpu(float min = 0, float max = 300);

    sensor_t
    getValue();

    void
    update();

    sensor_t
    getValuePid(pid_t pid = -1);

    void
    updatePid(pid_t pid);

    void
    add(pid_t pid);

    void
    remove(pid_t pid);

  private:
    CpuElapsedTime _sensor;
    float _min;
    float _delta;

    int
    getCountProc();
  };

}

#endif
