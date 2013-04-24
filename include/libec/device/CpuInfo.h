/*
 * CPU.h
 *
 *  Created on: Jul 4, 2012
 *      Author: Leandro
 */

#ifndef CPU_H_
#define CPU_H_

#include <vector>
#include <map>
#include "../Globals.h"

namespace cea
{

  struct Processor
  {
    unsigned long int id, // ID
        stepping, //
        cache_size_KB, // cache size in KB
        physical_id, // die id
        core_id;
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
    static int*
    getProcessorId(int die = -1, int core = -1, int smt = -1);

    int
    getCpuCount();

    int
    getCpuDies();

    int
    getCpuCores();

  private:
    static void
    buildMap();

    /// Number of CPUs present on the machine
    static unsigned int _cpus;

    /// Number of dies (processors) present on the machine
    static unsigned int _dies;

    /// Number of cores per die (processors)
    static unsigned int _cores_per_die;

    /// Total number of cores
    static unsigned int _cores_total;

//    static std::map<std::string, unsigned int> _map;

//    static std::vector<std::vector<std::vector<unsigned int>>>_vec;

    static std::vector<Processor*> _processors;
  };

} /* namespace cea */
#endif /* CPU_H_ */
