#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/grid.h>

#include <libec/sensor/FakeSensor.h>
#include <libec/tools.h>

using namespace cea;

/* -------------------------------------------------------------------------- */
/* Pie Chart */

class amChart : public HTMLChartGridView
{
public:
  /** constructor */
  amChart()
  {
    itemCount = 0;
  }

  amChart(GridModel& model) :
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

class MonitorB : public Monitor
{
public:
  /// Element Tag
  static const int NAME = 0;
  static const int PID = 1;
  static const int SENSOR_U64 = 2;
  static const int SENSOR_FLOAT = 3;

  bool isFreezed;

  MonitorB() :
      isFreezed(false)
  {
    // Add loader
    loadMapMonitorable<FakeSensor>("sensor", "FakeSensor");
    loadMapView<amChart>("amChart");
  }

  void
  onRowCreate(Row& r)
  {
    switch (r.tag)
      {
    case FEEDER_PROCESS_ITEM:
      setValue(r, NAME, cast<Process>(r)->getName());
      setValue(r, PID, cast<Process>(r)->getPid());
      break;
      }
  }

  void
  onRowUpdate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        Process& p = cast<Process>(r);
        /* Update Sensor U64 */
        for (ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).U64);
          }
        /* Update Sensor Float **/
        for (ColumnIterator<SENSOR_FLOAT, Sensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValue(p.getPid()).Float);
          }
      }
  }

};

int
main()
{
  bool isRunning = true;
  MonitorB m;
  ProcessEnumerator pe;
  m.connectRowFeeder(&pe);

  m.loadFile("monitorB.xml");

  while (isRunning)
    {

      /* Update if not freezed */
      if (!m.isFreezed)
        {
          pe.update();
          m.update();
        }

      /* Handle Event */
      int k = Console::keyHit();
      switch (k)
        {
      case 0:
        /* No Event */
        break;
      case 'q':
      case 'Q':
        isRunning = false;
        break;
      case 'p':
        m.isFreezed = !m.isFreezed;
        break;
      case 'r':
        // Refresh
        m.loadFile("monitorB.xml");
      default:
        m.handleEvent(GridCtrl::Event(GridCtrl::Event::TERM_KEYHIT, k));
        }

      /* Render Views */
      m.renderViews();

      /* Sleep 50 ms to get key from terminal and not comsume all CPU */
      Tools::sleep_ms(50);
    }

  return 0;
}
