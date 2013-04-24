//=============================================================================
/// @file		BaseFormat.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class for Format
//=============================================================================

#ifndef LIBEC_BASEFORMAT_H__
#define LIBEC_BASEFORMAT_H__

#include "../FormatType.h"
#include "../../tools/XMLReader.h"

namespace cea
{

  /* Prototype */
  class BaseFormatter;

  /// @brief Abstract class for Format
  class BaseFormat
  {
  public:

    /* Function */

    /// @brief Create the base format and set the type of format
    /// @param type Type of format
    /// @see cea::FormatType
    BaseFormat(FormatType type);

    /// @brief Get the format type
    /// @return the format type
    FormatType
    getType() const;

    /// @brief Load Format option from XML
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Create the Formatter for this format
    /// @return Formatter created or 0 if no enough memory
    virtual BaseFormatter*
    createFormatter() const = 0;

  protected:

    /* Member */
    FormatType _type; ///< Type of the format (ex: CSV, XML, etc ..)

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::BaseFormat
///	@ingroup format
///
/// To create a new format you need to public extend this class.
/// This class represent the option for the formatter.
///
///////////////////////////////////////////////////////////////////////////////
