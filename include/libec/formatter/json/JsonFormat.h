///////////////////////////////////////////////////////////////////////////////
/// @file		JsonFormat.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Json format option
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_Json_FORMAT_H__
#define LIBEC_Json_FORMAT_H__

#include <string>
#include "../base/BaseFormat.h"

namespace cea
{

  /* Prototype */
  class JsonFormatter;

  /// @brief Json format option
  class JsonFormat : public BaseFormat
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    JsonFormat();
    /// @brief Create a JsonFormat for JsonFormatter setting
    /// @param separator Separator used for separated value by default=','
    JsonFormat(const std::string& separator);
    /// @brief Copy constructor
    /// @param fmtOpt JsonFormat to copy
    JsonFormat(const JsonFormat& fmtOpt);

    /// @brief Load JsonFormatter option from XML
    ///
    /// Load settings by balise attribute.
    ///
    /// <h3>List of xml attribute loaded</h3>
    /// <ul>
    ///     <li><b>separator</b> == Separator used for Json by default ","
    ///     </li>
    /// </ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <!-- This example set separator = "(#)" for Json format -->
    ///     <log-format separator="(#)" />
    ///
    ///     <!-- This example set separator = "," for Json format -->
    ///     <log-format />
    /// @endcode
    ///
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Create a new JsonFormatter
    /// @return Formatter created or 0 if no enough memory
    BaseFormatter*
    createFormatter() const;

    /* Friend class */
    friend class JsonFormatter;

  public:

    /* Member */
    std::string separator; ///< Separator for value by default=','

  };
}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::JsonFormat
///	@ingroup format
///
/// This class represent the setting of JsonFormatter
///
///////////////////////////////////////////////////////////////////////////////
