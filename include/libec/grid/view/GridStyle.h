///////////////////////////////////////////////////////////////////////////////
/// @file		GridStyle.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Style for GridView column, row and cell
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDSTYLE_H__
#define LIBEC_GRIDSTYLE_H__

#include "../../tools/Color.h"
#include "../../tools/Tools.h"
#include "../../tools/XMLReader.h"

namespace cea
{

  /// @brief Style for GridView column, row and cell
  struct GridStyle
  {
  public:

    /* Enumeration */
    /// @brief Text horizontal align constants
    enum Align
    {
      LEFT = 0, ///< (default) Left text align
      CENTER, ///< Center text align
      RIGHT ///< Right text align
    };

    /* Constructor */
    /// @brief Default constructor
    ///
    /// Set default value:
    /// <ul>
    ///		<li><b>align</b> == GridStyle::Left</li>
    ///		<li><b>color</b> == -1. \n
    ///         Get default view device text color</li>
    ///		<li><b>backColor</b> == -1. \n
    ///         Get default view device text background color</li>
    ///		<li><b>bold</b> == false</li>
    ///		<li><b>italic</b> == false</li>
    ///		<li><b>underline</b> == false</li>
    ///		<li><b>strikethrough</b> == false</li>
    ///		<li><b>size</b> == 0</li>
    ///		<li><b>border</b> == true</li>
    /// </ul>
    ///
    GridStyle();
    /// @brief Create a style with align option
    ///
    /// Set default value:
    /// <ul>
    ///		<li><b>color</b> == -1. \n
    ///         Get default view device text color</li>
    ///		<li><b>backColor</b> == -1. \n
    ///         Get default view device text background color</li>
    ///		<li><b>bold</b> == false</li>
    ///		<li><b>italic</b> == false</li>
    ///		<li><b>underline</b> == false</li>
    ///		<li><b>strikethrough</b> == false</li>
    ///		<li><b>size</b> == 0</li>
    ///		<li><b>border</b> == true</li>
    /// </ul>
    ///
    GridStyle(Align align);
    /// @brief Create a style with all option
    ///
    /// Set default value:
    /// <ul>
    ///     <li><b>backColor</b> == -1. \n
    ///         Get default view device text background color</li>
    ///		<li><b>bold</b> == false</li>
    ///		<li><b>italic</b> == false</li>
    ///		<li><b>underline</b> == false</li>
    ///		<li><b>strikethrough</b> == false</li>
    ///		<li><b>size</b> == 0</li>
    ///		<li><b>border</b> == true</li>
    /// </ul>
    ///
    GridStyle(Align align, color_t color, color_t backcolor = -1, bool bold =
        false, bool italic = false, bool underline = false, bool strikethrough =
        false, const std::string& prefix = "", const std::string& suffix = "",
        int size = 0, bool border = true);

    /* Property set */
    /// @brief Set text align
    /// @param a New align of the text
    /// @return Reference to the current object
    GridStyle&
    setAlign(Align a);
    /// @brief Set text color
    /// @param c New color of the text
    /// @return Reference to the current object
    GridStyle&
    setTextColor(color_t c);
    /// @brief Set text background color
    /// @param bc New background color of the text
    /// @return Reference to the current object
    GridStyle&
    setBackColor(color_t bc);
    /// @brief Set text colors
    /// @param c New color of the text
    /// @param bc New background color of the text
    /// @return Reference to the current object
    GridStyle&
    setColor(color_t c, color_t bc);
    /// @brief Set text align
    /// @param bOn Bold status of the text
    /// @return Reference to the current object
    GridStyle&
    setBold(bool bOn);
    /// @brief Set text align
    /// @param bOn Italic status of the text
    /// @return Reference to the current object
    GridStyle&
    setItalic(bool bOn);
    /// @brief Set text Underline
    /// @param bOn Underline status of the text
    /// @return Reference to the current object
    GridStyle&
    setUnderline(bool bOn);
    /// @brief Set text Strikethrough
    /// @param bOn Strikethrough status of the text
    /// @return Reference to the current object
    GridStyle&
    setStrikethrough(bool bOn);
    /// @brief Set text font
    /// @param b Bold status of the text
    /// @param i Italic status of the text
    /// @param u Underline status of the text
    /// @param s Strikethrough status of the text
    /// @return Reference to the current object
    GridStyle&
    setFont(bool b = false, bool i = false, bool u = false, bool s = false);
    /// @brief Set text prefix
    /// @param prefix New prefix of the text
    /// @return Reference to the current object
    GridStyle&
    setPrefix(const std::string& prefix);
    /// @brief Set text suffix
    /// @param suffix New suffix of the text
    /// @return Reference to the current object
    GridStyle&
    setSuffix(const std::string& suffix);
    /// @brief Set text affix
    /// @param prefix New prefix of the text
    /// @param suffix New suffix of the text
    /// @return Reference to the current object
    GridStyle&
    setAffix(const std::string& prefix, const std::string& suffix);
    /// @brief Set cell border property
    /// @param b If true Then border activate Else border deactivate
    /// @return Reference to the current object
    GridStyle&
    setBorder(bool b);

    /// @brief Load from XML balise
    ///
    /// Load all style from XML attribute of an XML balise
    ///
    /// <h3>List of xml attribute loaded</h3>
    /// <ul>
    ///     <li><b>align</b> == {"left"; "center" | "middle"; "right"}.
    ///         By default = "left"</li>
    ///     <li><b>color</b> == Text color in html CSS format #RRGGBB. \n
    ///         By default = "-1" ==> Take default view device text color
    ///     </li>
    ///     <li><b>backcolor</b> == Background text color in html CSS
    ///          format #RRGGBB. \n
    ///          By default = "-1" ==> Take default view device text color
    ///     </li>
    ///		<li><b>prefix</b> == Prefix of a cell. By default = ""</li>
    ///		<li><b>suffix</b> == Suffix of a cell. By default = ""</li>
    ///		<li><b>bold</b> == {"true" | "1"; "false" | "0"}.
    ///         By default = "false"</li>
    ///		<li><b>italic</b> == {"true" | "1"; "false" | "0"}.
    ///         By default = "false"</li>
    ///		<li><b>underline</b> == {"true" | "1"; "false" | "0"}.
    ///         By default = "false"</li>
    ///		<li><b>strikethrough</b> == {"true" | "1"; "false" | "0"}.
    ///         By default = "false"</li>
    /// </ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <!-- Style (align=GridStyle::Center,
    ///                 suffix="%",
    ///                 color=Color::Red) -->
    ///     <style align="Center" suffix="%" color="#ff0000" />
    /// @endcode
    ///
    /// @param xml XML parser
    /// @param balise XML Balise containing all the attributes
    void
    loadXML(XMLReader& xml, const XMLReader::Balise& balise);

    /* Style property */
    Align align; ///< Align of the text
    color_t color; ///< Color of the text
    color_t backColor; ///< Background color of the text
    bool bold; ///< Bold status of the text
    bool italic; ///< Italic status of the text
    bool underline; ///< Underline status of the text
    bool strikethrough; ///< Strikethrough status of the text

    std::string prefix; ///< Prefix of the text
    std::string suffix; ///< Suffix of the text

    unsigned int size; ///< Reserved property for GridStyle

    bool border; ///< Border status of the cell

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridStyle
///	@ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
