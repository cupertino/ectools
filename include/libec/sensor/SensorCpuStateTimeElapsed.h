/*
 * CpuIdleStates.h
 *
 *  Created on: 3 juil. 2013
 *      Author: fontoura
 */

#ifndef LIBEC_CPUSTATETIMEELAPSED_H_
#define LIBEC_CPUSTATETIMEELAPSED_H_

#define PATH_TO_CPU "/sys/devices/system/cpu/"
#define MAX_LINE_LEN 255
#define SYSFS_PATH_MAX 255

#include "Sensor.h"
#include "SensorCpuStateTime.h"

namespace cea
{
  /// \brief CPU States (C-States) sensor
  /// \description
  /// The CpuStates class collects information regarding the
  /// Idle States of the CPU from the
  /// /sys/devices/system/cpu/cpu*/cpuidle/state*/ file.
  class CpuStateTimeElapsed : public cea::Sensor
  {
  public:
    CpuStateTimeElapsed(int stateId = 0, int cpuId = 0);

    virtual
    ~CpuStateTimeElapsed();

    void
    update();

    sensor_t
    getValue();

  protected:
    CpuStateTime *_cst;

    u64 _prev;
  };

} /* namespace cea */
#endif /* CPUIDLESTATES_H_ */
