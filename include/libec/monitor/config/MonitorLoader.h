///////////////////////////////////////////////////////////////////////////////
/// @file		MonitorLoader.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		This class is the xml loader implementation of Monitor
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MONITORLOADER_H__
#define LIBEC_MONITORLOADER_H__

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "../../tools/Tools.h"
#include "../../value/Value.h"
#include "../../tools/XMLReader.h"
#include "../../grid/view/GridView.h"
#include "../../grid/controller/GridCtrl.h"
#include "../../Globals.h"

#include "Loadable.h"

namespace cea
{

  /* Prototype of Monitor */
  class Monitor;

  /// @brief This class is the xml loader implementation of Monitor
  class MonitorLoader
  {
  public:

    /// @brief Loadable Map
    typedef std::map<std::string, BaseLoadable*> LoaderMap;

    /// @brief List of loaded Elements
    typedef std::list<BaseLoadable*> LoadedList;

    /// @brief Contructor: register loader for View (term,html,chart-html)
    ///			and controller ("terminal")
    MonitorLoader();

    /// @brief Destructor - Destroy all the loaded elements
    virtual
    ~MonitorLoader();

    /// @brief Load a file
    ///
    /// Load all elements from XML
    ///
    /// <h3>List of xml balise loaded</h3>
    /// <ul>
    ///	<li><b>monitor</b>, balise loaded:
    /// <ul>
    ///     <li><b>freq</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>value</b> == Frequency in ms --
    ///                  By default = 1000 ms</li>
    ///			</ul>
    ///		</li>
    ///     <li><b>filter</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>value</b> == Filter in specific formatted
    ///                 string</li>
    ///			</ul>
    ///		</li>
    ///     <li><b>column</b><br>Attributes loaded:
    ///			<ul>
    ///				<li><b>type</b> ==
    ///             {"bool"; "int"; "uint"; "double" | "float"; "string"}
    ///             -- Type of the cell -- By default = "int"
    ///                (see Value::Type)</li>
    ///				<li><b>name</b> == Name of the column. (can be empty)
    ///				</li>
    ///				<li><b>title</b> == Title of the column header --
    ///                 By default = name</li>
    ///				<li><b>fixed</b> == Fixed position --
    ///                 If < 0 Then column not fixed.
    ///					By default = -1</li>
    ///				<li><b>fixed-mode</b> ==
    ///             {"start" | "begin"; "last" | "end"}
    ///             -- Indicate the fixed edge of the view.
    ///             By default = "begin"</li>
    ///				<li><b>tag</b> == Tag of the column. By default = -1
    ///             </li>
    ///				<li><b>percent-size</b> == Percent size of the column.
    ///                 By default = 0</li>
    ///				<li><b>min-size</b> == Minimum size of the column.
    ///                 By default = 0</li>
    ///				<li><b>size</b> == Allias for minimum size.
    ///                 By default = 0</li>
    ///				<li><b>class</b> == Class of the monitorable to map.
    ///                 If "" Then No monitorable mapped.
    ///                 By default = "" </li>
    ///				<li><b>object</b> == Name of the monitorable to map.
    ///					If "" Then No monitorable mapped.
    ///					By default = ""</li>
    ///			</ul>
    ///			Balise loaded:
    ///			<ul>
    ///				<li><b>arg</b>, attributes loaded:
    ///					<ul>
    ///						<li><b>id</b> == Id of the arg.
    ///							By default = current count of arg</li>
    ///						<li><b>type</b> ==
    ///				 {"bool"; "int"; "uint"; "double" | "float"; "string"}
    ///				 Type of the arg. By default = "int".</li>
    ///						<li><b>value</b> == Value of the arg.</li>
    ///					</ul>
    ///				</li>
    ///			</ul>
    ///		</li>
    ///		<li><b>row</b>, attributes loaded:
    ///			<ul>
    ///				<li><b>tag</b> == Tag of the column. By default = -1
    ///             </li>
    ///				<li><b>fixed</b> == Fixed position --
    ///                 If < 0 Then row not fixed.
    ///					By default = -1</li>
    ///				<li><b>fixed-mode</b> ==
    ///             {"start" | "begin"; "last" | "end"}
    ///             -- Indicate the fixed edge of the view.
    ///             By default = "begin"</li>
    ///			</ul>
    ///		</li>
    ///	</ul>
    ///	<li><b>view</b>, attributes loaded:
    ///		<ul>
    ///			<li><b>type</b> == Class of the view.
    ///				By default = "classic"</li>
    ///			<li><b>controller</b> == Class of the controller</li>
    ///			<li><b>ctrl</b> == Alias of controller</li>
    ///			<li>Inner balise will be loaded by the GridViewLoader
    ///               (see GridViewLoader::loadXML for details)</li>
    ///		</ul>
    ///	</li>
    ///	</ul>
    ///
    /// <h3>Example</h3>
    /// @code{.xml}
    /// 	<monitor>
    /// 		<!-- Set frequency to 1s -->
    /// 		<freq value="1000" />
    ///
    /// 		<!-- Set filter -->
    /// 		<filter value="(PID = 100,200) or (Name = 'ectop')" />
    ///
    ///
    /// 		<!-- Add Column -->
    /// 		<column name="PID" type="INT" tag="0"
    /// 			min-size="30" percent-size="30" />
    /// 		<column name="Name" type="STRING" tag="1"
    /// 			min-size="30" percent-size="30" />
    ///
    ///
    /// 		<!-- Sensor Mapped column -->
    /// 		<column name="Sensor" title="CPU" type="INT"
    /// 			tag="2" class="sensor" object="FakeSensor" />
    ///
    ///
    /// 		<!-- CPU Core column - Set for each core the id -->
    /// 		<column name="CPU0" title="CPU" type="INT" tag="3">
    /// 			<arg id="1" type="INT" value="0" />
    /// 		</column>
    /// 		<column name="CPU1" title="CPU" type="INT" tag="3" />
    /// 			<arg id="1" type="INT" value="1" />
    /// 		</column>
    /// 		<column name="CPU2" title="CPU" type="INT" tag="3" />
    /// 			<arg id="1" type="INT" value="2" />
    /// 		</column>
    /// 		<column name="CPU3" title="CPU" type="INT" tag="3" />
    /// 			<arg id="1" type="INT" value="3" />
    /// 		</column>
    ///
    ///
    /// 		<!-- Add a fixed row -->
    /// 		<row tag="50" fixed="0" fixed-mode="end" />
    /// 	</monitor>
    /// @endcode
    ///
    /// @param file File to open
    void
    loadFile(const std::string& file);

    /// @brief Load a string
    ///
    /// @copydetails MonitorLoader::loadFile
    ///
    /// @param str String to parse
    void
    loadString(const std::string& str);

    /* Loader functions */
    /// @brief Delete all Created Elements (View, Controller, Monitorable)
    void
    deleteAllCreatedElements();

    /// @brief Map a loader for monitorable
    ///
    /// The loader is mapped with a string formatted by className::type
    ///
    /// @param className Class of the loader (sensor, estimator, etc ...)
    /// @param type Type of monitorable ("EstimatorA", etc ...)
    template<typename T>
      void
      loadMapMonitorable(const std::string& className, const std::string& type)
      {
        Loadable<T>* loader = new Loadable<T>;
        if (loader != 0)
          {
            monitorableLoaders.insert(
                LoaderMap::value_type(Tools::toLower(className + "::" + type),
                    loader));
          }
      }

    /// @brief Map a loader for view
    /// @param type Type of view ("terminal", "html", "pie-html", etc ...)
    template<typename T>
      void
      loadMapView(const std::string& type)
      {
        Loadable<T>* loader = new Loadable<T>;
        if (loader != 0)
          {
            viewLoaders.insert(
                LoaderMap::value_type(Tools::toLower(type), loader));
          }
      }

    /// @brief Map a loader for Controller
    /// @param type Type of controller ("terminal", etc ...)
    template<typename T>
      void
      loadMapController(const std::string& type)
      {
        Loadable<T>* loader = new Loadable<T>;
        if (loader != 0)
          {
            ctrlLoaders.insert(
                LoaderMap::value_type(Tools::toLower(type), loader));
          }
      }

    /// @brief Load monitorable
    /// @param className Class of the loader
    /// @param type Type of monitorable
    /// @return Generic pointer for created monitorable
    ///			Or 0 if no loader found
    /// @see MonitorLoader::loadMapMonitorable
    void*
    loadMonitorable(const std::string& className, const std::string& type);

    /// @brief Load view
    /// @param type Type of view
    /// @return View created or 0 if type not corresponding
    /// @see MonitorLoader::loadMapView
    GridView*
    loadView(const std::string& type);

    /// @brief Load controller
    /// @param type Type of controller
    /// @return Controller created or 0 if type not corresponding
    /// @see MonitorLoader::loadMapController
    GridCtrl*
    loadController(const std::string& type);

    /// @brief This function is used only to get a ref to Monitor
    /// @return Reference of current Monitor
    virtual Monitor&
    getMonitorRef() = 0;

    /// @brief Load all data from xml parser
    /// @param xml XML Reader
    virtual void
    loadXML(XMLReader& xml);

  protected:
    /* Member */
    /// @brief XML Parser
    XMLReader xml;

    /* Loader */
    LoaderMap monitorableLoaders; ///< List of Creater element
    LoaderMap viewLoaders; ///< List of Creater element
    LoaderMap ctrlLoaders; ///< List of Creater element
    LoadedList loadedElements; ///< List of elements created

  private:

    /// @brief List of loaded GridItem ptr
    typedef std::list<GridItem*> LoadedGridItemList;

    /// @brief Transform string to Value::Type
    /// @param type String representing the Value::Type
    /// @return Value::Type corresponding
    Value::Type
    stringToValueType(const std::string& type);

    /// @brief List of loaded GridColumn
    LoadedGridItemList loadedColumnLst;

    /// @brief List of loaded GridRow
    LoadedGridItemList loadedRowLst;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::MonitorLoader
///	@ingroup monitor
///
/// @copydetails cea::MonitorLoader::loadFile
///
///////////////////////////////////////////////////////////////////////////////
