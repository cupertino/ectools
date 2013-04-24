#include <iostream>
#include <string>

#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/sensors.h>

//#ifdef __FD_SETSIZE
//#undef __FD_SETSIZE
//#endif
//#define __FD_SETSIZE  65535

using namespace cea;

/* -------------------------------------------------------------------------- */
/* Pie Chart */

class amPieChart : public HTMLChartGridView
{
public:
  /** constructor */
  amPieChart(GridModel& model) :
      HTMLChartGridView(model)
  {
    _htmlHeader =
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \
					\"http://www.w3.org/TR/html4/strict.dtd\"><html><head> \
					<meta http-equiv=\"Content-Type\" content=\"text/html; \
					charset=utf-8\"><title>Libec - Pie chart</title><link \
					rel=\"stylesheet\" href=\"cea_chart.css\" \
					type=\"text/css\"><script src=\"amcharts/amcharts.js\" \
					type=\"text/javascript\"></script> \n \
					<script type=\"text/javascript\">\n var chart; \
					var chartData = [";
    itemCount = 0;
  }

protected:

  void
  afterRender(bool renderAllItem)
  {
    _htmlEnd =
        "]; \
					AmCharts.ready(function () { \
					chart = new AmCharts.AmPieChart(); \
					chart.addTitle(\"Libec - Pie chart\", 16); \
					chart.dataProvider = chartData; \
					chart.titleField = \""
            + headerTitle + "\"; \
					chart.valueField = \"" + headerData
            + "\"; \
					chart.sequencedAnimation = false; \
					chart.startEffect = \"elastic\"; \
					chart.innerRadius = \"30%\"; \
					chart.startDuration = 0.5; \
					chart.labelRadius = 15; \
					chart.depth3D = 10; \
					chart.angle = 15; \
					chart.write(\"chartdiv\"); \
					}); \
					</script> \
					</head><body><div id='chartdiv' \
					style='width:100%; height:800px;'></div></body></html>";
    itemCount = 0;
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

/* -------------------------------------------------------------------------- */
/* Monitor */

class MonitorA : public Monitor
{
public:
  /* Possible Column */
  static const int PID = 0;
  static const int NAME = 1;
  static const int CPU = 2;
  static const int SENSOR_U64 = 3;
  static const int SENSOR_FLOAT = 4;
  static const int REALVALUE_ROW = 50;

  /* Constructor */
  MonitorA()
  {
    addColumn("PID", Value::INT, PID);
    addColumn("Name", Value::STRING, NAME, 30, 30);

    addRow(REALVALUE_ROW).setSum(false).setValue(1, 58080808).setFixed(0, true);

  }

  /* Function to add Sensor */
  void
  addSensor(Sensor* s)
  {
    std::string name = s->getAlias();
    if (s->getType() == U64)
      {
        addMappedColumn(s, name, Value::INT, SENSOR_U64);
      }
    else
      {
        addMappedColumn(s, name, Value::DOUBLE, SENSOR_FLOAT);
      }
  }

  /* Row Creation */
  void
  onRowCreate(Row& r)
  {
    switch (r.tag)
      {
    case PROCESS:
      setValue(r, NAME, cast<Process>(r)->getName());
      setValue(r, PID, cast<Process>(r)->getPid());
      break;
      }
  }

  /* Can be overriden - fired when a row is updated */
  void
  onRowUpdate(Row& r)
  {
    if (r.tag == PROCESS)
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
    else if (r.tag == REALVALUE_ROW)
      {
        /* Update Sensor U64 */
        //for(ColumnIterator<SENSOR_U64, Sensor> s = this; s != 0; ++s) {
        //	setValue(r, s, getTotal( s->getId()));
        //}
      }
  }

};

/* -------------------------------------------------------------------------- */
/* Main */

int
main()
{

  DebugLog::create();
  DebugLog::clear();

  /* Model */
  MonitorA m;
  ProcessEnumerator pe;
  //pe.setCalculUsageCPU(true);
  m.connectRowFeeder(&pe);
  /* View */
  TermGridView view(m);
  TermGridCtrl termCtrl(view);

  /* HTML View */
  HTMLGridView html(m);
  TermGridCtrl htmlCtrl(html);
  amPieChart pieHtml(m);
  TermGridCtrl pieHtmlCtrl(pieHtml);

  pieHtml.setChartColumn(m.getColumn(0), m.getColumn(1));

  /* View Config */
  Console::drawText("ectop  - " + Tools::nowToString(), 0, 0);
  Console::drawText(
      "\t(use '1'..'9' to sort, '='+'1'..'9' to sum, 'c' to config)", 0, 1);
  view.move(0, 3);
  view.setStyle(3, GridStyle().setAlign(GridStyle::RIGHT));
  view.setStyle(4, GridStyle().setAlign(GridStyle::RIGHT));
//  view.setStyle(50, GridStyle().setColor(Color::Green, 255));
  html.move(0, 3);
  pieHtml.move(0, 3);

  /* Log */
  //view.setLog(FileLog("out.log"),XMLFormat(true));
  /* Add Sensor - here */
  std::cout << __FD_SETSIZE << std::endl;
  std::cout << pe.getProcessCount() << std::endl;

  PerfCount* pc;
//  for (int i = 0; i < PERF_COUNT_HW_MAX; i++)
  for (int i = 0; i < 5; i++)
    {
      pc = new PerfCount(0, i);
      if (pc->getStatus() == true)
        m.addSensor(pc);
    }

  struct timeval tv, tv_p;
  gettimeofday(&tv, NULL);
  usleep(1000 * 1000 - tv.tv_usec);

  /* Main Loop */bool bIsRunning = true, bFreezeMonitor = false;
  while (bIsRunning)
    {
      gettimeofday(&tv_p, NULL);

      /* Update Monitor */
      if (!bFreezeMonitor)
        {
          m.update();
          pe.update();
        }

      /* Event */
      int c = Console::keyHit();
      switch (c)
        {
      case 0:
        break;
      case 'q':
      case 'Q':
        bIsRunning = false;
        break;
      case 'p':
      case 'P':
        bFreezeMonitor = (!bFreezeMonitor);
        view.forceRender();
        break;
      case 'f':
        view.fixColumn(1, false, 0);
        break;
      case 'u':
        view.unfixColumn(1);
        break;
      default:
        termCtrl.handleTerminalEvent(c);
        htmlCtrl.handleTerminalEvent(c);
        pieHtmlCtrl.handleTerminalEvent(c);
        }

      /* Render */
      if (view.needRender())
        {

          Console::Size sz = Console::getSize();
          std::string freeze;
          if (bFreezeMonitor)
            {
              Console::drawText("PAUSED - " + Tools::nowToString(), 0, 0);
            }
          else
            {
              Console::drawText("ectop  - " + Tools::nowToString(), 0, 0);
            }

          view.size(sz.width - view.getLeft(), sz.height - view.getTop());
          view.render();

          gettimeofday(&tv, NULL);
          Console::drawText(
              "debug>> pid: " + Tools::CStr(getpid()) + ", tasks:"
                  + Tools::CStr(view.getRowViewCount()) + ", render time(s):"
                  + Tools::CStr(
                      (tv.tv_sec - tv_p.tv_sec)
                          + (float) ((tv.tv_usec - tv_p.tv_usec)) / 1000000)
                  + "   ", 0, 2, Console::DrawStyle(Color::Yellow));
        }
      if (html.needRender())
        {
          html.render();
          html.saveToFile("html_view/data.html");
        }
      if (pieHtml.needRender())
        {
          pieHtml.render();
          pieHtml.saveToFile("html_view/chart.html");
        }

//      gettimeofday(&tv, NULL);
//      usleep(1000 * 1000 - tv.tv_usec);

      Tools::sleep_ms(50);

    };

  return 0;

}
