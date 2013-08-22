/*
 * DPELRCpuProcs.cpp
 *
 *  Created on: 10 avr. 2013
 *      Author: fontoura
 */

#include "DPELRCpuProcs.h"

namespace cea
{

  DPELRCpuProcs::DPELRCpuProcs(PowerMeter &pm, double* weights) :
      DPELinearRegression(2, weights, &pm), rp(1)
  {
    this->_sensors.add(ctu);
    this->_sensors.add(rp);

    _isActive = ctu.getStatus();
    _isActive &= rp.getStatus();
  }

  void
  DPELRCpuProcs::updatePid(pid_t pid)
  {
    ctu.updatePid(pid);
    rp.update();
  }

  sensor_t
  DPELRCpuProcs::getValuePid(pid_t pid)
  {
    sensor_t val;
    val.Float = _weights[0] * (1.0f / rp.getValue().U64);
    val.Float += _weights[1] * ctu.getValuePid(pid).Float;
    val.Float += _weights[2];
    return val;
  }

} /* namespace cea */
