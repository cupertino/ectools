#include <libec/Globals.h>
#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/sensors.h>
// This using namespace is only to keep the example as short as possible
// but in C++ is better to avoid it.
using namespace cea;

// Store the default monitor into this string
const std::string DefaultMonitor =
    " \
<monitor> \
	<!-- Column Tag --> \
	<!-- 0 == Name --> \
	<!-- 1 == PID --> \
	<!-- 2 == Sensor --> \
	 \
	 \
	<freq value='2000' /> \
	 \
	<column name='Name' type='string' size='30' tag='0' /> \
	<column name='PID' type='int' size='10' tag='1' /> \
	\
	<column name='S1' type='int' size='10' tag='2' class='sensor' object='fakesensor'/>  \
</monitor> \
 \
<view type='term' ctrl='term'> \
 \
	<style id='1' align='Center' suffix='%' color='#ff0000' /> \
	<style id='2' align='RIGHT' suffix=' s' color='#00FF00' /> \
	 \
	<style id='100' backcolor='#FFFF00' color='#ff0000'/> \
	<style id='50' backcolor='#008000' color='#ff0000'/> \
	\
	<style-filter filter='(Name == 'PSPad.exe', 'eclib.exe')' style='100' /> \
	<sort by='Name' order='asc' /> \
	 \
	<sum columns='S1' /> \
	 \
	 \
	<position x='0' y='3' offseth='3' offsetw='0'/> \
	 \
</view>";

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
};

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

  // Load monitor follow command line parameters
  if (argc < 2)
    {
      // Load default monitor
      m.loadString(DefaultMonitor);
    }
  else
    {
      // Load monitor of param 1
      m.loadFile(argv[1]);
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
        // Refresh - load config file
        m.loadFile(configFile);
        break;
      default:
        // Create a Terminal KeyHit GridCtrl event
        GridCtrl::Event e(GridCtrl::Event::TERM_KEYHIT, c);

        // If other key -> let the monitor controllers handle the event
        m.handleEvent(e);
        break;
        }
      // Render Views
      m.renderViews();
      // Do a sleep of 50 ms to avoid comsuption of all CPU
      // with letting the interpretation of key event
      Tools::sleep_ms(50);
    };
  return 0;
}
