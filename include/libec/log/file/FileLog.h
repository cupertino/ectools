///////////////////////////////////////////////////////////////////////////////
/// @file		FileLog.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		File log type option
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_FILE_LOG_TYPE_H__
#define LIBEC_FILE_LOG_TYPE_H__

#include <string>
#include "../base/BaseLog.h"

namespace cea
{

  /* Prototype */
  class FileLogger;

  /// @brief File log type option
  class FileLog : public BaseLog
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    FileLog();
    /// @brief Create a FileLog for FileLogger setting
    /// @param outFile Ouptut file
    FileLog(const std::string& outFile);
    /// @brief Create a FileLog for FileLogger setting
    /// @param outFile Ouptut file
    /// @param clearFile If (false) Then append log entries to outFile. \n
    ///                  Else Clear content of outFile
    FileLog(const std::string& outFile, bool clearFile);
    /// @brief Copy constructor
    /// @param logOpt FileLog to copy
    FileLog(const FileLog& logOpt);

    /// @brief Create a new FileLogger
    /// @return FileLogger created or 0 if no enough memory
    BaseLogger*
    createLogger() const;

    void clear();

    /// @brief Load FileLogger option from XML
    ///
    /// Load settings by balise attribute.
    ///
    /// <h3>List of xml attribute loaded</h3>
    /// <ul>
    ///     <li><b>file</b> == Output file. By default = "out.log"</li>
    ///     <li><b>clear</b> == {"true" | "1"; "false" | "0"} --
    ///         Clear content of output file. By default = "false"</li>
    /// </ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <!-- This example set a File log type
    ///          and append log entries to "data.log" -->
    ///     <log-type file="data.log" />
    ///
    ///     <!-- This example set a File log type, clear "newLog.log"
    ///         and write log entries to "data.log" -->
    ///     <log-type file="newLog.log" clear="true" />
    /// @endcode
    ///
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Operator assign '='
    /// @param logOpt FileLog to copy
    FileLog&
    operator=(const FileLog& logOpt);

    /* Friend class */
    friend class FileLogger;

  public:

    /* Member */
    std::string outputFile; ///< Output file where log entries will be save

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::FileLog
///	@ingroup log
///
/// This class represent the setting of FileLogger
///
///////////////////////////////////////////////////////////////////////////////
