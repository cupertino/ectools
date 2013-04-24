#include <libec/formatter/base/BaseFormat.h>
#include <libec/formatter/base/BaseFormatter.h>

namespace cea
{
  /** +Constructor */
  BaseFormat::BaseFormat(FormatType fmtType) :
      _type(fmtType)
  {
    ;
  }

  /** +getFormat */
  FormatType
  BaseFormat::getType() const
  {
    return _type;
  }

  /** +loadXML */
  void
  BaseFormat::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    /* DO NOTHING - By Default */
  }
}
