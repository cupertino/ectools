/*
 * DynamicPowerEstimator.cpp
 *
 *  Created on: 28 mars 2013
 *      Author: fontoura
 */

#include <libec/estimator/DynamicPowerEstimator.h>

namespace cea
{

  DynamicPowerEstimator::DynamicPowerEstimator()
  {
  }

  DynamicPowerEstimator::DynamicPowerEstimator(PowerMeter *pm, Calibrator *c)
  {
    _pm = pm;

    this->_isActive = pm->getStatus();

    _calib = c;
  }

  DynamicPowerEstimator::~DynamicPowerEstimator()
  {
    // TODO Auto-generated destructor stub
  }
} /* namespace cea */
