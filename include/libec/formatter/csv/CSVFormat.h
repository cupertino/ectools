///////////////////////////////////////////////////////////////////////////////
/// @file		CSVFormat.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		CSV format option
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_CSV_FORMAT_H__
#define LIBEC_CSV_FORMAT_H__

#include <string>
#include "../base/BaseFormat.h"

namespace cea
{

  /* Prototype */
  class CSVFormatter;

  /// @brief CSV format option
  class CSVFormat : public BaseFormat
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    CSVFormat();
    /// @brief Create a CSVFormat for CSVFormatter setting
    /// @param separator Separator used for separated value by default=','
    CSVFormat(const std::string& separator);
    /// @brief Copy constructor
    /// @param fmtOpt CSVFormat to copy
    CSVFormat(const CSVFormat& fmtOpt);

    /// @brief Load CSVFormatter option from XML
    ///
    /// Load settings by balise attribute.
    ///
    /// <h3>List of xml attribute loaded</h3>
    /// <ul>
    ///     <li><b>separator</b> == Separator used for CSV by default ","
    ///     </li>
    /// </ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <!-- This example set separator = "(#)" for CSV format -->
    ///     <log-format separator="(#)" />
    ///
    ///     <!-- This example set separator = "," for CSV format -->
    ///     <log-format />
    /// @endcode
    ///
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Create a new CSVFormatter
    /// @return Formatter created or 0 if no enough memory
    BaseFormatter*
    createFormatter() const;

    /* Friend class */
    friend class CSVFormatter;

  public:

    /* Member */
    std::string separator; ///< Separator for value by default=','

  };

  /// @brief This typedef represent the old name of the class
  ///         created due to the renaming of the class
  /// @ingroup format
  typedef CSVFormat CVSFormat;

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::CSVFormat
///	@ingroup format
///
/// This class represent the setting of CSVFormatter
///
///////////////////////////////////////////////////////////////////////////////
