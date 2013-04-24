///////////////////////////////////////////////////////////////////////////////
/// @file		FileLogger.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		File logger type
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_FILE_LOGGER_H__
#define LIBEC_FILE_LOGGER_H__

#include <iostream>
#include <string>

#include "../base/BaseLogger.h"
#include "FileLog.h"

namespace cea
{

  /// @brief File logger type
  class FileLogger : public BaseLogger
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    FileLogger();
    /// @brief Create a file type log
    /// @param logOpt File settings
    FileLogger(const FileLog& logOpt);

    /* Option Property */
    /// @brief Set file log option
    ///
    /// Use a FileLog to set logger option.
    ///
    /// @param logOpt FileLog class used for setting.
    virtual void
    setOption(const BaseLog& logOpt);
    /// @brief Get file log option
    /// @return FileLog class containing settings
    virtual const FileLog&
    getOption() const;

    /// @brief Flush all pending data to output file
    /// @param data Data to flush
    /// @return True if succeed
    virtual bool
    flush(std::string& data);

  protected:

    /* Member */
    FileLog _logOpt; ///< Option of the logger

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::FileLogger
///	@ingroup log
///
/// This class log data into file.
/// 
/// @see cea::FileLog
///////////////////////////////////////////////////////////////////////////////
