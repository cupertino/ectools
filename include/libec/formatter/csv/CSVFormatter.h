///////////////////////////////////////////////////////////////////////////////
/// @file		CSVFormatter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Format data into CSV format 
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_CSV_FORMATTER_H__
#define LIBEC_CSV_FORMATTER_H__

#include <iostream>
#include <string>

#include "../base/BaseFormatter.h"
#include "CSVFormat.h"

namespace cea
{

  /// @brief Format data into CSV format
  class CSVFormatter : public BaseFormatter
  {
  public:

    /* Constructor */
    /// @brief Default constructor
    ///
    /// Set default generic name for block type.
    /// <ul>
    ///     <li><b>ITEM_ADDED</b> == "+"</li>
    ///     <li><b>ITEM_ERASED</b> == "-"</li>
    ///     <li><b>ITEM_UPDATED</b> == "."</li>
    /// </ul>
    ///
    CSVFormatter();
    /// @brief Create an specific CSV Formatter
    ///
    /// Set default generic name for block type.
    /// <ul>
    ///     <li><b>ITEM_ADDED</b> == "+"</li>
    ///     <li><b>ITEM_ERASED</b> == "-"</li>
    ///     <li><b>ITEM_UPDATED</b> == "."</li>
    /// </ul>
    ///
    /// @param fmtOpt CSV settings
    CSVFormatter(const CSVFormat& fmtOpt);

    /* Property */
    /// @brief Set CSV option
    ///
    /// Use a CSVFormat to set CSV option.
    ///
    /// @param fmtOpt CSVFormat class used for setting.
    void
    setOption(const BaseFormat& fmtOpt);

    /// @brief Get CSV option
    /// @return CSVFormat class containing settings
    const CSVFormat&
    getOption() const;

    /* Block open/close */
    /// @brief Create a new CSV line with name as the first value
    ///
    /// When a block is open you need to close it by calling
    /// closeBlock.
    ///
    /// @param name Name of the first value of the line
    virtual void
    openBlock(const std::string& name = "");

    /// @brief Add a new line char
    /// @param name Ignored in CSV format
    virtual void
    closeBlock(const std::string& name = "");

    /* Format data */
    /// @brief Format a data into CSV value.
    /// @param data Data to format
    virtual void
    format(const Value& data);

    /// @brief Format a data into CSV value.
    /// @param propertyName Ignored in CSV format.
    /// @param data Data to format
    virtual void
    format(const std::string& propertyName, const Value& data);

    /* Format a Comment */

    /// @brief Format a comment in CSV
    ///
    /// As CSV format have no grammar rule for comment,
    /// the implementation chosen is to add a line with
    /// protected string value. \n
    /// Format of the comment is "#comment" (the char " is important).\n
    /// \n
    /// This implementation fit perfectly with Spreadsheet Software.
    ///
    /// @param comment Comment to format
    virtual void
    formatComment(const std::string& comment);

    /* Format entries manipulation */
    /// @brief Clear the formatted data
    virtual void
    clear();

    /// @brief Peek all formatted values
    virtual void
    peek(std::string& resultFormattedString);

    /// @brief Format a value in CSV: protect the data if needed
    /// @param data Data to format and out string for result
    void
    strToCSV(std::string& data);

  protected:

    /* Members */
    CSVFormat _fmtOpt; ///< Option of the formatter
    std::string _formattedData; ///< Formatted values

  };

  /// @brief This typedef represent the old name of the class
  ///         created due to the renaming of the class
  /// @ingroup format
  typedef CSVFormatter CVSFormatter;

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::CSVFormatter
///	@ingroup format
///
/// This class format data into CSV format.
///
///////////////////////////////////////////////////////////////////////////////
