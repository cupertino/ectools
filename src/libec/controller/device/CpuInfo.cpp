/*
 * CPU.cpp
 *
 *  Created on: Jul 4, 2012
 *      Author: Leandro
 */

#include <cstdlib>
#include <libec/tools/Tools.h>
#include <libec/device/CpuInfo.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <fstream>

namespace cea
{
  // Static members
  uint CpuInfo::_cpus;
  uint CpuInfo::_dies;
  uint CpuInfo::_cores_per_die;
  uint CpuInfo::_cores_total;
  std::vector<Processor*> CpuInfo::_processors;
//  std::map<std::string, unsigned int> CpuInfo::_map;

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

    _cpus = sysconf(_SC_NPROCESSORS_ONLN);

    std::ifstream file;
    std::string line;
    Processor* proc;

    file.open("/proc/cpuinfo");
    if (file.is_open())
      {
        // Total number of cores
        line = findParam("cpu cores", &file);
        if (!line.find("error"))
          return;
        _cores_total = str2uint(line);

        file.close();
      }

    std::stringstream aux;
    aux
        << Tools::exec(
            "grep /proc/cpuinfo -e 'physical id' | sort | uniq | wc -l");
    aux >> _dies;

    aux.clear();
    aux << Tools::exec("grep /proc/cpuinfo -e 'core id' | sort | uniq | wc -l");

    aux >> _cores_per_die;

    file.open("/proc/cpuinfo");
      {
        while (file.good())
          {
            proc = new Processor();

            // Process id
            line = findParam("processor", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->id = str2uint(line);

            // Stepping
            line = findParam("stepping", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->stepping = str2uint(line);

            // Frequency
            line = findParam("cpu MHz", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->freq_MHz = str2float(line);

            // Cache size (KB)
            line = findParam("cache size", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->cache_size_KB = str2uint(line);

            // Physical id
            line = findParam("physical id", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->physical_id = str2uint(line);

            // Core id
            line = findParam("core id", &file);
            if (!line.find("error"))
              {
                delete proc;
                continue;
              }
            proc->core_id = str2uint(line);

            _processors.push_back(proc);
          }
        file.close();
        buildMap();
      }
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
    return _dies;
  }

  int
  CpuInfo::getCpuCores()
  {
    return _cores_total;
  }

} /* namespace cea */
