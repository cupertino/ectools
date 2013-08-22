//============================================================================
// Name        : SensorPerfCount.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.01
// Copyright   : Your copyright notice
// Description : Performance Counter Sensor
//============================================================================

#include <string>
#include <cstring>
#include <assert.h>
#include <sys/types.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

#include <libec/tools/Tools.h>
#include <libec/sensor/Sensor.h>
#include <libec/sensor/SensorPerfCount.h>

#define LEN(a) sizeof(a) / sizeof(a[0]);

namespace cea
{
  long
  perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu,
      int group_fd, unsigned long flags)
  {
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
  }

  uint
  len(int array[])
  {
    return sizeof(array) / sizeof(array[0]);
  }

  static std::string hwName[] =
    { "PERF_COUNT_HW_CPU_CYCLES", "PERF_COUNT_HW_INSTRUCTIONS",
        "PERF_COUNT_HW_CACHE_REFERENCES", "PERF_COUNT_HW_CACHE_MISSES",
        "PERF_COUNT_HW_BRANCH_INSTRUCTIONS", "PERF_COUNT_HW_BRANCH_MISSES",
        "PERF_COUNT_HW_BUS_CYCLES", "PERF_COUNT_HW_STALLED_CYCLES_FRONTEND",
        "PERF_COUNT_HW_STALLED_CYCLES_BACKEND", "PERF_COUNT_HW_REF_CPU_CYCLES" };

  PerfCount::PerfCount(u32 type_id, u64 config, int core_id)
  {
    // Array of performance counter names and alias according to their
    // type and config inputs

    static std::string hwAlias[] =
      { "PC_HCpuCycl", "PC_HCpuInst", "PC_HCacRefs", "PC_HCacMisses",
          "PC_HBraInst", "PC_HBraMiss", "PC_HBusCycl", "PC_HStaCycF",
          "PC_HStaCycB", "PC_RefCpuCycl" };

    static std::string swName[] =
      { "PERF_COUNT_SW_CPU_CLOCK", "PERF_COUNT_SW_TASK_CLOCK",
          "PERF_COUNT_SW_PAGE_FAULTS", "PERF_COUNT_SW_CONTEXT_SWITCHES",
          "PERF_COUNT_SW_CPU_MIGRATIONS", "PERF_COUNT_SW_PAGE_FAULTS_MIN",
          "PERF_COUNT_SW_PAGE_FAULTS_MAJ", "PERF_COUNT_SW_ALIGNMENT_FAULTS",
          "PERF_COUNT_SW_EMULATION_FAULTS" };

    static std::string swAlias[] =
      { "PC_SCpuClk", "PC_STskClk", "PC_SPgFlt", "PC_SCntSw", "PC_SCpuMig",
          "PC_SPgFltMn", "PC_SPgFltMj", "PC_SAligFlt", "PC_SEmFlt" };

    static std::string hwCacheName[] =
      { "PERF_COUNT_HW_CACHE_L1D", "PERF_COUNT_HW_CACHE_L1I",
          "PERF_COUNT_HW_CACHE_LL", "PERF_COUNT_HW_CACHE_DTLB",
          "PERF_COUNT_HW_CACHE_ITLB", "PERF_COUNT_HW_CACHE_BPU",
          "PERF_COUNT_HW_CACHE_NODE" };

    static std::string hwCacheAlias[] =
      { "HCL1D", "HCL1I", "HCLL", "HCDTLB", "HCITLB", "HCBPU", "HCNode" };

    static std::string hwCacheOpName[] =
      { "_READ", "_WRITE", "_PREFETCH" };

    static char hwCacheOpAlias[] =
      { 'R', 'W', 'P' };

    static std::string hwCacheOpResultName[] =
      { "_ACCESS", "_MISS" };

    static char hwCacheOpResultAlias[] =
      { 'A', 'M' };

    _type = U64;
    _cValue.U64 = 0;
    _pValue.U64 = 0;

    std::memset(&_attr, 0, sizeof(_attr));
    _attr.type = type_id;
    _attr.config = config;

    _cpus_total = sysconf(_SC_NPROCESSORS_ONLN);
    _cpu_id = core_id;

    switch (type_id)
      {
    case 0: //PERF_TYPE_HARDWARE
      {
        if (config < (sizeof(hwName) / sizeof(std::string)))
          {
            _name = hwName[config] + "_" + Tools::CStr(_cpu_id);
            _alias = hwAlias[config] + "_" + Tools::CStr(_cpu_id);
          }
        else
          {
            _name = "UNKNOWN";
            _alias = "Unkn";
          }
        break;
      }
    case 1:
      //PERF_TYPE_SOFTWARE
      {
        if (config < (sizeof(swName) / sizeof(std::string)))
          {
            _name = swName[config] + "_" + Tools::CStr(_cpu_id);
            _alias = swAlias[config] + "_" + Tools::CStr(_cpu_id);
          }
        else
          {
            _name = "UNKNOWN";
            _alias = "Unkn";
          }
        break;
      }
    case 2:
      //PERF_TYPE_TRACEPOINT
      {
        break;
      }
    case 3:
      //PERF_TYPE_HW_CACHE
      {
        int perf_hw_cache_id;
        int perf_hw_cache_op_id;
        int perf_hw_cache_op_result_id;

        perf_hw_cache_id = (config) & 0x000000FF;
        perf_hw_cache_op_id = (config >> 8) & 0x000000FF;
        perf_hw_cache_op_result_id = (config >> 16) & 0x000000FF;

        if ((perf_hw_cache_id < 7) && (perf_hw_cache_op_id < 3)
            && (perf_hw_cache_op_result_id < 2))
          {
            _name = hwCacheName[perf_hw_cache_id];
            _alias = hwCacheAlias[perf_hw_cache_id];

            _name += hwCacheOpName[perf_hw_cache_op_id];
            _alias += hwCacheOpAlias[perf_hw_cache_op_id];

            _name += hwCacheOpResultName[perf_hw_cache_op_result_id];
            _alias += hwCacheOpResultAlias[perf_hw_cache_op_result_id];

            _name += "_" + Tools::CStr(_cpu_id);
            _alias += "_" + Tools::CStr(_cpu_id);
          }
        else
          {
            _name = "UNKNOWN";
            _alias = "Unkn";
          }

        break;
      }
    case 4:
      //PERF_TYPE_RAW
      {
        break;
      }
    case 5:
      //PERF_TYPE_BREAKPOINT
      {
        break;
      }
    default:
      {
        _name = "UNKNOWN";
        _alias = "Unkn";
        break;
      }
      }

    // initialize node related file descriptors
    // and check if performance counter is present in the hardware
    _nArr = new hpc_ent[_cpus_total];
    _isActive = true;

    for (int i = 0; i < _cpus_total; i++)
      {
        struct hpc_ent v;
//        v.fd = sys_perf_event_open(&_attr, -1, 0, -1, 0);
        v.fd = perf_event_open(&_attr, -1, 0, -1, 0);
        v.pVal = v.cVal = 0;
        _nArr[i] = v;

        _isActive &= (v.fd >= 0);
      }
  }

  PerfCount::~PerfCount()
  {
    while (_pidMap.begin() != _pidMap.end())
      {
        std::map<pid_t, hpc_ent>::iterator it = _pidMap.begin();
        pid_t pid = it->first;
        remove(pid);
      }

    delete[] _nArr;
  }

  int
  PerfCount::openfd(pid_t pid)
  {
    // Opens the file descriptors
    if ((pid > 0) && (_cpu_id >= -1))
      {
//        int fd = sys_perf_event_open(&_attr, pid, _cpu_id, -1, 0);
        int fd = perf_event_open(&_attr, pid, _cpu_id, -1, 0);
        _isActive = (fd >= 0);
        if (_isActive)
          return (fd);
        else
          return NULL;
      }
    return NULL;
  }

  u64
  PerfCount::readPC(int fd)
  {
    u64 tmp = 0ull;

    if (read(fd, &tmp, sizeof(tmp)) < 0)
      {
        std::cerr << "error: performance counter [" << this->_alias
            << "] could not read.";
        return NULL;
      }
    else
      return tmp;
  }

  void
  PerfCount::updatePid(pid_t pid)
  {
    u64 tmp;

    // the update is only done if the pid has a file descriptor associated to it
    if (pid > 0)
      {
        // is the update time important? than it should be on the hpc entity structure
        _pidMap[pid].pVal = _pidMap[pid].cVal;

        if ((tmp = readPC(_pidMap[pid].fd)) != NULL)
          _pidMap[pid].cVal = tmp;
      }
  }

  void
  PerfCount::update()
  {
    u64 tmp;

    for (int i = 0; i < _cpus_total; i++)
      {
        _nArr[i].pVal = _nArr[i].cVal;

        if ((tmp = readPC(_nArr[i].fd)) != NULL)
          _nArr[i].cVal = tmp;
      }
  }

  void
  PerfCount::add(pid_t pid)
  {
    // if map does not contains key ..
    if (_pidMap.find(pid) == _pidMap.end())
      {
        int fd = openfd(pid);

        // check if the file descriptors were properly initiated
        if (fd > 0)
          {
            hpc_ent val;
            val.fd = fd;

            _pidMap.insert(std::pair<pid_t, hpc_ent>(pid, val));
          }
      }
  }

  void
  PerfCount::remove(pid_t pid)
  {
    close(_pidMap[pid].fd);

    _pidMap.erase(pid);
  }

  sensor_t
  PerfCount::getValue()
  {
    _cValue.U64 = 0;

    for (int i = 0; i < _cpus_total; i++)
      _cValue.U64 += _nArr[i].cVal - _nArr[i].pVal;

    return _cValue;
  }

  sensor_t
  PerfCount::getValuePid(pid_t pid)
  {
    sensor_t ret;

    add(pid);
    ret.U64 = _pidMap[pid].cVal - _pidMap[pid].pVal;

    return ret;
  }
}
