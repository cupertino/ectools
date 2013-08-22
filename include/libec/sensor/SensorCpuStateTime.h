/*
 * CpuIdleStates.h
 *
 *  Created on: 3 juil. 2013
 *      Author: fontoura
 */

#ifndef LIBEC_CPUSTATETIME_H_
#define LIBEC_CPUSTATETIME_H_

#define PATH_TO_CPU "/sys/devices/system/cpu/"
#define MAX_LINE_LEN 255
#define SYSFS_PATH_MAX 255

#include "Sensor.h"

namespace cea
{
  /// \brief CPU States (C-States) sensor
  /// \description
  /// The CpuStates class collects information regarding the
  /// Idle States of the CPU from the
  /// /sys/devices/system/cpu/cpu*/cpuidle/state*/ file.
  class CpuStateTime : public cea::Sensor
  {
  public:
    CpuStateTime(int stateId = 0, int cpuId = 0);

    virtual
    ~CpuStateTime();

    void
    update();

    sensor_t
    getValue();

    static int
    getNumberofStates();

    std::string
    getStateName();

  protected:

    bool
    checkAccess();

    unsigned int _cpuId;
    unsigned int _cstateId;

    /// Latency to exit out of this idle state (in microseconds)
    int _cstateLatency;

    /// Power consumed while in this idle state (in milliwatts)
    long _cstatePower;

    /// Total time spent in this idle state (in microseconds)
    unsigned long long _cstateTime;

    /// Number of times this state was entered (count)
    long _cstateUsage;

    /// Small description about the idle state
    std::string _cstateDesc;

    /// Name of the idle state
    std::string _cstateName;

    /// Path to cpuinfo_cur_freq file
    char _filepath[SYSFS_PATH_MAX];
  };

} /* namespace cea */
#endif /* CPUIDLESTATES_H_ */
