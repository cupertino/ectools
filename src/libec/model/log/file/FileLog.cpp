#include <libec/log/file/FileLog.h>
#include <libec/log/file/FileLogger.h>
#include <libec/tools/Tools.h>
#include <fstream>

namespace cea
{

  /** +Constructor */
  FileLog::FileLog() :
      BaseLog(FILE)
  {
    ;
  }
  FileLog::FileLog(const std::string& outFile) :
      BaseLog(FILE), outputFile(outFile)
  {
    ;
  }
  FileLog::FileLog(const std::string& outFile, bool clearFile) :
      BaseLog(FILE), outputFile(outFile)
  {
    if (clearFile)
      {
        this->clear();
      }
  }
  FileLog::FileLog(const FileLog& logOpt) :
      BaseLog(FILE), outputFile(logOpt.outputFile)
  {
    ;
  }

  void
  FileLog::clear()
  {
    std::ofstream f;
    /* Open the file */
    f.open(outputFile.c_str(), std::ios::out);
    /* Close the file */
    f.close();
  }

  /** +loadXML */
  void
  FileLog::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    /* Set OutputFile */
    outputFile.assign(balise.getAttr("file", "out.log"));
    /* Check if clear needed */
    std::string strClearFile = balise.getAttr("clear", "false");
    if (((Tools::identicalString(strClearFile, "true"))
        || (Tools::identicalString(strClearFile, "1"))))
      {
        this->clear();
      }
  }

  /** Assign operator */
  FileLog&
  FileLog::operator=(const FileLog& logOpt)
  {
    if (&logOpt != this)
      {
        outputFile.assign(logOpt.outputFile);
      }
    return *this;
  }

  /** Create the Logger */
  BaseLogger*
  FileLog::createLogger() const
  {
    return new FileLogger();
  }

}
