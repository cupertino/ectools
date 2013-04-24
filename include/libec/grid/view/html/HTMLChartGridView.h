///////////////////////////////////////////////////////////////////////////////
/// @file		HTMLChartGridView.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		HTML GridModel chart view
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_HTMLCHARTGRIDVIEW_H__
#define LIBEC_HTMLCHARTGRIDVIEW_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "HTMLGridView.h"
#include "../../../tools/Console.h"

namespace cea
{

  /// @brief HTML GridModel chart view
  class HTMLChartGridView : public HTMLGridView
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
    ///                   href='cea_chart.css' />
    ///			</head>
    ///		<body>
    /// @endcode
    /// <h3>Default footer source</h3>
    /// @code{.xml}
    ///			</body>
    ///		</html>
    /// @endcode
    ///
    HTMLChartGridView();
    /// @brief Create an HTML chart view of a GridModel
    ///
    /// <h3>Default header source</h3>
    /// @code{.xml}
    ///		<!doctype html>
    ///		<html>
    ///			<head>
    ///				<title>Auto-generated page by libec</title>
    ///				<link rel='stylesheet' type='text/css'
    ///                   href='cea_chart.css' />
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
    HTMLChartGridView(GridModel& model);

    /* Chart public function */
    /// @brief Set Chart column
    /// @param keyColumn Column containing keys
    /// @param valueColumn Column containing values
    void
    setChartColumn(const GridColumn& keyColumn, const GridColumn& valueColumn);

    /* Render Method */
    /// @copydoc HTMLGridView::renderColumnHeader
    virtual void
    renderColumnHeader(const ColumnView& col, int colId,
        const std::string& text, int x, int y, int colSize,
        const GridStyle& style);

    /// @copydoc HTMLGridView::renderRow
    virtual void
    renderRow(const RowView& row, int rowId, int x, int y, int viewWidth);
    /// @copydoc HTMLGridView::renderCell
    virtual void
    renderCell(const GridCell& c, int rowId, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @copydoc HTMLGridView::renderSumBar
    virtual void
    renderSumBar(int x, int y, int viewWidth);
    /// @copydoc HTMLGridView::renderSumBarItem
    virtual void
    renderSumBarItem(const ColumnView& col, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @copydoc GridView::beforeRender
    virtual void
    beforeRender();

    /// @brief Function called after function render
    ///
    /// Add html footer and save html result into output file. \n
    /// This function can be override.
    ///
    /// @param viewWidth Width of the view
    /// @param viewHeight Height of the view
    virtual void
    afterRender(int viewWidth, int viewHeight);

    /// @brief Load specific XML element
    ///
    /// This function can be overridden in order to load specific
    /// properties.
    ///
    ///
    /// <h3>List of xml specific balise loaded</h3>
    /// <ul>
    ///     <li><b>chart</b>, attributes loaded
    ///			<ul>
    ///				<li><b>key</b> == GridColumn used for keys</li>
    ///				<li><b>value</b> == GridColumn used for values</li>
    ///         </ul>
    ///     </li>
    ///     <li><b>html-header</b> == HTML source for header.
    ///         By default see HTMLChartGridView() construtor</li>
    ///     <li><b>html-footer</b> == HTML source for footer.
    ///         By default see HTMLChartGridView() construtor</li>
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
    ///     <view type="chart-html" controller="terminal">
    ///
    ///         <chart key="PID" value="CPU" />
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
    ///
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

    /* Chart event function */
    /// @brief Event raised when a header of key column is rendered
    /// @param txt Title of the column header
    virtual void
    addChartHeaderKey(const std::string& txt);
    /// @brief Event raised when a header of value column is rendered
    /// @param txt Title of the column header
    virtual void
    addChartHeaderValue(const std::string& txt);
    /// @brief Event raised when a cell of key column is rendered
    /// @param txt Value of the cell
    virtual void
    addChartKey(const std::string& txt);
    /// @brief Event raised when a cell of value column is rendered
    /// @param txt Value of the cell
    virtual void
    addChartValue(const std::string& txt);

    /// @copydoc GridView::getShowText
    virtual void
    getShowText(const std::string& txt, unsigned int colSize,
        const GridStyle& style, std::string& retText);

    /* Members */
    /// @brief Column representing the key of chart
    const GridColumn* _keyColumn;

    /// @brief Column representing the value of chart
    const GridColumn* _valueColumn;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::HTMLChartGridView
///	@ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
