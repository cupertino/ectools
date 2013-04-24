///////////////////////////////////////////////////////////////////////////////
/// @file		XMLFormatter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Format data into XML format
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_XML_FORMATTER_H__
#define LIBEC_XML_FORMATTER_H__

#include <iostream>
#include <string>

#include "../base/BaseFormatter.h"
#include "XMLFormat.h"

namespace cea
{

  /// @brief Format data into XML format
  class XMLFormatter : public BaseFormatter
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    ///
    /// Set default generic name for block type.
    /// <ul>
    ///     <li><b>ITEM_ADDED</b> == "add"</li>
    ///     <li><b>ITEM_ERASED</b> == "delete"</li>
    ///     <li><b>ITEM_UPDATED</b> == "update"</li>
    ///     <li><b>ITEM_DEFAULT</b> == "data"</li>
    /// </ul>
    ///
    XMLFormatter();
    /// @brief Create an specific XML formatter
    ///
    /// Set default generic name for block type.
    /// <ul>
    ///     <li><b>ITEM_ADDED</b> == "add"</li>
    ///     <li><b>ITEM_ERASED</b> == "delete"</li>
    ///     <li><b>ITEM_UPDATED</b> == "update"</li>
    ///     <li><b>ITEM_DEFAULT</b> == "data"</li>
    /// </ul>
    ///
    /// @param fmtOpt XML settings
    XMLFormatter(const XMLFormat& fmtOpt);

    /* Property */
    /// @brief Set XML option
    ///
    /// Use a XMLFormat to set XML option.
    ///
    /// @param fmtOpt XMLFormat class used for setting.
    void
    setOption(const BaseFormat& fmtOpt);

    /// @brief Get XML option
    /// @return XMLFormat class containing settings
    const XMLFormat&
    getOption() const;

    /* Block open/close */
    /// @brief Create a new XML balise
    ///
    /// When a block is open you need to close it by calling
    /// closeBlock.
    ///
    /// @param name Name of the balise to create
    virtual void
    openBlock(const std::string& name = "");

    /// @brief Close the XML balise
    ///
    /// If attribute mode Then
    ///		<ul><li>balise will be closed by '/>'</li></ul>
    /// Else
    ///		<ul><li>balise will be closed by '</' + balise.name + '>'
    ///		</li></ul>
    ///
    /// @param name Name of balise to close. \n
    ///				Ignored in attribute mode format
    virtual void
    closeBlock(const std::string& name = "");

    /* Format data */
    /// @brief Format a data into XML.
    ///
    ///	If attribute mode Then
    ///		<ul><li>data will be formatted into XML attribute with
    ///			attribute name="data"</li></ul>
    ///	Else
    ///		<ul><li>data will be formatted into XML balise with
    ///			balise name="data"</li></ul>
    ///
    /// @param data Data to format
    virtual void
    format(const Value& data);

    /// @brief Format a data into XML.
    ///
    ///	If attribute mode Then
    ///		<ul><li>data will be formatted into XML
    ///			attribute with name=propertyName</li></ul>
    ///	Else
    ///		<ul><li>data will be formatted into XML balise with
    ///			balise name=propertyName</li></ul>
    ///
    /// @param propertyName Name of XML attribute or balise.
    /// @param data Data to format
    virtual void
    format(const std::string& propertyName, const Value& data);

    /* Format a Comment */

    /// @brief Format a comment in XML
    ///
    /// Comment will be formmatted into standard XML comment.
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

    /// @brief Format a value in XML: protect the data with appropriate
    ///			html entities.
    /// @param data Data to format and out string for result
    /// @param forXMLName Indicate if is for a XML balise or attribute name
    /// @param isAttrValue Indicate if is a XML attribute value since the
    ///						format is different for XML attribute value and
    ///						XML balise content.
    static void
    htmlentities(std::string& data, bool forXMLName = false, bool isAttrValue =
        false);

  protected:

    /* Members */
    XMLFormat _fmtOpt; ///< Option of the formatter
    std::string _formattedData; ///< Formatted values
    std::string _addedTab; ///< Tabulation string added for each new balise

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::XMLFormatter
///	@ingroup format
///
/// This class format data into XML format, with 2 mode attribute and balise.
/// 
/// @see cea::XMLFormat
///////////////////////////////////////////////////////////////////////////////
