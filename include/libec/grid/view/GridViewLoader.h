///////////////////////////////////////////////////////////////////////////////
/// @file		GridViewLoader.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Load implementation of the GridView
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDVIEWLOADER_H__
#define LIBEC_GRIDVIEWLOADER_H__

#include <iostream>
#include <string>

#include "../../tools/XMLReader.h"

namespace cea
{

  /* Prototype */
  class GridView;
  class GridFilter;
  class GridViewLog;
  class GridModel;

  /// @brief Load implementation of the GridView
  class GridViewLoader
  {
  public:

    /* Load XML data */
    /// @brief Load view from XML
    ///
    /// Load all view elements from an XML balise
    ///
    /// <h3>List of xml balise loaded</h3>
    /// <ul>
    ///     <li><b>position</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>x</b> == Left position -- By default = 0</li>
    ///				<li><b>y</b> == Top position -- By default = 0</li>
    ///				<li><b>w</b> == View width -- By default = -1
    ///                 (auto width)</li>
    ///				<li><b>h</b> == View height -- By default = -1
    ///                 (auto height)</li>
    ///				<li><b>offsetw</b> == Offset width of auto size --
    ///                  By default = 0</li>
    ///				<li><b>offseth</b> == Offset height of auto size --
    ///                  By default = 0</li>
    ///			</ul>
    ///			By default left, top, offsetw, offseth = 0 and w, h = -1
    ///		</li>
    ///		<li><b>style</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>id</b> == Id of the style</li>
    ///				<li>Other attributes are detailled in
    ///                 GridStyle::loadXML</li>
    ///			</ul>
    ///		</li>
    ///		<li><b>style-filter</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>style</b> == Style Id to associate with filter
    ///                            </li>
    ///				<li><b>filter</b> == Filter in specific formatted
    ///                 string</li>
    ///			</ul>
    ///		</li>
    ///		<li><b>hide</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>columns</b> == List of GridColumn to hide.
    ///					GridColumn name separated by "," </li>
    ///			</ul>
    ///		</li>
    ///		<li><b>show</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>columns</b> == List of GridColumn to show.
    ///					Also indicates the order of GridColumn.
    ///					GridColumn name separated by "," </li>
    ///			</ul>
    ///		</li>
    ///		<li><b>sum</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>columns</b> == List of GridColumn to
    ///                 activate sum.
    ///					GridColumn name separated by "," </li>
    ///			</ul>
    ///		</li>
    ///		<li><b>filter</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>value</b> == Filter in specific formatted
    ///                 string</li>
    ///			</ul>
    ///		</li>
    ///		<li><b>sort</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>order</b> == Sort order
    ///					{"ascending" | "asc"; "descending" | "desc"}.
    ///					By default order="asc".
    ///             </li>
    ///				<li><b>by</b> == Sort ref GridColumn</li>
    ///			</ul>
    ///			By default sort is disabled.
    ///		</li>
    ///		<li><b>log</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>type</b> == Type of the log default="file"
    ///					(see LogCreator::create) </li>
    ///				<li><b>format</b> == Format of the log default="xml"
    ///					(see FormatCreator::create) </li>
    ///
    ///				<li><b>flush-freq</b> == Set Flush frequency in ms.
    ///					By default = 0 (see Log::setFlushFrequency) </li>
    ///				<li><b>flush-max-item</b> == Set max item before flush
    ///					By default = 0 (see Log::setFlushItemCount) </li>
    ///
    ///				<li><b>default-block</b> == Set default block type name
    ///                 (see Log::setBlockType) </li>
    ///				<li><b>added-block</b> == Set added block type name
    ///                 (see Log::setBlockType) </li>
    ///				<li><b>erased-block</b> == Set erased block type name
    ///                 (see Log::setBlockType) </li>
    ///				<li><b>updated-block</b> == Set updated block type name
    ///                 (see Log::setBlockType) </li>
    ///
    ///			</ul>
    ///		</li>
    ///		<li><b>log-type</b>, attributes are detailled in loadXML of
    ///			each Log option (see BaseLog::loadXML)</li>
    ///		<li><b>log-format</b>, attributes are detailled in loadXML of
    ///			each Format option (see BaseFormat::loadXML)</li>
    ///     <li><b>option</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>additional-header-line</b> == If  = 1 then an
    ///					additional line after column line will be added
    ///					on terminal view. \n
    ///					By Default = 0</li>
    ///				<li><b>additional-sum-line</b> == If  = 1 then an
    ///					additional line before sum line will be added
    ///					on terminal view. \n
    ///					By Default = 0</li>
    ///			</ul>
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
    ///     <view type="terminal" controller="terminal">
    ///         <!-- Position of the view
    ///                (autosize of the width and height) -->
    ///         <position x="0" y="0" w="0" h="0"
    ///                     offsetw="3" offseth="1"/>
    ///
    ///         <!-- Configure log -->
    ///         <log     type="file"
    ///                  format="xml"
    ///                  flush-freq="5000"
    ///                  flush-max-item="100"
    ///                  add-block="add"
    ///                  default-block="data"
    ///                  delete-block="delete"
    ///                  update-block="update"
    ///                  filter="(NAME = 'ectop')" />
    ///         <!-- Configure file log type -->
    ///         <log-type clear="true" output="out.log" />
    ///         <!-- Configure xml format -->
    ///         <log-format attr-mode="true" />
    ///
    ///         <!-- Filter in the view -->
    ///         <filter value="(PID > 100) . (PID < 300)" />
    ///
    ///         <!-- Hide CPU and SENSOR1 column all other column will be
    ///              show -->
    ///         <hide columns="CPU,SENSOR1" />
    ///
    ///         <!-- Add a Style with key=5 and {align=GridStyle::RIGHT-->
    ///         <style id="5" align="right" color="#rrggbb"
    ///                         backcolor="#rrggbb" prefix="PID:"
    ///                          suffix="%" bold="true" />
    ///
    ///
    ///         <!-- Filter Style all row with PID > 150 will be stylized
    ///              with style number 5 -->
    ///         <style-filter style="5" filter="(PID > 150)" />
    ///
    ///
    ///         <!-- Activate sort by PID column and order by ASCENDING -->
    ///         <sort order="ASC" by="PID" />
    ///
    ///     </view>
    /// @endcode
    ///
    /// @param model GridModel loaded
    /// @param xml XML parser
    /// @param balise XML Balise containing all the attributes
    /// @see HTMLGridView::loadSpecificXML
    void
    loadXML(GridModel& model, XMLReader& xml, const XMLReader::Balise& balise);

    /// @brief Load specific XML element
    ///
    /// This function is called at the end of the function loadXML. \n
    /// This function do nothing by default. \n
    /// This function can be overridden in order to load specific
    /// properties. (for example see HTMLGridView::loadSpecificXML)
    ///
    /// @param model GridModel loaded
    /// @param xml XML parser
    /// @param balise XML Balise containing all the attributes
    /// @see HTMLGridView::loadSpecificXML
    virtual void
    loadSpecificXML(GridModel& model, XMLReader& xml,
        const XMLReader::Balise& balise);

  protected:

    /// @brief Loader integration
    /// @return Pointer of this current GridView
    virtual GridView*
    getGridViewLoaderRef() = 0;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridViewLoader
///	@ingroup grid_view
///
/// @copydetails GridView::loadXML
///
///////////////////////////////////////////////////////////////////////////////
