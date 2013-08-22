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
    _pm = NULL;
    _calib = NULL;
  }

  DynamicPowerEstimator::DynamicPowerEstimator(PowerMeter *pm, Calibrator *c)
  {
    _pm = pm;
    _calib = c;

    this->_isActive = pm->getStatus();
  }

  DynamicPowerEstimator::~DynamicPowerEstimator()
  {
    // TODO Auto-generated destructor stub
  }
} /* namespace cea */
