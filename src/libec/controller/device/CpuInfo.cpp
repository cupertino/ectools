/*
 * CPU.cpp
 *
 *  Created on: Jul 4, 2012
 *  (C) 2004-2009  Dominik Brodowski <linux@dominikbrodowski.de>
 *  (C) 2011       Thomas Renninger <trenn@novell.com> Novell Inc.
 *  (C) 2012       Leandro Fontoura Cupertino <fontoura@irit.fr>
 */

#include <libec/tools/Tools.h>
#include <libec/tools/FileTools.h>
#include <libec/tools/DebugLog.h>
#include <libec/device/CpuInfo.h>

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

namespace cea
{
  // Static members
//  uint CpuInfo::_cpus;
//  uint CpuInfo::_pkgs;
//  uint CpuInfo::_cores;
//  uint CpuInfo::_cores_per_die;
//  std::vector<Processor*> CpuInfo::_processors;

  unsigned int
  CpuInfo::cpuid_reg(enum reg_id reg, unsigned int op)
  {
    unsigned int _reg[reg_max];
    __cpuid(op, _reg[eax], _reg[ebx], _reg[ecx], _reg[edx]);
    return _reg[reg];
  }

  /* get_cpu_info
   *
   * Extract CPU vendor, family, model, stepping info from /proc/cpuinfo
   *
   * Returns 0 on success or a negativ error code
   *
   * TBD: Should there be a cpuid alternative for this if /proc is not mounted?
   */
  int
  CpuInfo::get_cpu_info(unsigned int cpu, Processor *processor)
  {
    FILE *fp;
    char value[64];
    unsigned int proc, x;
    unsigned int unknown = 0xffffff;
    unsigned int cpuid_level, ext_cpuid_level;

    int ret = -EINVAL;

    processor->vendor = X86_VENDOR_UNKNOWN;
    processor->family = unknown;
    processor->model = unknown;
    processor->stepping = unknown;
    processor->caps = 0;

    fp = fopen("/proc/cpuinfo", "r");
    if (!fp)
      return -EIO;

    while (!feof(fp))
      {
        if (!fgets(value, 64, fp))
          continue;
        value[63 - 1] = '\0';

        if (!strncmp(value, "processor\t: ", 12))
          sscanf(value, "processor\t: %u", &proc);

        if (proc != cpu)
          continue;

        /* Get CPU vendor */
        if (!strncmp(value, "vendor_id", 9))
          {
            for (x = 1; x < X86_VENDOR_MAX; x++)
              {
                if (strstr(value, cpu_vendor_table[x]))
                  processor->vendor = (cpu_vendor) x;
              }
            /* Get CPU family, etc. */
          }
        else if (!strncmp(value, "cpu family\t: ", 13))
          {
            sscanf(value, "cpu family\t: %u", &processor->family);
          }
        else if (!strncmp(value, "model\t\t: ", 9))
          {
            sscanf(value, "model\t\t: %u", &processor->model);
          }
        else if (!strncmp(value, "stepping\t: ", 10))
          {
            sscanf(value, "stepping\t: %u", &processor->stepping);

            /* Exit -> all values must have been set */
            if (processor->vendor == X86_VENDOR_UNKNOWN
                || processor->family == unknown || processor->model == unknown
                || processor->stepping == unknown)
              {
                ret = -EINVAL;
                goto out;
              }

            ret = 0;
            goto out;
          }
      }
    ret = -ENODEV;
    out: fclose(fp);
    /* Get some useful CPU capabilities from cpuid */
    if (processor->vendor != X86_VENDOR_AMD
        && processor->vendor != X86_VENDOR_INTEL)
      return ret;

    cpuid_level = cpuid_reg(eax, 0);
    ext_cpuid_level = cpuid_reg(eax, 0x80000000);

    /* Invariant TSC */
    if (ext_cpuid_level >= 0x80000007
        && (cpuid_reg(edx, 0x80000007) & (1 << 8)))
      processor->caps |= CPUPOWER_CAP_INV_TSC;

    /* Aperf/Mperf registers support */
    if (cpuid_level >= 6 && (cpuid_reg(ecx, 6) & 0x1))
      processor->caps |= CPUPOWER_CAP_APERF;

    /* AMD Boost state enable/disable register */
    if (processor->vendor == X86_VENDOR_AMD)
      {
        if (ext_cpuid_level >= 0x80000007
            && (cpuid_reg(edx, 0x80000007) & (1 << 9)))
          processor->caps |= CPUPOWER_CAP_AMD_CBP;
      }

    if (processor->vendor == X86_VENDOR_INTEL)
      {
        if (cpuid_level >= 6 && (cpuid_reg(eax, 6) & (1 << 1)))
          processor->caps |= CPUPOWER_CAP_INTEL_IDA;
      }

    if (processor->vendor == X86_VENDOR_INTEL)
      {
        /* Intel's perf-bias MSR support */
        if (cpuid_level >= 6 && (cpuid_reg(ecx, 6) & (1 << 3)))
          processor->caps |= CPUPOWER_CAP_PERF_BIAS;

        /* Intel's Turbo Ratio Limit support */
        if (processor->family == 6)
          {
            switch (processor->model)
              {
            case 0x1A: /* Core i7, Xeon 5500 series
             * Bloomfield, Gainstown NHM-EP
             */
            case 0x1E: /* Core i7 and i5 Processor
             * Clarksfield, Lynnfield, Jasper Forest
             */
            case 0x1F: /* Core i7 and i5 Processor - Nehalem */
            case 0x25: /* Westmere Client
             * Clarkdale, Arrandale
             */
            case 0x2C: /* Westmere EP - Gulftown */
              processor->caps |= CPUPOWER_CAP_HAS_TURBO_RATIO;
            case 0x2A: /* SNB */
            case 0x2D: /* SNB Xeon */
            case 0x3A: /* IVB */
            case 0x3E: /* IVB Xeon */
              processor->caps |= CPUPOWER_CAP_HAS_TURBO_RATIO;
              processor->caps |= CPUPOWER_CAP_IS_SNB;
              break;
            case 0x2E: /* Nehalem-EX Xeon - Beckton */
            case 0x2F: /* Westmere-EX Xeon - Eagleton */
            default:
              break;
              }
          }
      }

    /*      printf("ID: %u - Extid: 0x%x - Caps: 0x%llx\n",
     cpuid_level, ext_cpuid_level, cpu_info->caps);
     */
    return ret;
  }

  unsigned long int
  readCacheFile(int proc_id, int cache_id)
  {
    char path[SYSFS_PATH_MAX];
    char magnitude = 'K';
    unsigned long int size = 0;

    snprintf(path, sizeof(path), PATH_TO_CPU "cpu%d/cache/index%d/size",
        proc_id, cache_id);
//    DebugLog::writeMsg(DebugLog::INFO, "CpuInfo::readCacheFile()", path);
    std::ifstream ifs(path);
    if (ifs.good())
      {
        ifs >> size;
        ifs >> magnitude;
        if (magnitude == 'M')
          size *= 1024;
      }
//    DebugLog::writeMsg(DebugLog::INFO, "CpuInfo::getCacheSize()",
//        "Cache %d (Kb): %lu", cache_id, size);
    return size;
  }

  int
  CpuInfo::getCacheSize(Processor *processor)
  {
    processor->cache_size_KB[L1d] = readCacheFile(processor->id, L1d);
    processor->cache_size_KB[L1i] = readCacheFile(processor->id, L1i);
    processor->cache_size_KB[L2] = readCacheFile(processor->id, L2);
    processor->cache_size_KB[L3] = readCacheFile(processor->id, L3);

    return 0;
  }

  std::string
  findParam(std::string str, std::ifstream* file)
  {
    std::string line;
    size_t found;
    size_t pos;

    found = std::string::npos;

    while (file->good() && (found == std::string::npos))
      {
        getline(*file, line);
        found = line.find(str);
      }

    if (!file->good())
      return "error: pattern not found";

    pos = line.find(": ") + 2;

    return line.substr(pos);
  }

  unsigned long int
  str2uint(std::string str)
  {
    unsigned long int res;
    std::stringstream ss;

    ss << str;
    ss >> res;

    return res;
  }

  float
  str2float(std::string str)
  {
    float res;
    std::stringstream ss;

    ss << str;
    ss >> res;

    return res;
  }

  void
  CpuInfo::buildMap()
  {
    uint nProc = _processors.size();

    for (uint d = 0; d < nProc; d++)
      {
      }

//    for (uint i = 0; i < 4; i++)
//      {
//        uint3 a(1, 2, 3);
//
//        _map.insert(std::pair<std::string, uint>(a.ToString(), i));
//        std::cout << _map["1;2;3"] << std::endl;
//      }
  }

  CpuInfo::CpuInfo()
  {
    std::string tmp; // temporary string to get data from console
    std::ifstream file;
    std::string line;
    Processor* proc;

    _cpus = sysconf(_SC_NPROCESSORS_CONF);
    _pkgs = _cores = 0;

    int *isPkgCounted, *isCoreCounted;
    isPkgCounted = new int[_cpus];
    isCoreCounted = new int[_cpus];

    for (unsigned int cpu = 0; cpu < _cpus; cpu++)
      {
        proc = new Processor();

        proc->id = cpu;
        proc->is_online = isCpuOnline(cpu);

        //Package ID
        if (topologyReadFile(cpu, "physical_package_id", &(proc->pkg)) < 0)
          {
            DebugLog::writeMsg(DebugLog::ERROR, "CpuInfo::CpuInfo()",
                "The 'topology/physical_package_id' file could not be read");
            delete proc;
            continue;
          }
        if (!isPkgCounted[proc->pkg])
          {
            _pkgs++;
            isPkgCounted[proc->pkg] = 1;
          }

        // Core ID
        if (topologyReadFile(cpu, "core_id", &(proc->core)) < 0)
          {
            DebugLog::writeMsg(DebugLog::ERROR, "CpuInfo::CpuInfo()",
                "The 'topology/core_id' file could not be read");
            delete proc;
            continue;
          }
        if (!isCoreCounted[proc->core])
          {
            _cores++;
            isCoreCounted[proc->core] = 1;
          }

        get_cpu_info(cpu, proc);
        getCacheSize(proc);

        _processors.push_back(proc);
      }

    buildMap();

    delete (isPkgCounted);
    delete (isCoreCounted);
  }

  CpuInfo::~CpuInfo()
  {
    for (unsigned i = 0; i < _processors.size(); i++)
      delete (_processors[i]);
    _processors.clear();
  }

  int
  CpuInfo::getCpuCount()
  {
    return _cpus;
  }

  int
  CpuInfo::getCpuDies()
  {
    return _pkgs;
  }

  int
  CpuInfo::getCpuCores()
  {
    return _cores;
  }

  int
  CpuInfo::isCpuOnline(unsigned int cpu)
  {
    char path[SYSFS_PATH_MAX];
    int fd;
    ssize_t numread;
    unsigned long long value;
    char linebuf[MAX_LINE_LEN];
    char *endp;
    struct stat statbuf;

    snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u", cpu);

    if (stat(path, &statbuf) != 0)
      return 0;

    /*
     * kernel without CONFIG_HOTPLUG_CPU
     * -> cpuX directory exists, but not cpuX/online file
     */
    snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/online", cpu);
    if (stat(path, &statbuf) != 0)
      return 1;

    fd = open(path, O_RDONLY);
    if (fd == -1)
      return -errno;

    numread = read(fd, linebuf, MAX_LINE_LEN - 1);
    if (numread < 1)
      {
        close(fd);
        return -EIO;
      }
    linebuf[numread] = '\0';
    close(fd);

    value = strtoull(linebuf, &endp, 0);
    if (value > 1 || value < 0)
      return -EINVAL;

    return value;
  }

  int
  CpuInfo::topologyReadFile(unsigned int cpu, const char *fname, int *result)
  {
    char linebuf[MAX_LINE_LEN];
    char *endp;
    char path[SYSFS_PATH_MAX];

    snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/topology/%s", cpu, fname);
    if (FileTools::sysfsReadFile(path, linebuf, MAX_LINE_LEN) == 0)
      return -1;
    *result = strtol(linebuf, &endp, 0);
    if (endp == linebuf || errno == ERANGE)
      return -1;
    return 0;
  }

  Processor*
  CpuInfo::getProcessor(uint id)
  {
    if (id < _cpus)
      {
        return _processors[id];
      }
    else
      return NULL;
  }

}
/* namespace cea */
