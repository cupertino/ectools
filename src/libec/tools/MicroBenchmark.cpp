#include <math.h>
#include <stdlib.h>

#include <libec/tools/MicroBenchmark.h>

namespace cea
{
  void
  MicroBenchmark::stressCpu(time_t duration)
  {
    time_t start, now;
    start = time(NULL);

    while (1)
      {
        sqrt(rand());

        if (duration > 0)
          {
            now = time(NULL);
            if (now - start > duration)
              break;
          }
      }
  }
}
