#include <libec/log/base/BaseLogger.h>

namespace cea
{

  /** +Constructor */
  BaseLogger::BaseLogger(LogType logType) :
      _type(logType)
  {
    ;
  }

  /** +getType */
  LogType
  BaseLogger::getType() const
  {
    return _type;
  }

}
