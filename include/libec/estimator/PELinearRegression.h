#ifndef LIBEC_STATIC_AVERAGE_H__
#define LIBEC_STATIC_AVERAGE_H__

#include "../Globals.h"
#include "PowerEstimator.h"

namespace cea
{

  /**
   * @brief   Linear Regression Power Estimator
   * @author  Leandro Fontoura Cupertino
   * @date    Aug 31 2012
   */
  class PELinearRegression : public PowerEstimator
  {
  public:
    PELinearRegression(float min = 0, float max = 300, PIDSensor* sensor =
        new SensorCpuUsage());

    void
    updatePid(pid_t pid);

    sensor_t
    getValuePid(pid_t pid);

  private:
    PIDSensor* _sensor;
    float _min;
    float _delta;

    int
    getCountProc();

    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
  };
}

#endif
