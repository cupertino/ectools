/*
 eclib - Energy Consumption Library
 Copyright (C) 2012 by Leandro Fontoura Cupertino, IRIT
 Author: Leandro Fontoura Cupertino <fontoura@irit.fr>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEC_SENSORCPUSTATEMSR_H__
#define LIBEC_SENSORCPUSTATEMSR_H__

#include "Sensor.h"

namespace cea
{
  /// Need to be root
  class CpuStateMsr : public Sensor
  {

  public:
    enum mperf_id
    {
      C0 = 0, Cx, AVG_FREQ, MPERF_CSTATE_COUNT
    };

    static const char* ClassName;

    /// Constructor
    CpuStateMsr(unsigned short cpuId = 0, enum mperf_id id = C0);

    /// Destructor
    ~CpuStateMsr();

    /// Updates CPU's frequency 'current value'
    void
    update();

    /// Current CPU's frequency value in KHz (u64)
    /// \return A sensor_t with the current value
    sensor_t
    getValue();

    /// Returns class name
    const char*
    getClassName();

  protected:
    int
    mperf_start(void);
    int
    mperf_stop(void);
    void
    mperf_register(void);
    void
    mperf_unregister(void);
    int
    mperf_get_count_percent(float *percent);
    int
    mperf_get_tsc(unsigned long long *tsc);
    int
    read_msr(int cpu, unsigned int idx, unsigned long long *val);
    int
    mperf_init_stats(unsigned int cpu);
    int
    mperf_measure_stats(unsigned int cpu);
    long long
    timespec_diff_us(struct timespec start, struct timespec end);
    int
    init_maxfreq_mode(void);

    struct timespec time_start, time_end;

    enum MAX_FREQ_MODE
    {
      MAX_FREQ_SYSFS, MAX_FREQ_TSC_REF
    };
    int max_freq_mode;

    unsigned long max_frequency;

    unsigned long long tsc_at_measure_start;
    unsigned long long tsc_at_measure_end;
    unsigned long long mperf_previous_count;
    unsigned long long aperf_previous_count;
    unsigned long long mperf_current_count;
    unsigned long long aperf_current_count;

    int _cpuId;
    enum mperf_id _id;

    /* valid flag for all CPUs. If a MSR read failed it will be zero */
    int is_valid;

  };

}

#endif
