/*
 * DPELRCpuProcs.h
 *
 *  Created on: 10 avr. 2013
 *      Author: fontoura
 */

#ifndef DPELRCPUPROCS_H_
#define DPELRCPUPROCS_H_

#include <libec/estimator/DPELinearRegression.h>
#include <libec/sensor/SensorRunningProcs.h>
#include <libec/sensor/SensorPidCpuTimeUsage.h>

namespace cea
{
  class DPELRCpuProcs : public DPELinearRegression
  {
  public:
    DPELRCpuProcs(PowerMeter &pm, double* weights);

    void
    updatePid(pid_t pid);

    sensor_t
    getValuePid(pid_t pid);

  protected:
    RunningProcs rp;
    CpuTimeUsage ctu;
  };

} /* namespace cea */
#endif /* DPELRCPUPROCS_H_ */
