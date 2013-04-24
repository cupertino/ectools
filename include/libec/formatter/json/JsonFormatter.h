///////////////////////////////////////////////////////////////////////////////
/// @file		JsonFormatter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Format data into Json format
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_Json_FORMATTER_H__
#define LIBEC_Json_FORMATTER_H__

#include <iostream>
#include <string>

#include "../base/BaseFormatter.h"
#include "JsonFormat.h"

namespace cea
{

  /// @brief Format data into Json format
  class JsonFormatter : public BaseFormatter
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
    JsonFormatter();
    /// @brief Create an specific Json Formatter
    ///
    /// Set default generic name for block type.
    /// <ul>
    ///     <li><b>ITEM_ADDED</b> == "+"</li>
    ///     <li><b>ITEM_ERASED</b> == "-"</li>
    ///     <li><b>ITEM_UPDATED</b> == "."</li>
    /// </ul>
    ///
    /// @param fmtOpt Json settings
    JsonFormatter(const JsonFormat& fmtOpt);

    /* Property */
    /// @brief Set Json option
    ///
    /// Use a JsonFormat to set Json option.
    ///
    /// @param fmtOpt JsonFormat class used for setting.
    void
    setOption(const BaseFormat& fmtOpt);

    /// @brief Get Json option
    /// @return JsonFormat class containing settings
    const JsonFormat&
    getOption() const;

    /* Block open/close */
    /// @brief Create a new Json line with name as the first value
    ///
    /// When a block is open you need to close it by calling
    /// closeBlock.
    ///
    /// @param name Name of the first value of the line
    virtual void
    openBlock(const std::string& name = "");

    /// @brief Add a new line char
    /// @param name Ignored in Json format
    virtual void
    closeBlock(const std::string& name = "");

    /* Format data */
    /// @brief Format a data into Json value.
    /// @param data Data to format
    virtual void
    format(const Value& data);

    /// @brief Format a data into Json value.
    /// @param propertyName Ignored in Json format.
    /// @param data Data to format
    virtual void
    format(const std::string& propertyName, const Value& data);

    /* Format a Comment */

    /// @brief Format a comment in Json
    ///
    /// As Json format have no grammar rule for comment,
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

    /// @brief Format a value in Json: protect the data if needed
    /// @param data Data to format and out string for result
    void
    strToJson(std::string& data);

  protected:

    /* Members */
    JsonFormat _fmtOpt; ///< Option of the formatter
    std::string _formattedData; ///< Formatted values
  };
}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::JsonFormatter
///	@ingroup format
///
/// This class format data into Json format.
///
///////////////////////////////////////////////////////////////////////////////
