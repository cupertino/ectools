#include <sys/time.h>

#include <libec/tools/Tools.h>
#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  // Static member
  struct timeval Debug::_startTime;

  void
  Debug::StartClock()
  {
    gettimeofday(&_startTime, NULL);
  }

  long
  Debug::ElapsedTimeMsec()
  {
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    return Tools::timevaldiff(_startTime, timeNow);
  }

  long
  Debug::ElapsedTimeUsec()
  {
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    return Tools::usecdiff(&_startTime, &timeNow);
  }

}
