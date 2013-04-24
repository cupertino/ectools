/*
 * DPECpuTime.cpp
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#include <libec/machine-learning/LinearRegression.h>
#include <libec/estimator/DPELinearRegression.h>

#include <libec/estimator/DPELRCpuMem.h>

namespace cea
{

  DPELRCpuMem::DPELRCpuMem(PowerMeter &pm, double* weights) :
      DPELinearRegression(pm, 3, weights), _cpu0_freq(0), _cpu1_freq(1)
//      , _cpu0_temp(0), _cpu1_temp(1)
  {
    _sensors.add(_cpu_usage);
    _sensors.add(_cpu0_freq);
    _sensors.add(_cpu1_freq);
//    _sensors.add(_cpu0_temp);
//    _sensors.add(_cpu1_temp);
//    _sensors.add(_mem_usage);
  }

  DPELRCpuMem::~DPELRCpuMem()
  {
    clean();
  }

  void
  DPELRCpuMem::clean()
  {
    DPELinearRegression::clean();

    _calib = NULL;
    _pm = NULL;
    _sensors.clear();
  }

} /* namespace cea */
