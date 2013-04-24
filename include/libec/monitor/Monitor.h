///////////////////////////////////////////////////////////////////////////////
/// @file       Monitor.h
/// @author     Dorian RODDE
/// @version    0.1
/// @date       2012.07
/// @copyright  2012, CoolEmAll (INFSO-ICT-288701)
/// @brief      Class to extend to build the specific monitor
///
/// \section example1 Short example of monitor loaded from XML file
/// This example show how to create a monitor of running process
/// from a XML file.
///
/// This example create a terminal view and a html view in data.html controlled
/// by a TerminalController.
///
/// This code works on Linux and Windows without changing anything.
///
/// <h3>XML monitor loaded</h3>
/// @code{.xml}
/// <monitor>
///   <!-- Column Tag -->
///   <!-- 0 == Name -->
///   <!-- 1 == PID -->
///   <!-- 2 == Sensor -->
///
///   <freq value="200" />
///
///   <column name="Name" type="string" size="30" tag="0" />
///   <column name="PID" type="int" size="10" tag="1" />
///
///   <column name="S1" type="int" size="10" tag="2" class="sensor"
///                                             object="fakesensor"  />
///   <column name="S2" type="int" size="10" tag="2" class="sensor"
///                                             object="fakesensor">
///     <arg id="54" type="float" value="78.56" />
///     <arg type="string" value="FakeSensor test" />
///   </column>
/// </monitor>
///
/// <view type="term" ctrl="term">
///   <show columns="Name,PID,S1" />
///
///   <filter value="PID < 2000" />
///
///   <style id="1" align="Center" color="#ff0000" />
///   <style id="2" align="RIGHT" suffix="%" color="#00FF00" />
///
///   <style id="100" backcolor="#FFFF00" color="#ff0000"/>
///   <style id="50" backcolor="#008000" color="#ff0000"/>
///
///   <style-filter filter="(S1 > 10000)" style="100" />
///   <sort by="Name" order="asc" />
///
///   <sum columns="S1" />
///
///   <position x="0" y="3" offseth="3" offsetw="0"/>
///
///   <log type="file"
///        format="xml"
///        flush-freq="4000"
///        flush-max-item="100"
///        filter="(PID > 1000) . (PID < 2000)" />
///
///   <log-type clear="false" file="out.log" />
///   <log-format attr-mode="true" />
/// </view>
///
/// <view type="html" ctrl="term">
///     <output file="data.html" />
/// </view>
/// @endcode
///
/// <h3>Monitor source</h3>
/// @code
///
/// #include <libec/monitors.h>
/// #include <libec/process.h>
/// #include <libec/sensors.h>
///
/// // This using namespace is only to keep the example as short as possible
/// // but in C++ is better to avoid it.
/// using namespace cea;
///
/// class MyMonitor : public Monitor
/// {
/// public:
///   // Column tags
///   static const int NAME = 0;
///   static const int PID = 1;
///   static const int SENSOR_U64 = 2;
///   static const int SENSOR_FLOAT = 3;
///
///   // Member
///   bool isFreezed;
///
///   // Constructor
///   MyMonitor()
///     : isFreezed(false)
///   {
///     // Add loader
///     loadMapMonitorable<FakeSensor>("sensor", "FakeSensor");
///   }
///
///   // Event raised by Monitor after a Row created
///   void onRowCreate(Row& r)
///   {
///     if (r.tag == FEEDER_PROCESS_ITEM) {
///       setValue(r, NAME, cast<Process>(r)->getName());
///       setValue(r, PID, cast<Process>(r)->getPid());
///      }
///   }
///
///   // Event raised when a row need updated
///   void onRowUpdate(Row& r)
///   {
///     if (r.tag == FEEDER_PROCESS_ITEM) {
///       Process& p = cast<Process>(r);
///       // Update Sensor U64
///       for(ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s) {
///         setValue(r, s, s->getValue(p.getPid()).U64);
///       }
///       // Update Sensor Float
///       for(ColumnIterator<SENSOR_FLOAT, Sensor> s = this; s != 0; ++s){
///         setValue(r, s, s->getValue(p.getPid()).Float);
///       }
///     }
///   }
/// };
///
/// int main()
/// {
///   // Model
///   MyMonitor m;
///   ProcessEnumerator pe;
///   // Connect Process enumerator to monitor
///   m.connectRowFeeder(&pe);
///
///   // Load monitor
///   m.loadFile("monitor.xml");
///
///   // Variable to know application running status
///   bool isRunning = true;
///
///   // Main Loop
///   while (isRunning) {
///
///     // Update Monitor
///     if (!m.isFreezed) {
///       m.update();
///       pe.update();
///     }
///
///     // Handle Terminal Event
///     int c = Console::keyHit();
///     switch (c) {
///       case 0:
///         // = No key hit (keyHit is async method -> return 0 if no key)
///         break;
///       case 'q': case 'Q':
///         // Key for exit the application
///         isRunning = false;
///         break;
///       case 'p': case 'P':
///         // Key to pause the monitor
///         m.isFreezed = !m.isFreezed;
///         break;
///       case 'r': case 'R':
///         // Refresh
///         m.loadFile("monitor.xml");
///       default:
///         // Create a Terminal KeyHit GridCtrl event
///         GridCtrl::Event e(GridCtrl::Event::TERM_KEYHIT, c);
///         // If other key -> let the monitor controllers handle the event
///         m.handleEvent(e);
///     }
///
///     // Render Views
///     m.renderViews();
///
///     // Do a sleep of 50 ms to avoid comsuption of all CPU
///     // with letting the interpretation of key event
///     Tools::sleep_ms(50);
///   }
///
///   return 0;
/// }
/// @endcode
///
///
/// @section example2 Short example of a simple monitor
/// This example show how to create a monitor of running process
/// and show it in a terminal view.
///
/// This code works on Linux and Windows without changing anything.
///
/// @code
///
/// #include <libec/monitors.h>
/// #include <libec/process.h>
/// #include <libec/sensors.h>
///
/// // This using namespace is only to keep the example as short as possible
/// // but in C++ is better to avoid it.
/// using namespace cea;
///
/// class MyMonitor : public Monitor
/// {
/// public:
///     // Column tags
///     static const int NAME = 0;
///     static const int SENSOR_U64 = 2;
///     static const int SENSOR_FLOAT = 3;
///
///     // Constructor
///     MyMonitor()
///     {
///         addColumn("Name", Value::STRING, NAME,30,30);
///         addColumn("PID", Value::INT, PID);
///     };
///
///     // Add Sensor column
///     void addSensor(Sensor* s)
///     {
///         if (s->getType() == U64) {
///             addMappedColumn(s, s->getAlias(), Value::INT, SENSOR_U64);
///         }else{
///             addMappedColumn(s, s->getAlias(), Value::DOUBLE, SENSOR_FLOAT);
///         }
///     }
///
///     // Event raised by Monitor after a Row created
///     void onRowCreate(Row& r)
///     {
///         if (r.tag == FEEDER_PROCESS_ITEM) {
///             setValue(r, NAME, cast<Process>(r)->getName());
///             setValue(r, PID, cast<Process>(r)->getPid());
///         }
///     }
///
///     // Event raised when a row need updated
///     void onRowUpdate(Row& r)
///     {
///         if (r.tag == FEEDER_PROCESS_ITEM) {
///             Process& p = cast<Process>(r);
///             // Update Sensor U64
///             for(ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s) {
///                 setValue(r, s, s->getValue(p.getPid()).U64);
///             }
///             // Update Sensor Float
///             for(ColumnIterator<SENSOR_FLOAT, Sensor> s = this; s != 0; ++s){
///                 setValue(r, s, s->getValue(p.getPid()).Float);
///             }
///         }
///     }
/// };
///
/// int main()
/// {
///     // Model
///     MyMonitor m;
///     ProcessEnumerator pe;
///     // Connect Process enumerator to monitor
///     m.connectRowFeeder(&pe);
///
///     // View
///     TermGridView view(m);
///
///     // Controller
///     TermGridCtrl termCtrl(view);
///
///     // Start Text
///     Console::drawText("My Monitor", 0, 0);
///     Console::drawText("\t(use '1'..'9' to sort, '='+'1'..'9' to sum," +
///                       "'f'+'1'..'9' to fix)", 0, 1);
///
///     // Place the view
///     view.move(0,3);
///     // Set auto size with offset
///     view.setAutoSize(GridView::AutoSize(0,3));
///
///     // Create a style for Sensor
///     GridStyle s;
///     s.setSuffix(" u/s").setAlign(GridStyle::RIGHT);
///
///     // Set style for sensor Column in the view
///     view.setStyle(MyMonitor::SENSOR_U64, s);
///     view.setStyle(MyMonitor::SENSOR_FLOAT, s);
///
///     // Add a Fake Sensor
///     FakeSensor s1;
///     m.addSensor(&s1);
///
///     // Variable to pause monitor and know application running status
///     bool isRunning = true, isFreezed = false;
///
///     // Main Loop
///     while (isRunning) {
///         // Update Monitor
///         if (!isFreezed) {
///             m.update();
///             pe.update();
///         }
///
///         // Handle Terminal Event
///         int c = Console::keyHit();
///         switch (c) {
///         case 0:
///             // = No key hit (keyHit is async method -> return 0 if no key)
///         break;
///         case 'q': case 'Q':
///             // Key for exit the application
///             isRunning = false;
///         break;
///         case 'p': case 'P':
///             // Key to pause the monitor
///             isFreezed = (!isFreezed);
///             // Force render will make needRender return true (see bellow)
///             view.forceRender();
///         break;
///         default:
///             // If other key -> let the terminal controller handle the event
///             termCtrl.handleTerminalEvent(c);
///         }
///
///         // Render
///         if (view.needRender()) {
///
///             // Draw a tips text
///             Console::drawText("Columns count=" +
///                                 Tools::CStr(view.getColumnViewCount()) +
///                                 ", Row count=" +
///                                 Tools::CStr(view.getRowViewCount()) +
///                                 (isFreezed ? " - PAUSED" : "         "),
///                                 0, 2,
///                                 Console::DrawStyle(Color::Yellow));
///
///             // Refresh size of view
///             view.refreshAutoSize();
///             // Finally render the view
///             view.render();
///         }
///
///         // Do a sleep of 50 ms to avoid comsuption of all CPU
///         // with letting the interpretation of key event
///         Tools::sleep_ms(50);
///     };
///
///     return 0;
/// }
/// @endcode
///
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MONITOR_H__
#define LIBEC_MONITOR_H__

#include <iostream>
#include <vector>
#include <map>

#include "../tools/Tools.h"
#include "../value/Value.h"
#include "../grid/model/GridModel.h"
#include "../grid/filter/GridFilter.h"
#include "../grid/view/GridView.h"
#include "../grid/controller/GridCtrl.h"

#include "config/MonitorLoader.h"
#include "config/Loadable.h"
#include "../Globals.h"

namespace cea
{

  /* Prototype */
  class MonitorFeeder;

  /// @brief Class to extend to build the specific monitor
  class Monitor : public GridModel, public MonitorLoader
  {
  public:

    /* Typedefs */
    /// @brief Short name for row
    typedef GridRow Row;
    /// @brief Short name of the column
    typedef GridColumn Column;

    /// @brief Key for row mapping
    typedef void* MapKey;
    /// @brief Column mapped to a Monitorable
    typedef std::map<MapKey, Column*> ColumnMap;
    /// @brief Row mapped to a Monitorable
    typedef std::map<MapKey, Row*> RowMap;

    /// @brief Monitor feeder list
    typedef std::list<MonitorFeeder*> FeederList;

    /// @brief Controller List
    typedef std::list<GridCtrl*> ControllerList;

    /// @ingroup monitor
    /// @brief Cast operator
    ///
    /// This class simplify access to mapped monitorable item
    ///
    /// <h3>Example</h3>
    /// @code
    ///		// Get a FakeSensor from a column
    ///		FakeSensor& s = cast<FakeSensor>(column);
    /// @endcode
    ///
    template<typename T>
      struct cast
      {
        /* Member */
        void* ptr; ///< Generic pointer to cast
        /* Constructor */
        /// @brief Cast a generic pointer of a row
        /// @param r Row containing the generic pointer
        cast(Row& r) :
            ptr(r.getData())
        {
        }
        /// @brief Cast a generic pointer of a column
        /// @param c Column containing the generic pointer
        cast(Column& c) :
            ptr(c.getData())
        {
        }
        /// @brief Cast a generic pointer
        /// @param mem Generic pointer to cast
        cast(void *mem) :
            ptr(mem)
        {
        }
        /* Function */
        /// @brief Use the C++ convert operator to cast the generic pointer
        /// @return Cast result
        operator T&() const
        {
          return *(T*) (ptr);
        }
        /// @brief Implement the C++ -> operator to allow direct
        ///        manipulationcast of the generic pointer
        /// @return Cast result
        T*
        operator->()
        {
          return (T*) (ptr);
        }

      };

    /* Function */
    /// @brief Get frequency of the monitor
    /// @return Update frequency in ms. \n
    ///	        Or 0 if always update.
    cea_time_t
    getFrequency() const;
    /// @brief Set frequency of the monitor
    ///
    /// This frequency control the update of the monitor.
    ///
    /// By default the frequency of the monitor is 1000 ms.
    ///
    /// @param freq Frequency in ms. \n
    ///             If = 0 Then Each call of update will update Monitor. \n
    ///             Else update function will check if is time to update
    ///                  following frequency.
    void
    setFrequency(cea_time_t freq);

    /// @brief Check if update needed
    /// @return true if update needed following the frequency
    virtual bool
    needUpdate() const;

    /// @brief Update all the monitor rows follow the frequency
    ///
    /// If frequency = 0 Then Update all rows at each call \n
    /// Else Update following the frequency of the monitor.
    ///
    virtual void
    update();

    /* Feeder section */
    /// @brief Connect a MonitorFeeder to rows
    /// @param feeder Feeder to connect
    void
    connectRowFeeder(MonitorFeeder* feeder);
    /// @brief Connect a MonitorFeeder to columns
    /// @param feeder Feeder to connect
    void
    connectColumnFeeder(MonitorFeeder* feeder);
    /// @brief Disconnect a MonitorFeeder
    /// @param feeder Feeder to deconnect
    void
    disconnectFeeder(MonitorFeeder* feeder);
    /// @brief Disconnect all connected MonitorFeeder
    void
    clearFeeder();

    /* Controller Section */
    /// @brief Transmit an event to all grid controllers
    /// @param e Event to transmit
    void
    handleEvent(const GridCtrl::Event& e);

    /* View Section */
    /// @brief Render all views
    void
    renderViews();

    /* Friend class */
    friend class MonitorFeeder;
    friend class MonitorLoader;

  protected:

    /* Construtor */
    /// @brief Default constructor
    ///
    /// Set update frequency to 1 second
    ///
    Monitor();
    /// @brief Create a monitor and set his frequency
    ///
    /// @param frequency Frequency in ms. \n
    ///             If = 0 Then Each call of update will update Monitor. \n
    ///             Else update function will check if is time to update
    ///                  following frequency.
    Monitor(cea_time_t frequency);

    /// @brief Disconnect all MonitorFeeder
    virtual
    ~Monitor();

    /* Row/Column mapped section */
    /// @brief Add a Column mapped to an object
    ///
    /// To check if the column was added: test the return with
    /// GridModel::isValid function.
    ///
    /// @param linkObject Generic pointer to map
    /// @param title Title of the column to add
    /// @param type Type of the column to add
    /// @param tag Tag of the column used to identify which kind of object
    ///             the column is mapped to
    /// @param minimumSize Theorical minimum size of the column.
    ///						Normally this value represent minimum size by
    ///						value length.
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    /// @param percentWidth Theorical percent size of the column.
    ///						Normally this value represent percent of a total
    ///						width availible on the view and the unit is
    ///						text char (not pixel).
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    /// @return Reference to the created Column. \n
    ///         Or reference to GridModel::ncolumn if column not added
    Column&
    addMappedColumn(void* linkObject, const std::string& title = "",
        Value::Type type = Value::INT, int tag = -1, unsigned int minimumSize =
            0, unsigned int percentWidth = 0);
    /// @brief Return the corresponding column of a generic pointer
    ///
    /// To check if the return column is valid: test with GridModel::isValid
    /// function.
    ///
    /// @param linkObject Generic pointer considered
    /// @return Column mapped to the given object. \n
    ///         Or GridModel::ncolumn if no column mapped to linkObject.
    /// @see addMappedColumn
    Column&
    getMappedColumn(void* linkObject);

    /// @brief Add a Row mapped to an object
    ///
    /// To check if the row was added: test the return with
    /// GridModel::isValid function.
    ///
    /// @param linkObject Generic pointer to map
    /// @param tag Tag of the row used to identify which kind of object
    ///             the row is mapped to
    /// @return Reference to the created Row. \n
    ///         Or reference to GridModel::nrow if row not added
    Row&
    addMappedRow(void* linkObject, int tag = -1);
    /// @brief Return the corresponding row of a generic pointer
    ///
    /// To check if the return row is valid: test with GridModel::isValid
    /// function.
    ///
    /// @param linkObject Generic pointer considered
    /// @return Row mapped to the given object. \n
    ///         Or GridModel::nrow if no row mapped to linkObject.
    /// @see addMappedRow
    Row&
    getMappedRow(void* linkObject);

    /* Argument section */

    /// @brief Get an arg value of a column
    /// @param col Column considered
    /// @param id Id of arg to get
    /// @return The arg value if exist otherwise GridModel::nvalue.
    /// @see GridItem::setArg
    const Value&
    getArg(const Column& col, unsigned int id = 0);
    /// @brief Get an arg value of a row
    /// @param row Row considered
    /// @param id Id of arg to get
    /// @return The arg value if exist otherwise GridModel::nvalue.
    /// @see GridItem::setArg
    const Value&
    getArg(const Row& row, unsigned int id = 0);

    /// @brief Associate to an arg id of a column a value
    ///
    /// @copydetails GridItem::setArg
    ///
    /// @param col Column considered
    /// @param id Id of arg to set
    /// @param value Value of the arg
    /// @return Reference to the current object
    Monitor&
    setArg(Column& col, unsigned int id, const Value& value);
    /// @brief Associate to an arg id of a row a value
    ///
    /// @copydetails GridItem::setArg
    ///
    /// @param row Row considered
    /// @param id Id of arg to set
    /// @param value Value of the arg
    /// @return Reference to the current object
    Monitor&
    setArg(Row& row, unsigned int id, const Value& value);

    /* Feeder Function section */
    /// @brief Event triggered after a MonitorFeeder meets a new element
    ///
    /// This function by default create a Row and call onRowCreate. \n
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element). \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedCreateRow
    ///         void onFeedCreateRow(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 addMappedRow(obj, tag);
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the new MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the new MonitorFeeder element
    /// @see FeederType, Monitor::onRowCreate
    virtual void
    onFeedCreateRow(int tag, void* obj);
    /// @brief Event triggered after a MonitorFeeder update an element
    ///
    /// This function by default find the associated Row of obj and call
    /// onRowUpdate. \n
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element). \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedUpdateRow
    ///         void onFeedUpdateRow(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 // Find the associated Row
    ///                 Row& r = getMappedRow(obj);
    ///                 // Update the row
    ///                 if (isValid(r)) {
    ///                     // (update code)
    ///                 }
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the updated MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the updated MonitorFeeder element
    /// @see FeederType, Monitor::onRowUpdate
    virtual void
    onFeedUpdateRow(int tag, void* obj);
    /// @brief Event triggered before a MonitorFeeder delete an element
    ///
    /// This function by default find the associated Row of obj and call
    /// onRowDelete then call GridModel::removeRow. \n
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element). \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedDeleteRow
    ///         void onFeedDeleteRow(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 // Find the associated Row
    ///                 Row& r = getMappedRow(obj);
    ///                 // Delete the Row
    ///                 if (isValid(r)) removeRow(r);
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the updated MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the MonitorFeeder element to delete
    /// @see FeederType, Monitor::onRowDelete
    virtual void
    onFeedDeleteRow(int tag, void* obj);

    /// @brief Event triggered after a MonitorFeeder meets a new element
    ///
    /// This function by default create a Column and call onColumnCreate. \n
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element). \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedCreateColumn
    ///         void onFeedCreateColumn(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 Process& p = cast<Process>(obj);
    ///                 addMappedColumn(obj, p.getName(), Value::INT, tag);
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the new MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the new MonitorFeeder element
    /// @see FeederType, Monitor::onColumnCreate
    virtual void
    onFeedCreateColumn(int tag, void* obj);
    /// @brief Event triggered after a MonitorFeeder update an element
    ///
    /// This function by default find the associated Column of obj and call
    /// onColumnUpdate. \n
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element). \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedUpdateColumn
    ///         void onFeedUpdateColumn(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 // Find the associated Column
    ///                 Column& c = getMappedColumn(obj);
    ///                 // Update the column
    ///                 if (isValid(c)) {
    ///                     // (update code)
    ///                 }
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the updated MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the updated MonitorFeeder element
    /// @see FeederType, Monitor::onColumnUpdate
    virtual void
    onFeedUpdateColumn(int tag, void* obj);
    /// @brief Event triggered before a MonitorFeeder delete an element
    ///
    /// This function by default find the associated Column of obj and call
    /// onColumnDelete then call GridModel::removeColumn.
    ///
    /// This function can be overridden (eg. To considerate only certain
    /// MonitorFeeder element).
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onFeedDeleteColumn
    ///         void onFeedDeleteColumn(int tag, void* obj)
    ///         {
    ///             // Accept only process
    ///             if (tag == FEEDER_PROCESS_ITEM) {
    ///                 // Find the associated Column
    ///                 Column& c = getMappedColumn(obj);
    ///                 // Delete the Column
    ///                 if (isValid(c)) removeColumn(c);
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param tag Tag of the updated MonitorFeeder element (see FeederType)
    /// @param obj Generic pointer to the MonitorFeeder element to delete
    /// @see FeederType, Monitor::onColumnDelete
    virtual void
    onFeedDeleteColumn(int tag, void* obj);

    /* Monitor event - section */
    /// @brief Event triggered when update call follow monitor frequency
    ///
    /// This function by default update all the rows which passed the
    /// filter. \n
    ///
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onUpdate
    ///         void onUpdate()
    ///         {
    ///             // Update only sensor Row
    ///             for(RowIterator<FEEDER_SENSOR_ITEM, FakeSensor> s = this
    ///                 ; s != 0; ++s)
    ///             {
    ///                 // Action on sensor
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    virtual void
    onUpdate();

    /// @brief Event triggered after a row is created by a MonitorFeeder
    ///
    /// This function is an important part to build a monitor.
    /// Since its represent the initial value of columns for a row. \n
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onRowCreate
    ///         void onRowCreate(Row& r)
    ///         {
    ///             std::cout << "Row created: tag=" << r.tag << std::endl;
    ///             // Complete initial column value of the row
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param r Row created
    virtual void
    onRowCreate(Row& r);
    /// @brief Event triggered after columns loaded
    ///
    /// This function is an important part to build a monitor.
    /// Since its represent the initial value of columns for a row
    /// after a load of XML file. \n
    ///
    /// This function by default call onRowCreate(Row& r). \n
    /// This function need to be overridden when memory for MonitorFeeder
    /// row is created or specific action in onRowCreate function. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onRowCreate
    ///         void onRowLoaded(Row& r)
    ///         {
    ///             std::cout << "Row loaded: tag=" << r.tag << std::endl;
    ///             // Complete initial column value of the row
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param r Row loaded
    virtual void
    onRowLoaded(Row& r);
    /// @brief Event triggered after a row is updated by a MonitorFeeder
    ///
    /// This function is an important part to build a monitor.
    /// Since its represent the update value of columns of rows. \n
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onRowUpdate
    ///         void onRowUpdate(Row& r)
    ///         {
    ///             std::cout << "Row updated: tag=" << r.tag << std::endl;
    ///             // Update column value of the row
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param r Row updated
    virtual void
    onRowUpdate(Row& r);
    /// @brief Event triggered before a row is deleted by a MonitorFeeder
    ///
    /// This function represent the deletion of memory allocated
    /// for columns values of a row. \n
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Static const representing the differents columns
    ///         static const int VALUE_COL = 0;
    ///
    ///         // Create the columns
    ///         MyMonitor()
    ///         {
    ///             // Add a int Value column
    ///             addColumn("Value", Value::INT, VALUE_COL, 30);
    ///         }
    ///
    ///         // Implementation of onRowCreate
    ///         void onRowCreate(Row& r)
    ///         {
    ///             // Debug infos
    ///             std::cout << "Row created: tag=" << r.tag << std::endl;
    ///
    ///             // Create a memory value
    ///             r.setData(new int);
    ///
    ///             // Set memory value and cell value
    ///             if (r.getData() != 0) {
    ///                 // Memory value = current row position
    ///                 cast<int>(r) = getRowId(r);
    ///                 // Cell Value of Column(VALUE_COL) = memory value
    ///                 setValue(r, VALUE_COL, cast<int>(r));
    ///             }
    ///         }
    ///
    ///         // Implementation of onRowDelete
    ///         void onRowDelete(Row& r)
    ///         {
    ///             // We have entire access to the row and the mapped
    ///             // object since the event is raised before the memory
    ///             // deletion
    ///
    ///             // Debug infos
    ///             std::cout << "Row deleted: tag=" << r.tag << std::endl;
    ///
    ///             // Delete the allocated memory
    ///             if (r.getData() != 0) delete r.getData();
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param r Row deleted
    virtual void
    onRowDelete(Row& r);

    /// @brief Event triggered after a column is created by a MonitorFeeder
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onColumnCreate
    ///         void onColumnCreate(Column& c)
    ///         {
    ///             std::cout << "Column created: tag=" << c.tag
    ///                       << std::endl;
    ///             // Complete initial row/column value of the Column
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param c Column created
    virtual void
    onColumnCreate(Column& c);
    /// @brief Event triggered after a column is updated by a MonitorFeeder
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Implementation of onColumnUpdate
    ///         void onColumnUpdate(Column& c)
    ///         {
    ///             std::cout << "Column update: tag=" << c.tag
    ///                       << std::endl;
    ///             // Update row/column value of the column
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param c Column updated
    virtual void
    onColumnUpdate(Column& c);
    /// @brief Event triggered before a column is deleted by a MonitorFeeder
    ///
    /// This function represent the deletion of memory allocated
    /// for cells values of a column. \n
    ///
    /// This function by default do nothing. \n
    /// This function can be overridden. \n
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Static const representing the differents columns
    ///         static const int VALUE_ROW = 0;
    ///
    ///         // Create the columns
    ///         MyMonitor()
    ///         {
    ///             // Add a Value row
    ///             addRow(VALUE_ROW);
    ///         }
    ///
    ///         // Implementation of onColumnCreate
    ///         void onColumnCreate(Column& c)
    ///         {
    ///             // Debug infos
    ///             std::cout << "Column created: tag=" << r.tag
    ///                       << std::endl;
    ///
    ///             // Create a memory value
    ///             c.setData(new int);
    ///
    ///             // Set memory value and cell value
    ///             if (c.getData() != 0) {
    ///                 // Memory value = current row position
    ///                 cast<int>(r) = getColumnId(c);
    ///                 // Cell Value of Row(VALUE_ROW) = memory value
    ///                 setValue(VALUE_ROW, c, cast<int>(c));
    ///             }
    ///         }
    ///
    ///         // Implementation of onColumnDelete
    ///         void onColumnDelete(Column& c)
    ///         {
    ///             // We have entire access to the column and the mapped
    ///             // object since the event is raised before the memory
    ///             // deletion
    ///
    ///             // Debug infos
    ///             std::cout << "Column deleted: tag=" << r.tag
    ///                       << std::endl;
    ///
    ///             // Delete the allocated memory
    ///             if (c.getData() != 0) delete c.getData();
    ///         }
    ///     };
    /// @endcode
    ///
    /// @param c Column deleted
    virtual void
    onColumnDelete(Column& c);

    /* View Section */
    /// @brief This function is called before renderView
    ///
    /// This function can be used to show text or set style option before
    /// the render of all the views.
    ///
    /// This function by default do nothing.
    ///
    /// @see renderViews
    virtual void
    beforeRenderViews();
    /// @brief This function is called after renderView
    ///
    /// This function can be used to show text or set style option after
    /// the render of all the views.
    ///
    /// This function by default do nothing.
    ///
    /// @see renderViews
    virtual void
    afterRenderViews();

    /// @brief Event triggered before a row is deleted by GridModel
    ///
    /// This function can be overridden but should not.
    ///
    virtual void
    onGridRowDelete(GridRow& r);
    /// @brief Event triggered before all row is deleted by GridModel
    ///
    /// This function can be overridden but should not.
    ///
    virtual void
    onGridRowClear();

    /// @brief Event triggered before a column is deleted by GridModel
    ///
    /// This function can be overridden but should not.
    ///
    virtual void
    onGridColumnDelete(GridColumn& c);
    /// @brief Event triggered before all column is deleted by GridModel
    ///
    /// This function can be overridden but should not.
    ///
    virtual void
    onGridColumnClear();

    /* Loader functions */
    /// @brief Connect loaded view to monitor
    ///
    ///	This function connect loaded view to monitor and add to
    ///		loadedViews list which allow to monitor to use
    ///		renderViews.
    ///
    /// @param loadedView View loaded
    ///	@see Monitor::renderViews
    void
    connectLoadedView(GridView* loadedView);

    /// @brief Connect loaded controller and connect it to a view
    ///
    /// This function connect loaded controller to monitor and
    ///		view and allow to monitor to use handleEvent.
    ///
    /// @param loadedCtrl Controller loaded
    /// @param loadedView View previously loaded
    /// @see Monitor::handleEvent
    void
    connectLoadedController(GridCtrl* loadedCtrl, GridView& loadedView);

    /// @brief This function is call by MonitorFeeder to deconnect this
    ///        monitor.
    ///
    /// This function is called only on MonitorFeeder destructor
    ///
    /// @see MonitorFeeder::~MonitorFeeder
    void
    autoDisconnectFeeder(MonitorFeeder* feeder);

    /* Protected - Members */

    ControllerList loadedControllers; ///< List of controllers loaded
    ViewList loadedViews; ///< List of view loaded

    /// @brief Map of column with their generic pointer data
    ColumnMap mappedColumns;

    /// @brief Map of row with their generic pointer data
    RowMap mappedRows;

    /// @brief List of MonitorFeeder connected to rows
    FeederList _rowFeeders;
    /// @brief List of MonitorFeeder connected to columns
    FeederList _columnFeeders;

  private:

    /* Private - Members */
    /// @brief Frequency in millisecond of update function
    ///
    /// If 0 always update.
    /// By default=1000ms
    ///
    cea_time_t _frequency;
    cea_time_t _lastTick; ///< Last time of update call

    /* Private - Function */
    /// @brief This function is used only to get a ref to Monitor
    /// @return Reference of current Monitor
    Monitor&
    getMonitorRef();

    /// @brief Generic tag iterator class
    template<typename T, typename LST, int TAG, typename CAST>
      class monitoriterator
      {
      private:
        /* Members */
        /// @brief Contains the considered list
        LST& lst;
        /// @brief Range of item with same tag
        std::pair<typename LST::iterator, typename LST::iterator> ret;
        /// @brief The iterator used
        typename LST::iterator it;

      public:
        /* Constructor */
        /// @brief Create a "tag" iterator for a list
        /// @param l List considered
        monitoriterator(LST& l) :
            lst(l)
        {
          ret = lst.equal_range(TAG);
          it = ret.first;
        }
        /// @brief Create a "tag" iterator from a same "tag" iterator
        /// @param i "tag" iterator to copy
        monitoriterator(const monitoriterator& i) :
            lst(i.lst), ret(i.ret), it(i.it)
        {
          ;
        }

        /* Operator */
        /// @brief Get data of an item casted into CAST type
        /// @return CAST reference to the data of an item
        CAST&
        operator*()
        {
          return (CAST*) (it->second->getData());
        }

        /// @brief Get data of an item casted into CAST type
        /// @return CAST pointer to the data of an item
        CAST*
        operator->()
        {
          return (CAST*) (it->second->getData());
        }
        /// @brief Get corresponding row or column of the iterator
        /// @return Corresponding item of the iterator
        operator T&()
        {
          return *(it->second);
        }
        /// @brief Increment the iterator
        /// 
        /// Get next "tag" item in the list.
        ///
        /// @return A reference to the current object
        monitoriterator&
        operator++()
        {
          ++it;
          return *this;
        }
        /// @brief Check if reach the end of the corresponding "tag" item
        /// @param v Ignored used only to allow the usage of != C++ operator
        /// @return true if reach the end of the corresponding "tag" item
        bool
        operator!=(int v)
        {
          return (it != ret.second);
        }
      };

  public:

    /// @brief Column "tag" Iterator
    ///
    /// This iterator allow to select and directly cast only certain item
    /// follow their tag. \n
    ///
    /// This iterator is optimized and should be used if not all
    /// columns/rows of a column/row need to be updated. \n
    ///
    /// If all rows/columns need to be updated then use classic iterator
    /// of GridModel::columns or GridModel::rows member.
    ///
    /// <h3>Example</h3>
    /// @code
    ///     class MyMonitor : public Monitor
    ///     {
    ///         // Code for columns, rows ...
    ///         void onRowUpdate(Row& r)
    ///         {
    ///             if (r.tag == FEEDER_PROCESS_ITEM) {
    ///                 Process& p = cast<Process>(r);
    ///                 // Update Sensor U64
    ///                 for(ColumnIterator<SENSOR_U64, Sensor> s = this;
    ///                                                         s != 0; ++s)
    ///                 {
    ///                     setValue(r, s, s->getValue(p.getPid()).U64);
    ///                 }
    ///
    ///                 // Update Sensor Float
    ///                 for(ColumnIterator<SENSOR_FLOAT, Sensor> s = this;
    ///                                                         s != 0; ++s)
    ///                 {
    ///                     setValue(r, s, s->getValue(p.getPid()).Float);
    ///                 }
    ///             }
    ///         }
    ///     };
    /// @endcode
    ///
    template<int TAG, typename CAST>
      class ColumnIterator : public monitoriterator<GridColumn, ColumnTagMap,
          TAG, CAST>
      {
      public:
        /// @brief Create a column "tag" iterator
        /// @param m Monitor considered (generally egal to this)
        ColumnIterator(Monitor* m) :
            monitoriterator<GridColumn, ColumnTagMap, TAG, CAST>(m->columnTags)
        {
        }
      };

    /// @brief Row "tag" Iterator
    ///
    /// @copydetails ColumnIterator
    ///
    template<int TAG, typename CAST>
      class RowIterator : public monitoriterator<GridRow, RowTagMap, TAG, CAST>
      {
      public:
        /// @brief Create a row "tag" iterator
        /// @param m Monitor considered (generally egal to this)
        RowIterator(Monitor* m) :
            monitoriterator<GridRow, RowTagMap, TAG, CAST>(m->rowTags)
        {
        }
      };

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Monitor
///	@ingroup monitor
///
/// <h3>Example of monitor</h3>
/// This example show how to create a monitor of running process 
/// and show it in a terminal view. \n
/// This code works on Linux and Windows without changing anything.
/// 
/// @code
///    
/// #include <libec/monitors.h>
/// #include <libec/process.h>
/// #include <libec/sensors.h>
/// 
/// 
/// // This using namespace is only to keep the example as short as possible
/// // but in C++ is better to avoid it.
/// using namespace cea;
/// 
/// 
/// class MyMonitor : public Monitor
/// {
/// public:
///     // Column tags
///     static const int NAME = 0;
///     static const int PID = 1;
///     static const int SENSOR_U64 = 2;
///     static const int SENSOR_FLOAT = 3;
/// 
///     // Constructor
///     MonitorA()
///     {
///         addColumn("Name", Value::STRING, NAME,30,30);
///         addColumn("PID", Value::INT, PID);
///     };
/// 
///     // Add Sensor column
///     void addSensor(Sensor* s)
///     {
///         if (s->getType() == U64) {
///             addMappedColumn(s, s->getAlias(), Value::INT, SENSOR_U64);
///         }else{
///             addMappedColumn(s, s->getAlias(), Value::DOUBLE, SENSOR_FLOAT);
///         }
///     }
/// 
///     // Event raised by Monitor after a Row created
///     void onRowCreate(Row& r)
///     {
///         if (r.tag == FEEDER_PROCESS_ITEM) {
///             setValue(r, NAME, cast<Process>(r)->getName());
///             setValue(r, PID, cast<Process>(r)->getPid());
///         }
///     }
/// 
///     // Event raised when a row need updated
///     void onRowUpdate(Row& r)
///     {
///         if (r.tag == FEEDER_PROCESS_ITEM) {
///             Process& p = cast<Process>(r);
///             // Update Sensor U64
///             for(ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s) {
///                 setValue(r, s, s->getValue(p.getPid()).U64);
///             }
///             // Update Sensor Float
///             for(ColumnIterator<SENSOR_FLOAT, Sensor> s = this; s != 0; ++s){
///                 setValue(r, s, s->getValue(p.getPid()).Float);
///             }
///         }
///     }
/// 
/// };
/// 
/// int main()
/// {
///   // Model
///   MyMonitor m;
///   ProcessEnumerator pe;
///   // Connect Process enumerator to monitor
///   m.connectRowFeeder(&pe);
/// 
///   // View
///   TermGridView view(m);
/// 
///   // Controller
///   TermGridCtrl termCtrl(view);
/// 
///   // Start Text
///   Console::drawText("My Monitor", 0, 0);
///   Console::drawText("\t(use '1'..'9' to sort, '='+'1'..'9' to sum, " +
///                     "'f'+'1'..'9' to fix)", 0, 1);
///
///   // Place the view
///   view.move(0,3);
///   // Set auto size with offset
///   view.setAutosize(GridView::AutoSize(0,3));
/// 
///   // Create a style for Sensor
///   GridStyle s;
///   s.setSuffix(" u/s").setAlign(GridStyle::RIGHT);
/// 
///   // Set style for sensor Column in the view
///   view.setStyle(MyMonitor::SENSOR_U64, s);
///   view.setStyle(MyMonitor::SENSOR_FLOAT, s);
/// 
///   // Add a Fake Sensor
///   FakeSensor s1;
///   m.addSensor(&s1);
/// 
///   // Variable to pause monitor and know application running status
///   bool isRunning = true, isFreezed = false;
/// 
///   // Main Loop
///   while (bIsRunning) {
///     // Update Monitor
///     if (!isFreezed) {
///       m.update();
///       pe.update();
///     }
/// 
///     // Handle Terminal Event
///     int c = Console::keyHit();
///     switch (c) {
///       case 0:
///         // = No key hit (keyHit is async method -> return 0 if no key)
///         break;
///       case 'q': case 'Q':
///         // Key for exit the application
///         isRunning = false;
///         break;
///       case 'p': case 'P':
///         // Key to pause the monitor
///         isFreezed = (!bFreezeMonitor);
///         // Force render will make needRender return true (see bellow)
///         view.forceRender();
///         break;
///       default:
///         // If other key -> let the terminal controller handle the event
///         termCtrl.handleTerminalEvent(c);
///     }
/// 
///     // Render
///     if (view.needRender()) {
///       // Draw a tips text
///       Console::drawText("Columns count=" +
///                         Tools::CStr(view.getColumnViewCount()) +
///                         ", Row count=" +
///                         Tools::CStr(view.getRowViewCount()) +
///                         (isFreezed ? " - PAUSED" : "         "),
///                         0, 2,
///                         Console::DrawStyle(Color::Yellow));
/// 
///       // Refresh size of view
///       view.refreshAutoSize();
///       // Finally render the view
///       view.render();
///     }
/// 
///     // Do a sleep of 50 ms to avoid comsuption of all CPU
///     // with letting the interpretation of key event
///     Tools::sleep_ms(50);
///   };
/// 
///   return 0;
/// }
/// @endcode
///////////////////////////////////////////////////////////////////////////////
