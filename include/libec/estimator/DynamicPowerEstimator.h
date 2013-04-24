/*
 * DynamicPowerEstimator.h
 *
 *  Created on: 28 mars 2013
 *      Author: fontoura
 */

#ifndef DYNAMICPOWERESTIMATOR_H_
#define DYNAMICPOWERESTIMATOR_H_

#include <list>

#include "PowerEstimator.h"
#include "../sensor/SensorList.h"
#include "../sensor/SensorPower.h"
#include "../machine-learning/Calibrator.h"

namespace cea
{

  /// Dynamic Power Estimator class uses a calibration method to
  /// update the estimator's parameters.
  class DynamicPowerEstimator : public PowerEstimator
  {
  public:
    DynamicPowerEstimator();
    DynamicPowerEstimator(PowerMeter *pm, Calibrator *c = NULL);

    virtual
    ~DynamicPowerEstimator();

    /// Calibrate's the power estimator dynamically
    virtual void
    calibrate() = 0;

  protected:
    SensorList _sensors;
    PowerMeter *_pm;
    Calibrator *_calib;
  };

} /* namespace cea */
#endif /* DYNAMICPOWERESTIMATOR_H_ */
