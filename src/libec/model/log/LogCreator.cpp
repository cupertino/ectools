#include <libec/log/LogType.h>
#include <libec/log/base/BaseLog.h>
#include <libec/log/file/FileLog.h>

#include <libec/tools/Tools.h>

namespace cea
{

  /** +create */
  BaseLog*
  LogCreator::create(const std::string& type)
  {
    if (Tools::identicalString(type, "FILE"))
      {
        return new FileLog();

        /** NOTE: Add Log Type here */

      }
    return 0;
  }

}
