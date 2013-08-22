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
  DPELRCpu::DPELRCpu() :
      DPELinearRegression(1)
  {
    _name = "POWER_DYN_LR_CPU";
    _alias = "DPELRCpu";
    _sensors.add(_ctu);
    _isActive = _ctu.getStatus();
  }

  DPELRCpu::DPELRCpu(PowerMeter &pm, double* weights) :
      DPELinearRegression(1, weights, &pm)
  {
    _sensors.add(_ctu);
    _isActive = _ctu.getStatus();
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
