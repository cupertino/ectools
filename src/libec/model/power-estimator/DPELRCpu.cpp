/*
 * DPECpuTime.cpp
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#include <libec/machine-learning/LinearRegression.h>
#include <libec/estimator/DPELinearRegression.h>

#include <libec/estimator/DPELRCpu.h>

namespace cea
{

  DPELRCpu::DPELRCpu(PowerMeter &pm, double* weights) :
      DPELinearRegression(pm, 1, weights)
  {
    _sensors.add(_ctu);
  }

  DPELRCpu::~DPELRCpu()
  {
    clean();
  }

  void
  DPELRCpu::clean()
  {
    DPELinearRegression::clean();

    _calib = NULL;
    _pm = NULL;
    _sensors.clear();
  }

} /* namespace cea */
