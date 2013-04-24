#include <libec/log/base/BaseLog.h>
#include <libec/log/base/BaseLogger.h>

namespace cea
{
  /** +Constructor */
  BaseLog::BaseLog(LogType logType) :
      _type(logType)
  {
    ;
  }

  /** +getType */
  LogType
  BaseLog::getType() const
  {
    return _type;
  }

  /** +loadXML */
  void
  BaseLog::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    /* DO NOTHING - By Default */
  }

}
