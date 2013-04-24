#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/sensors.h>
#include <typeinfo>
// This using namespace is only to keep the example as short as possible
// but in C++ is better to avoid it.
using namespace cea;

/* -------------------------------------------------------------------------- */
/* Pie Chart */

class amPieChart : public HTMLChartGridView
{
public:
  /** constructor */
  amPieChart()
  {
    itemCount = 0;
  }

  amPieChart(GridModel& model) :
      HTMLChartGridView(model)
  {
    itemCount = 0;
  }

protected:

  void
  afterRender(int viewWidth, int viewHeight)
  {
    itemCount = 0;
    HTMLChartGridView::afterRender(viewWidth, viewHeight);
  }

  void
  addChartKey(const std::string& txt)
  {
    if (itemCount < 10)
      {
        _html.append("{" + headerTitle + ": \"" + txt + "\"");
      }
  }
  void
  addChartValue(const std::string& txt)
  {
    if (itemCount < 10)
      {
        _html.append("," + headerData + ": \"" + txt + "\"},");
        itemCount++;
      }
  }

  void
  addChartHeaderKey(const std::string& txt)
  {
    headerTitle = txt;
  }
  void
  addChartHeaderValue(const std::string& txt)
  {
    headerData = txt;
  }

  /* Member */
  std::string headerTitle;
  std::string headerData;
  unsigned int itemCount;

};

class MyMonitor : public Monitor
{
public:
  // Column tags
  static const int NAME = 0;
  static const int PID = 1;
  static const int SENSOR_U64 = 2;
  static const int SENSOR_FLOAT = 3;
  static const int TEST_FLOAT = 4;

  // Member
  bool isFreezed;

  // Constructor
  MyMonitor() :
      isFreezed(false)
  {
    // Add loader
    loadMapMonitorable<FakeSensor>("sensor", "FakeSensor");
    loadMapView<amPieChart>("amPieChart");
  }

  // Default Load
  void
  defaultLoad()
  {
    addColumn("Name", Value::STRING, NAME, 30, 30);
    addColumn("PID", Value::INT, PID);
    // You can also add sensor here or bellow in loadMonitor function
  }
  ;

  // Add Sensor column
  void
  addSensor(Sensor* s)
  {
    if (s->getType() == U64)
      {
        addMappedColumn(s, s->getAlias(), Value::INT, SENSOR_U64);
      }
    else
      {
        addMappedColumn(s, s->getAlias(), Value::DOUBLE, SENSOR_FLOAT);
      }
  }

  // Event raised by Monitor after a Row created
  void
  onRowCreate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        setValue(r, NAME, cast<Process>(r)->getName());
        setValue(r, PID, cast<Process>(r)->getPid());
      }
  }

  // Event raised when a row need updated
  void
  onRowUpdate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        Process& p = cast<Process>(r);
        // Update Sensor U64
        for (ColumnIterator<SENSOR_U64, PIDSensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).U64);
          }
        // Update Sensor Float
        for (ColumnIterator<SENSOR_FLOAT, PIDSensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).Float);
          }
      }
  }

  // Check if a TermGridView is loaded
  bool
  isTermGridViewLoaded() const
  {
    // Search if one of the loaded view is Terminal or classic view
    for (ViewList::const_iterator v = loadedViews.begin();
        v != loadedViews.end(); ++v)
      {
        if ((typeid(*(*v)) == typeid(GridView))
            || (typeid(*(*v)) == typeid(TermGridView)))
          {
            return true;
          }
      }
    // Return false if no corresponding view found
    return false;
  }

  // Get a TermGridView
  GridView*
  getMainConsoleGridView()
  {
    // Search if one of the loaded view is Terminal or classic view
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((typeid(*(*v)) == typeid(GridView))
            || (typeid(*(*v)) == typeid(TermGridView)))
          {
            return *v;
          }
      }
    // Return false if no corresponding view found
    return 0;
  }

  // Before RenderViews
  void
  beforeRenderViews()
  {
    // Get the current terminal gridview
    GridView* v = getMainConsoleGridView();
    if (v == 0)
      return;
    // Draw a tips text
    Console::drawText(
        "Columns count=" + Tools::CStr(v->getColumnViewCount()) + ", Row count="
            + Tools::CStr(v->getRowViewCount())
            + (isFreezed ? " - PAUSED" : "         "), 0, 2,
        Console::DrawStyle(Color::Yellow));

  }

};

// Global Sensor
FakeSensor fakeSensor1;

// Load a monitor
bool
loadMonitor(MyMonitor& m, GridView& defaultView, const std::string& file)
{
  m.clear();
  m.loadFile(file);
  if (m.isTermGridViewLoaded())
    {
      m.removeView(defaultView);
      return true;
    }
  else
    {
      m.connectView(defaultView);
      m.defaultLoad();
      // Add your default sensor here
      m.addSensor(&fakeSensor1);

      return false;
    }
}

int
main(int argc, char* argv[])
{
  // Model
  MyMonitor m;
  ProcessEnumerator pe;
  // Connect Process enumerator to monitor
  m.connectRowFeeder(&pe);

  // Store the configuration file of command line parameter
  // By default set to "ectop.xml"
  std::string configFile = "ectop.xml";

  // Used to know if default view loaded
  bool defaultViewUsed = true;
  // Default view
  TermGridView defaultView;
  TermGridCtrl defaultViewCtrl(defaultView);
  // Place the defaultView
  defaultView.move(0, 3);
  // Set auto size with offset
  defaultView.setAutoSize(GridView::AutoSize(0, 3));
  // Create a style for Sensor
  GridStyle s;
  s.setSuffix(" u/s").setAlign(GridStyle::RIGHT);
  // Set style for sensor Column in the defaultView
  defaultView.setStyle(MyMonitor::SENSOR_U64, s);
  defaultView.setStyle(MyMonitor::SENSOR_FLOAT, s);

  // Load monitor follow command line parameters
  if (argc < 2)
    {
      // Load default monitor
      defaultViewUsed = (!loadMonitor(m, defaultView, ""));
    }
  else
    {
      // Load monitor of param 1
      defaultViewUsed = (!loadMonitor(m, defaultView, argv[1]));
      // Keep the file loaded
      configFile.assign(argv[1]);

    }

  // Variable to know application running status
  bool isRunning = true;
  // Main Loop
  while (isRunning)
    {

      // Update Monitor
      if (!m.isFreezed)
        {
          m.update();
          pe.update();
        }

      // Handle Terminal Event
      int c = Console::keyHit();
      switch (c)
        {
      case 0:
        // = No key hit (keyHit is async method -> return 0 if no key)
        break;
      case 'q':
      case 'Q':
        // Key for exit the application
        isRunning = false;
        break;
      case 'p':
      case 'P':
        // Key to pause the monitor
        m.isFreezed = !m.isFreezed;
        break;

      case 'r':
      case 'R':
        // Refresh
        defaultViewUsed = (!loadMonitor(m, defaultView, configFile));
        break;
      default:

        // If other key -> let the monitor controllers handle the event
        if (defaultViewUsed)
          {
            defaultViewCtrl.handleTerminalEvent(c);
          }
        else
          {
            // Create a Terminal KeyHit GridCtrl event
            GridCtrl::Event e(GridCtrl::Event::TERM_KEYHIT, c);

            m.handleEvent(e);
          }
        break;
        }

      // Render
      if ((defaultViewUsed) && (defaultView.needRender()))
        {

          // Draw a tips text
          m.beforeRenderViews();

          // Refresh size of defaultView
          defaultView.refreshAutoSize();
          // Finally render the defaultView
          defaultView.render();
        }
      else
        {

          // Render Views
          m.renderViews();

        }

      // Do a sleep of 50 ms to avoid comsuption of all CPU
      // with letting the interpretation of key event
      Tools::sleep_ms(50);

    };
  return 0;
}
