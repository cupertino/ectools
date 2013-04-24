#include <libec/formatter/csv/CSVFormat.h>
#include <libec/formatter/csv/CSVFormatter.h>

namespace cea
{

  /** +Constructor */
  CSVFormat::CSVFormat() :
      BaseFormat(CSV), separator(",")
  {
    ;
  }
  CSVFormat::CSVFormat(const std::string& separator) :
      BaseFormat(CSV), separator(separator)
  {
    ;
  }
  CSVFormat::CSVFormat(const CSVFormat& fmtOpt) :
      BaseFormat(CSV), separator(fmtOpt.separator)
  {
    ;
  }

  /** +loadXML */
  void
  CSVFormat::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    separator = balise.getAttr("separator", ",");
  }

  /** Create the Formatter */
  BaseFormatter*
  CSVFormat::createFormatter() const
  {
    return new CSVFormatter();
  }

}
