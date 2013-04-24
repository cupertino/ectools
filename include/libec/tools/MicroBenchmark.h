#ifndef LIBEC_MICRO_BENCHMARK_H__
#define LIBEC_MICRO_BENCHMARK_H__

#include <time.h>

namespace cea
{
  class MicroBenchmark
  {
  public:
    /// Stresses a CPU with a single threaded load
    static void
    stressCpu(time_t duration = -1);
  };
}
#endif
