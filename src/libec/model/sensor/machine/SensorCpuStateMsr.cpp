/*
 *  (C) 2010,2011       Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Based on the idea from Michael Matz <matz@suse.de>
 *
 */

#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorCpuStateMsr.h>

#include <fcntl.h>
#include <time.h>
#include <cpufreq.h>

namespace cea
{
  ///////////////////////////////////////////////////////////////////
  // Static Members
  ///////////////////////////////////////////////////////////////////
  const char* CpuStateMsr::ClassName = "CpuStateMsr";

  ///////////////////////////////////////////////////////////////////
  // Public Members
  ///////////////////////////////////////////////////////////////////
  CpuStateMsr::CpuStateMsr(unsigned short cpu_id, enum mperf_id id)
  {
    clean();

    //Set name and alias
    std::string cpuIdStr;
    cpuIdStr = Tools::CStr(cpu_id);
    _name = "CPU" + cpuIdStr + "_CSTATEMSR";

    _id = id;
    if (_id == C0)
      _alias = "CPU" + cpuIdStr + "_C0_MSR";
    else
      _alias = "CPU" + cpuIdStr + "_Cx_MSR";
    _cpuId = cpu_id;
    _type = Float;

    mperf_register();
    if (!SystemInfo::isRootUser())
      {
        DebugLog::writeMsg(DebugLog::ERROR, "CpuStateMsr::CpuStateMsr()",
            "This sensor needs root access");
        _isActive = false;
        return;
      }

    _isActive = true;
    mperf_start();
  }

  CpuStateMsr::~CpuStateMsr()
  {
    clean();
  }

  inline const char*
  CpuStateMsr::getClassName()
  {
    return ClassName;
  }

  void
  CpuStateMsr::update()
  {
    mperf_stop();
    mperf_get_count_percent(&_cValue.Float);

    aperf_previous_count = aperf_current_count;
    mperf_previous_count = mperf_current_count;
    tsc_at_measure_start = tsc_at_measure_end;
    time_start = time_end;
  }

  sensor_t
  CpuStateMsr::getValue()
  {
    return _cValue;
  }

  ///////////////////////////////////////////////////////////////////
  // Protected Members
  ///////////////////////////////////////////////////////////////////

#define MSR_APERF       0xE8
#define MSR_MPERF       0xE7
#define MSR_TSC 0x10
#define MSR_AMD_HWCR 0xc0010015

  /*
   * Mperf register is defined to tick at P0 (maximum) frequency
   *
   * Instead of reading out P0 which can be tricky to read out from HW,
   * we use TSC counter if it reliably ticks at P0/mperf frequency.
   *
   * Still try to fall back to:
   * /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq
   * on older Intel HW without invariant TSC feature.
   * Or on AMD machines where TSC does not tick at P0 (do not exist yet, but
   * it's still double checked (MSR_AMD_HWCR)).
   *
   * On these machines the user would still get useful mperf
   * stats when acpi-cpufreq driver is loaded.
   */
  int
  CpuStateMsr::init_maxfreq_mode(void)
  {
    int ret;
    unsigned long long hwcr;
    unsigned long min;
    CpuInfo *cpuInfo;
    cpuInfo = SystemInfo::getCpuInfo();
    Processor *proc;
    proc = cpuInfo->getProcessor(_cpuId);

    if (!proc->caps & CPUPOWER_CAP_INV_TSC)
      goto use_sysfs;

    if (proc->vendor == X86_VENDOR_AMD)
      {
        /* MSR_AMD_HWCR tells us whether TSC runs at P0/mperf
         * freq.
         * A test whether hwcr is accessable/available would be:
         * (cpupower_cpu_info.family > 0x10 ||
         *   cpupower_cpu_info.family == 0x10 &&
         *   cpupower_cpu_info.model >= 0x2))
         * This should be the case for all aperf/mperf
         * capable AMD machines and is therefore safe to test here.
         * Compare with Linus kernel git commit: acf01734b1747b1ec4
         */
        ret = read_msr(0, MSR_AMD_HWCR, &hwcr);
        /*
         * If the MSR read failed, assume a Xen system that did
         * not explicitly provide access to it and assume TSC works
         */
        if (ret != 0)
          {
            //dprint("TSC read 0x%x failed - assume TSC working\n", MSR_AMD_HWCR);
            return 0;
          }
        else if (1 & (hwcr >> 24))
          {
            max_freq_mode = MAX_FREQ_TSC_REF;
            return 0;
          }
        else
          { /* Use sysfs max frequency if available */
          }
      }
    else if (proc->vendor == X86_VENDOR_INTEL)
      {
        /*
         * On Intel we assume mperf (in C0) is ticking at same
         * rate than TSC
         */
        max_freq_mode = MAX_FREQ_TSC_REF;
        return 0;
      }
    use_sysfs: if (cpufreq_get_hardware_limits(0, &min, &max_frequency))
      {
        DebugLog::writeMsg(DebugLog::ERROR, "CpuStateMsr::init_maxfreq_mode()",
            "Cannot retrieve max freq from cpufreq kernel subsystem");
        return -1;
      }
    max_freq_mode = MAX_FREQ_SYSFS;
    return 0;
  }

  void
  CpuStateMsr::mperf_register(void)
  {
    if (init_maxfreq_mode())
      return;
  }

  /*
   * read_msr
   *
   * Will return 0 on success and -1 on failure.
   * Possible errno values could be:
   * EFAULT -If the read/write did not fully complete
   * EIO    -If the CPU does not support MSRs
   * ENXIO  -If the CPU does not exist
   */
  int
  CpuStateMsr::read_msr(int cpu, unsigned int idx, unsigned long long *val)
  {
    int fd;
    char msr_file_name[64];

    sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
    fd = open(msr_file_name, O_RDONLY);
    if (fd < 0)
      return -1;
    if (lseek(fd, idx, SEEK_CUR) == -1)
      goto err;
    if (read(fd, val, sizeof *val) != sizeof *val)
      goto err;
    close(fd);
    return 0;
    err: close(fd);
    return -1;
  }

  int
  CpuStateMsr::mperf_get_tsc(unsigned long long *tsc)
  {
    int ret;
    ret = read_msr(0, MSR_TSC, tsc);
    if (ret)
      DebugLog::writeMsg(DebugLog::ERROR, "CpuStateMsr::mperf_get_tsc()",
          "Reading TSC MSR failed.");
    return ret;
  }

  int
  CpuStateMsr::mperf_init_stats(unsigned int cpu)
  {
    unsigned long long val;
    int ret;

    ret = read_msr(cpu, MSR_APERF, &val);
    aperf_previous_count = val;
    ret |= read_msr(cpu, MSR_MPERF, &val);
    mperf_previous_count = val;
    is_valid = !ret;

    return 0;
  }

  int
  CpuStateMsr::mperf_start(void)
  {
    clock_gettime(CLOCK_REALTIME, &time_start);
    mperf_get_tsc(&tsc_at_measure_start);

    mperf_init_stats(_cpuId);

    return 0;
  }

  int
  CpuStateMsr::mperf_measure_stats(unsigned int cpu)
  {
    unsigned long long val;
    int ret;

    ret = read_msr(cpu, MSR_APERF, &val);
    aperf_current_count = val;
    ret |= read_msr(cpu, MSR_MPERF, &val);
    mperf_current_count = val;
    is_valid = !ret;

    return 0;
  }

  long long
  CpuStateMsr::timespec_diff_us(struct timespec start, struct timespec end)
  {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
      {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
      }
    else
      {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
      }
    return (temp.tv_sec * 1000000) + (temp.tv_nsec / 1000);
  }

  int
  CpuStateMsr::mperf_get_count_percent(float *percent)
  {
    unsigned long long aperf_diff, mperf_diff, tsc_diff;
    unsigned long long timediff;

    if (!is_valid)
      return -1;

    if (_id != C0 && _id != Cx)
      return -1;

    mperf_diff = mperf_current_count - mperf_previous_count;
    aperf_diff = aperf_current_count - aperf_previous_count;

    if (max_freq_mode == MAX_FREQ_TSC_REF)
      {
        tsc_diff = tsc_at_measure_end - tsc_at_measure_start;
        *percent = 1.0 * mperf_diff / tsc_diff;
      }
    else if (max_freq_mode == MAX_FREQ_SYSFS)
      {
        timediff = timespec_diff_us(time_start, time_end);
        *percent = 1.0 * mperf_diff / timediff;
      }
    else
      return -1;

    if (_id == Cx)
      *percent = 1.0 - *percent;

    return 0;
  }

  int
  CpuStateMsr::mperf_stop(void)
  {
    mperf_measure_stats(_cpuId);

    mperf_get_tsc(&tsc_at_measure_end);
    clock_gettime(CLOCK_REALTIME, &time_end);

    return 0;
  }

  void
  CpuStateMsr::mperf_unregister(void)
  {
  }

}
