///////////////////////////////////////////////////////////////////////////////
/// @file               JsonGridView.h
/// @author             Dorian RODDE
/// @version    0.1
/// @date               2012.07
/// @copyright  2012, CoolEmAll (INFSO-ICT-288701)
/// @brief              Json GridModel view
///////////////////////////////////////////////////////////////////////////////

#ifndef JASONGRIDVIEW_H__
#define JASONGRIDVIEW_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <libec/grid/view/GridView.h>

namespace cea
{

  /// @brief Json GridModel view
  class JsonGridView : public GridView
  {
  public:

    /* Constructor */
    /// @brief Default constructor
    ///
    /// <h3>Default header source</h3>
    /// @code{.xml}
    ///         <!doctype Json>
    ///         <Json>
    ///                 <head>
    ///                         <title>Auto-generated page by libec</title>
    ///                         <link rel='stylesheet' type='text/css'
    ///                   href='cea_grid.css' />
    ///                 </head>
    ///         <body>
    /// @endcode
    /// <h3>Default footer source</h3>
    /// @code{.xml}
    ///                 </body>
    ///         </Json>
    /// @endcode
    ///
    JsonGridView();
    /// @brief Create an Json view of a GridModel
    ///
    /// <h3>Default header source</h3>
    /// @code{.xml}
    ///         <!doctype Json>
    ///         <Json>
    ///                 <head>
    ///                         <title>Auto-generated
    ///                    page by libec
    ///             </title>
    ///                         <link rel='stylesheet'
    ///                   type='text/css'
    ///                href='cea_grid.css'
    ///              />
    ///                 </head>
    ///         <body>
    /// @endcode
    /// <h3>Default footer source</h3>
    /// @code{.xml}
    ///                 </body>
    ///         </Json>
    /// @endcode
    ///
    /// @param model GridModel to connect to
    JsonGridView(GridModel& model);

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
    /// Clear Json result add Json header and open Json table
    ///
    virtual void
    beforeRender();

    /// @brief Function called after function render
    ///
    /// Close Json table add Json footer and save Json result into
    /// output file
    ///
    /// @param viewWidth Width of the view
    /// @param viewHeight Height of the view
    virtual void
    afterRender(int viewWidth, int viewHeight);

    /* Property */
    /// @brief Set Json source option
    /// @param header Json source for header
    /// @param end Json source for footer
    void
    setJsonSource(const std::string& header, const std::string& end);

    /// @brief Set output file
    /// @param outputFile Out file
    void
    setOutput(const std::string& outputFile);

    /// @brief Get output file
    /// @return Output file
    std::string
    getOutput() const;

    /* Function */
    /// @brief Get Json result
    /// @return content of view in Json
    std::string
    getContent() const;

    /// @brief Save Json result in a file
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
    ///     <li><b>Json-header</b> == Json source for header.
    ///         By default see JsonGridView() construtor</li>
    ///     <li><b>Json-footer</b> == Json source for footer.
    ///         By default see JsonGridView() construtor</li>
    ///     <li><b>output</b>, attributes loaded
    ///                 <ul>
    ///                         <li><b>file</b> == Output file. </li>
    ///                 </ul>
    ///         By default no output file is set.
    ///         </li>
    /// </ul>
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
    ///     <view type="Json" controller="terminal">
    ///
    ///         <Json-header>
    ///             <![CDATA[
    ///                 <Json>
    ///                     <head>
    ///                          <title>Json grid result</title>
    ///                     </head>
    ///                 <body>
    ///             ]]>
    ///         </Json-header>
    ///
    ///         <Json-end>
    ///             <![CDATA[
    ///                     </body>
    ///                 </Json>
    ///             ]]>
    ///         </Json-end>
    ///
    ///                 <output file="Json_result.Json"/>
    ///     </view>
    /// @endcode
    ///
    /// @param model GridModel loaded
    /// @param xml XML parser
    /// @param balise XML Balise containing all the attributes
    /// @see JsonGridView::loadSpecificXML
    virtual void
    loadSpecificXML(GridModel& model, XMLReader& xml,
        const XMLReader::Balise& balise);

  protected:

    /// @copydoc GridView::getShowText
    virtual void
    getShowText(const std::string& txt, unsigned int colSize,
        const GridStyle& style, std::string& retText);

    std::string _Json; ///< Json result source
    std::string _JsonHeader; ///< Json Header source
    std::string _JsonEnd; ///< Json Footer source
    std::string _outputFile; ///< Json output File

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///     @class cea::JsonGridView
///     @ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
