///////////////////////////////////////////////////////////////////////////////
/// @file		HTMLGridView.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		HTML GridModel view
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_HTMLGRIDVIEW_H__
#define LIBEC_HTMLGRIDVIEW_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "../GridView.h"
#include "../../../tools/Console.h"

namespace cea
{

  /// @brief HTML GridModel view
  class HTMLGridView : public GridView
  {
  public:

    /* Constructor */
    /// @brief Default constructor
    ///
    /// <h3>Default header source</h3>
    /// @code{.xml}
    ///		<!doctype html>
    ///		<html>
    ///			<head>
    ///				<title>Auto-generated page by libec</title>
    ///				<link rel='stylesheet' type='text/css'
    ///                   href='cea_grid.css' />
    ///			</head>
    ///		<body>
    /// @endcode
    /// <h3>Default footer source</h3>
    /// @code{.xml}
    ///			</body>
    ///		</html>
    /// @endcode
    ///
    HTMLGridView();
    /// @brief Create an HTML view of a GridModel
    ///
    /// <h3>Default header source</h3>
    /// @code{.xml}
    ///		<!doctype html>
    ///		<html>
    ///			<head>
    ///				<title>Auto-generated
    ///                    page by libec
    ///             </title>
    ///				<link rel='stylesheet'
    ///                   type='text/css'
    ///                href='cea_grid.css'
    ///              />
    ///			</head>
    ///		<body>
    /// @endcode
    /// <h3>Default footer source</h3>
    /// @code{.xml}
    ///			</body>
    ///		</html>
    /// @endcode
    ///
    /// @param model GridModel to connect to
    HTMLGridView(GridModel& model);

    /* Render Method */
    /// @copydoc GridView::renderColumnHeader
    virtual void
    renderColumnHeader(const ColumnView& col, int colId,
        const std::string& text, int x, int y, int colSize,
        const GridStyle& style);

    /// @copydoc GridView::renderRow
    virtual void
    renderRow(const RowView& row, int rowId, int x, int y, int viewWidth);
    /// @copydoc GridView::renderCell
    virtual void
    renderCell(const GridCell& c, int rowId, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @copydoc GridView::renderSumBar
    virtual void
    renderSumBar(int x, int y, int viewWidth);
    /// @copydoc GridView::renderSumBarItem
    virtual void
    renderSumBarItem(const ColumnView& col, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @brief Function called before function render
    ///
    /// Clear html result add html header and open HTML table
    ///
    virtual void
    beforeRender();

    /// @brief Function called after function render
    ///
    /// Close HTML table add html footer and save html result into
    /// output file
    ///
    /// @param viewWidth Width of the view
    /// @param viewHeight Height of the view
    virtual void
    afterRender(int viewWidth, int viewHeight);

    /* Property */
    /// @brief Set html source option
    /// @param header HTML source for header
    /// @param end HTML source for footer
    void
    setHTMLSource(const std::string& header, const std::string& end);

    /// @brief Set output file
    /// @param outputFile Out file
    void
    setOutput(const std::string& outputFile);

    /// @brief Get output file
    /// @return Output file
    std::string
    getOutput() const;

    /* Function */
    /// @brief Get html result
    /// @return content of view in html
    std::string
    getContent() const;

    /// @brief Save html result in a file
    /// @param outputFile Out file
    /// @return true if save success
    bool
    saveToFile(const std::string& outputFile);

    /// @brief Load specific XML element
    ///
    /// This function can be overridden in order to load specific
    /// properties.
    ///
    ///
    /// <h3>List of xml specific balise loaded</h3>
    /// <ul>
    ///     <li><b>html-header</b> == HTML source for header.
    ///         By default see HTMLGridView() construtor</li>
    ///     <li><b>html-footer</b> == HTML source for footer.
    ///         By default see HTMLGridView() construtor</li>
    ///     <li><b>output</b>, attributes loaded
    ///			<ul>
    ///				<li><b>file</b> == Output file. </li>
    ///			</ul>
    ///         By default no output file is set.
    ///		</li>
    ///	</ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    ///     <monitor>
    ///         <column name="PID" tag="0" type="int" />
    ///         <column name="CPU" tag="1" type="float" />
    ///         <column name="SENSOR1" tag="2" class="sensor"
    ///                 object="FakeSensor"  />
    ///     </monitor>
    ///
    ///
    ///     <view type="html" controller="terminal">
    ///
    ///         <html-header>
    ///             <![CDATA[
    ///                 <html>
    ///                     <head>
    ///                          <title>HTML grid result</title>
    ///                     </head>
    ///                 <body>
    ///             ]]>
    ///         </html-header>
    ///
    ///         <html-end>
    ///             <![CDATA[
    ///                     </body>
    ///                 </html>
    ///             ]]>
    ///         </html-end>
    ///
    ///			<output file="html_result.html"/>
    ///     </view>
    /// @endcode
    ///
    /// @param model GridModel loaded
    /// @param xml XML parser
    /// @param balise XML Balise containing all the attributes
    /// @see HTMLGridView::loadSpecificXML
    virtual void
    loadSpecificXML(GridModel& model, XMLReader& xml,
        const XMLReader::Balise& balise);

  protected:

    /// @copydoc GridView::getShowText
    virtual void
    getShowText(const std::string& txt, unsigned int colSize,
        const GridStyle& style, std::string& retText);

    std::string _html; ///< HTML result source
    std::string _htmlHeader; ///< HTML Header source
    std::string _htmlEnd; ///< HTML Footer source
    std::string _outputFile; ///< HTML output File

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::HTMLGridView
///	@ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
