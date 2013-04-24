#include <libec/formatter/json/JsonFormat.h>
#include <libec/formatter/json/JsonFormatter.h>

namespace cea
{

  /** +Constructor */
  JsonFormat::JsonFormat() :
      BaseFormat(Json), separator(",")
  {
    ;
  }
  JsonFormat::JsonFormat(const std::string& separator) :
      BaseFormat(Json), separator(separator)
  {
    ;
  }
  JsonFormat::JsonFormat(const JsonFormat& fmtOpt) :
      BaseFormat(Json), separator(fmtOpt.separator)
  {
    ;
  }

  /** +loadXML */
  void
  JsonFormat::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    separator = balise.getAttr("separator", ",");
  }

  /** Create the Formatter */
  BaseFormatter*
  JsonFormat::createFormatter() const
  {
    return new JsonFormatter();
  }

}
