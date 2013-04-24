#ifndef LIBEC_DEBUG_H__
#define LIBEC_DEBUG_H__

#include <sys/time.h>

namespace cea
{
  /// \brief Provide debugging methods
  class Debug
  {
  public:
    /// Starts a static debug clock
    static void StartClock();

    /// Elapsed time in milliseconds (ms)
    static long ElapsedTimeMsec();

    /// Elapsed time in microseconds (us)
    static long ElapsedTimeUsec();

  private:
    /// Keeps the start time for debugging time information
    static struct timeval _startTime;
  };
}

#endif
