#include <libec/formatter/FormatType.h>
#include <libec/formatter/base/BaseFormat.h>

#include <libec/formatter/csv/CSVFormat.h>
#include <libec/formatter/xml/XMLFormat.h>

#include <libec/tools/Tools.h>

namespace cea
{

  /** +create */
  BaseFormat*
  FormatCreator::create(const std::string& type)
  {
    if (Tools::identicalString(type, "XML"))
      {
        return new XMLFormat();
      }
    else if ((Tools::identicalString(type, "CSV"))
        || (Tools::identicalString(type, "CVS")))
      {
        return new CSVFormat();

        /** NOTE: Add Format Type here */

      }
    return 0;
  }

}
