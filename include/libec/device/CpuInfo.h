/*
 * CPU.h
 *
 *  Created on: Jul 4, 2012
 *      Author: Leandro
 */

#ifndef LIBEC_CPUINFO_H_
#define LIBEC_CPUINFO_H_

#include <vector>
#include <map>
#include "../Globals.h"
#include <cpuid.h>

#define PATH_TO_CPU "/sys/devices/system/cpu/"
#define MAX_LINE_LEN 255
#define SYSFS_PATH_MAX 255

#define CPUPOWER_CAP_INV_TSC          0x00000001
#define CPUPOWER_CAP_APERF              0x00000002
#define CPUPOWER_CAP_AMD_CBP            0x00000004
#define CPUPOWER_CAP_PERF_BIAS          0x00000008
#define CPUPOWER_CAP_HAS_TURBO_RATIO    0x00000010
#define CPUPOWER_CAP_IS_SNB             0x00000020
#define CPUPOWER_CAP_INTEL_IDA          0x00000040

namespace cea
{
  enum cpu_vendor
  {
    X86_VENDOR_UNKNOWN = 0, X86_VENDOR_INTEL, X86_VENDOR_AMD, X86_VENDOR_MAX
  };

  static const char *cpu_vendor_table[X86_VENDOR_MAX] =
    { "Unknown", "GenuineIntel", "AuthenticAMD" };

  enum cache_id
  {
    L1d = 0, L1i = 1, L2 = 2, L3 = 3, CACHE_MAX
  };

  struct Processor
  {
    unsigned long int is_online; //
    unsigned long int cache_size_KB[CACHE_MAX]; // cache size in KB
    int id, // ID
        pkg, // physical_id
        core; // core_id
    enum cpu_vendor vendor;
    unsigned int family, model, stepping;

    unsigned long long caps; /// CPU capabilities

    float freq_MHz; // frequency in mega Hertz
  };

  class CpuInfo
  {
  public:
    /** Constructor */
    CpuInfo();

    virtual
    ~CpuInfo();

    /**
     * Gets the processor id
     * @param die Processor's physical id
     * @param core Processor's core id
     * @param smt Processsor's simultaneous multithreading id
     * @return Processor ID
     */
    Processor*
    getProcessor(uint id);

    int
    getCpuCount();

    int
    getCpuDies();

    int
    getCpuCores();

  private:
    void
    buildMap();

    enum reg_id
    {
      eax, ebx, ecx, edx, reg_max
    };

    int
    isCpuOnline(unsigned int cpu);

    /* returns -1 on failure, 0 on success */
    int
    topologyReadFile(unsigned int cpu, const char *fname, int *result);

    int
    get_cpu_info(unsigned int cpu, Processor *processor);

    unsigned int
    cpuid_reg(enum reg_id reg, unsigned int op);

    int
    getCacheSize(Processor *processor);

    /// Number of CPUs present on the machine
    unsigned int _cpus;

    /// Number of packages (aka dies or sockets) present on the machine
    unsigned int _pkgs;

    /// Total number of cores
    unsigned int _cores;

    /// Number of cores per die (processors)
    unsigned int _cores_per_die;

//    static std::map<std::string, unsigned int> _map;

//    static std::vector<std::vector<std::vector<unsigned int>>>_vec;

    std::vector<Processor*> _processors;
  };

} /* namespace cea */
#endif /* CPU_H_ */
