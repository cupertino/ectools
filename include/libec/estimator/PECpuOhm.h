#ifndef LIBEC_PE_CPU_H__
#define LIBEC_PE_CPU_H__

#include "../Globals.h"
#include "../sensor/SensorCpuFreq.h"
#include "../sensor/SensorCPULoad.h"
#include "PowerEstimator.h"

namespace cea
{
  /// \brief   Cpu Power Estimator
  /// \author  Leandro Fontoura Cupertino
  /// \date    Sept 11 2012
  class CpuPowerEstimator : public PowerEstimator
  {
  public:
    CpuPowerEstimator(double capacitance, u64 voltage[], u64 frequency[]);

    ~CpuPowerEstimator();

    void
    updatePid(pid_t pid);

    sensor_t
    getValuePid(pid_t pid);

  private:
    double _c; /// > CPU's capacitance
    u64 *_f; /// > CPU's frequency array
    u64 *_v2; /// > CPU's squared voltage array

    CpuFreq *_sf[2]; /// > CPU frequency sensor
//TODO:    CpuLoad *_sc; /// > CPU load sensor
    std::map<u64, double> _fv2; /// > CPU's frequency to squared voltage mapping
    std::map<pid_t, sensor_t> _cvPIDMap; ///< Current value of the sensor. The PID is used as the key.
  };

}

#endif
