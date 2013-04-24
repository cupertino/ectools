///////////////////////////////////////////////////////////////////////////////
/// @file		XMLFormat.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		XML format option
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_XML_FORMAT_H__
#define LIBEC_XML_FORMAT_H__

#include <string>

#include "../base/BaseFormat.h"

namespace cea
{

  /* Prototype */
  class XMLFormatter;

  /// @brief XML Format Option
  class XMLFormat : public BaseFormat
  {
  public:

    /** Constructor */
    /// @brief Default Constructor
    XMLFormat();
    /// @brief Create a XMLFormat for XMLFormatter setting
    /// @param attributeMode If true then name for value will be attr
    ///                      Else name for value will be balise
    ///                      Default = true
    /// @see XMLFormatter::format
    XMLFormat(bool attributeMode);
    /// @brief Create a XMLFormat for XMLFormatter setting
    /// @param tabIndent Start indentation in XML data default = 0
    /// @see XMLFormatter::format
    XMLFormat(int tabIndent);
    /// @brief Create a XMLFormat for XMLFormatter setting
    /// @param tabIndent Start indentation in XML data default = 0
    /// @param attributeMode If true then name for value will be attr
    ///                      Else name for value will be balise
    ///                      Default = true
    /// @see XMLFormatter::format
    XMLFormat(bool attributeMode, int tabIndent);

    /// @brief Copy constructor
    /// @param fmtOpt XMLFormat to copy
    XMLFormat(const XMLFormat& fmtOpt);

    /// @brief Operator assign '='
    /// @param fmtOpt XMLFormat to copy
    XMLFormat&
    operator=(const XMLFormat& fmtOpt);

    /// @brief Load XMLFormatter option from XML
    ///
    /// Load settings by balise attribute.
    ///
    /// <h3>List of xml attribute loaded</h3>
    /// <ul>
    ///     <li><b>attr-mode</b> == {"true" | "1"; "false" | "0"}.
    ///         By default = "true"</li>
    ///     <li><b>indent</b> == Start indentation in XML data.
    ///         By default = "0"</li>
    /// </ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <!-- This example set XML format in attribute mode and set
    ///          start indent = 4 -->
    ///     <log-format attr-mode="true" indent="4" />
    ///
    ///     <!-- This example set XML format in balise mode with no start
    ///          indent -->
    ///     <log-format attr-mode="0" />
    /// @endcode
    ///
    /// @param xml XML reader
    /// @param balise XML Balise containing all the attributes
    virtual void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Create a new XMLFormatter
    /// @return XMLFormatter created or 0 if no enough memory
    BaseFormatter*
    createFormatter() const;

    /* Friend class */
    friend class XMLFormatter;

  public:

    /* Member */
    bool attrMode; ///< Indicate if mode attribute - default = true

  protected:

    /** Indicate the offset indentation */
    std::string _offsetIndent;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::XMLFormat
///	@ingroup format
///
/// This class represent the setting of XMLFormatter. \n \n
///
/// A block in XMLFormat is represented by a xml balise. \n \n
/// There are 2 mode of format in XMLFormatter.
/// <ol>
///     <li>Attribute mode: create an attribute for each value in the open block.
///         formatted. \n
///         This is the default mode.
///     <li>Balise mode: create a balise for each value in the open block.
/// </ol>
/// <h3>Example</h3>
/// <!--=======-->
/// Format function call:
/// @code
///     cea::XMLFormatter xml;
///     xml.openBlock("process")
///     xml.format("name", "ectop");
///     xml.format("pid", "1001");
///     xml.format("test");
///     xml.closeBlock("process");
/// @endcode
/// In attribute mode:
/// @code{.xml}
///     <process name="ectop" pid="1001" data="test" />
/// @endcode
/// In balise mode:
/// @code{.xml}
///     <process>
///         <name>ectop</name>
///         <pid>1001</pid>
///         <data>test</data>
///     </process>
/// @endcode
///////////////////////////////////////////////////////////////////////////////
