//=============================================================================
/// @file		BaseLog.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class for Log type
//=============================================================================

#ifndef LIBEC_BASELOG_H__
#define LIBEC_BASELOG_H__

#include "../LogType.h"
#include "../../tools/XMLReader.h"

namespace cea
{

  /* Prototype */
  class BaseLogger;

  /// @brief Abstract class for Log type
  class BaseLog
  {
  public:

    /* Function */
    /// @brief Create the base log and set the type of log
    /// @param type Type of log
    /// @see cea::LogType
    BaseLog(LogType type);

    /// @brief Get the log type
    /// @return the log type
    LogType
    getType() const;

    /// @brief Load Logger option from XML
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Create the Logger for this type
    /// @return Logger created or 0 if no enough memory
    virtual BaseLogger*
    createLogger() const = 0;

  protected:

    /* Member */
    LogType _type; ///< Type of the logger (ex: File, etc ..)

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::BaseLog
///	@ingroup log
///
/// To create a new log type you need to public extend this class.
/// This class represent the option for the logger.
///
///////////////////////////////////////////////////////////////////////////////
