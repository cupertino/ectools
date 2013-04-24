/*
 * DPECpuTime.h
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#ifndef DPELRCPUMEM_H_
#define DPELRCPUMEM_H_

#include "DPELinearRegression.h"
#include "../sensor/SensorPidCpuTimeUsage.h"
#include "../sensor/SensorCpuFreq.h"
#include "../sensor/SensorCpuTemp.h"

namespace cea
{
  /// Dynamic Power Estimator based on CPU usage.
  /// This is a device proportional dynamic power estimator that uses
  /// the linear regression method to estimate machine/application's
  /// consumption
  class DPELRCpuMem : public DPELinearRegression
  {
  public:
    DPELRCpuMem(PowerMeter &pm, double* weights = NULL);
    virtual
    ~DPELRCpuMem();

  protected:
    void
    clean();

    CpuTimeUsage _cpu_usage;
    CpuFreq _cpu0_freq;
    CpuFreq _cpu1_freq;
//    CpuTemp _cpu0_temp;
//    CpuTemp _cpu1_temp;
//    MemoryUsage _mem_usage;
  };

} /* namespace cea */
#endif /* DPECPUTIME_H_ */
