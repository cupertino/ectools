/*
 * DPECpuTime.h
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#ifndef DPELRCPU_H_
#define DPELRCPU_H_

#include "DPELinearRegression.h"
#include "../sensor/SensorPidCpuTimeUsage.h"

namespace cea
{
  /// Dynamic Power Estimator based on CPU usage.
  /// This is a device proportional dynamic power estimator that uses
  /// the linear regression method to estimate machine/application's
  /// consumption
  class DPELRCpu : public DPELinearRegression
  {
  public:
    DPELRCpu();

    DPELRCpu(PowerMeter &pm, double* weights = NULL);
    virtual
    ~DPELRCpu();

  protected:
    void
    clean();

    CpuTimeUsage _ctu;
  };

} /* namespace cea */
#endif /* DPECPUTIME_H_ */
