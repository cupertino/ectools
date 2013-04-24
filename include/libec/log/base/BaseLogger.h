///////////////////////////////////////////////////////////////////////////////
/// @file		BaseLogger.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class for Logger
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_BASELOGGER_H__
#define LIBEC_BASELOGGER_H__

#include <iostream>
#include <string>

#include "BaseLog.h"

namespace cea
{

  /// @brief Abstract class for Logger
  class BaseLogger
  {
  public:

    /* Constructor */

    /// @brief Create the base logger and set the type of logger
    /// @param logType Type of logger
    /// @see cea::LogType
    BaseLogger(LogType logType);

    /* Function */

    /// @brief Get the logger type
    /// @return the log type
    LogType
    getType() const;

    /** Option Property */
    /// @brief Get the option of the logger
    ///
    /// This function need to be overridden.
    ///
    /// @return Corresponding BaseLog class of the Logger
    /// @see setOption
    virtual const BaseLog&
    getOption() const = 0;

    /// @brief Set option of the logger
    ///
    /// Logger have setting manage by extend BaseLog class. \n
    /// For each logger a BaseLog class need to be create. \n
    /// This function need to be overridden.
    ///
    /// @param logOpt Corresponding BaseLog class of the Logger. \n
    virtual void
    setOption(const BaseLog& logOpt) = 0;

    /// @brief Flush all pending data
    /// @param data Data to flush
    /// @return True if succeed
    virtual bool
    flush(std::string& data) = 0;

  protected:

    /* Members */
    LogType _type; ///< Type of the log

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::BaseLogger
///	@ingroup log
///
/// To create a new logger you need to public extend this class.
/// This class represent the logger itself and use the corresponding extend
/// of BaseLog as option.
///
///////////////////////////////////////////////////////////////////////////////
