#include <libec/log/file/FileLogger.h>

namespace cea
{

  /** +Constructor */
  FileLogger::FileLogger() :
      BaseLogger(FILE)
  {
    ;
  }
  FileLogger::FileLogger(const FileLog& logOpt) :
      BaseLogger(FILE), _logOpt(logOpt)
  {
    ;
  }

  /** +getOption */
  const FileLog&
  FileLogger::getOption() const
  {
    return _logOpt;
  }
  /** +setOption */
  void
  FileLogger::setOption(const BaseLog& logOpt)
  {
    if (logOpt.getType() == FILE)
      {
        _logOpt = static_cast<const FileLog&>(logOpt);
      }
  }

  /** +flush */
  bool
  FileLogger::flush(std::string& data)
  {
    bool dataFlushed = false;

    std::ofstream f;
    /* Open the file */
    f.open(_logOpt.outputFile.c_str(), std::ios::out | std::ios::app);
    /* Flush data */
    if ((f.is_open()) && (f.good()))
      {
        /* Write data */
        f << data;
        /* Data flushed */
        dataFlushed = true;
      }
    /* Close the file */
    f.close();

    /* Return true if data Flushed correctly */
    return dataFlushed;
  }

}
