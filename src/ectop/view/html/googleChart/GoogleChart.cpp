#include "GoogleChart.h"
#include "../../MonitorEctop.h"
#include <libec/grid/model/GridModel.h>
#include <libec/sensor/SensorPid.h>
#include <libec/monitor/Monitor.h>

GoogleChart::GoogleChart(cea::GridModel& model)
{
  _model = &model;

  createCSS();
  createJSON();
  createHTML();
}

void
GoogleChart::createCSS()
{
  std::ofstream ofs("/var/www/html/ectop/style.css");

  // >> i/o operations here <<
  ofs << "body {"
      "font-family: Tahoma, Arial, Verdana; "
      "font-size: 12px; color: black; }" << std::endl;
  // unvisited link
  ofs << "a:link { text-decoration:none;}" << std::endl;
  // visited link
  ofs << "a:visited { text-decoration:none;}" << std::endl;
  // mouse over link
  ofs << "a:hover { text-decoration:underline;}" << std::endl;
  // selected link
  ofs << "a:active { text-decoration:underline;}" << std::endl;

  ofs.flush();
  ofs.close();
}

void
GoogleChart::createHTML()
{
  const int max = 10;
  std::ofstream ofs("/var/www/html/ectop/index.html");

  ofs << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "
      "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" << std::endl;
  ofs << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << std::endl;
  ofs << "  <head>" << std::endl;
  ofs << "    <meta http-equiv=\"content-type\" "
      "content=\"text/html; charset=utf-8\"/>" << std::endl;
  ofs << "    <title>ectop - Web</title>" << std::endl;
  ofs << "    <link rel=\"stylesheet\" href=\"style.css\" "
      "type=\"text/css\">" << std::endl;
  ofs << "    <script type=\"text/javascript\" "
      "src=\"http://www.google.com/jsapi\"></script>" << std::endl;
  // If local copy of jquery exists, use it
//  ofs << "    <script src=\"http://code.jquery.com/jquery-1.8.2.min.js\">"
//      "</script>" << std::endl;
  // Els use the version from jquery's website
  ofs << "    <script src=\"jquery-1.8.2.min.js\">"
      "</script>" << std::endl;
  ofs << "    <script type=\"text/javascript\">" << std::endl;
  ofs << "      google.load('visualization', '1', "
      "{packages: ['corechart']});" << std::endl;
  ofs << "    </script>" << std::endl;
  ofs << "    <script type=\"text/javascript\">" << std::endl;
  ofs << "      var timeEvent;" << std::endl;
  ofs << "      var t = jQuery.parseJSON('[ "
      "[\"Command\", \"S1\", \"S2\" ], "
      "[\"migration/0\", 11, 11], "
      "[\"kthreadd\", 2, 1 ], "
      "[\"ksoftirqd/0\", 2, 11] ]');" << std::endl;
  ofs << "      var data = google.visualization.arrayToDataTable(t);"
      << std::endl;
  ofs << "      var optEctop = {" << std::endl;
  ofs << "        title: 'Top Energy Consumming Processes'," << std::endl;
  ofs << "        width:600," << std::endl;
  ofs << "        height:400 };" << std::endl;
  ofs << "      var optPie = {" << std::endl;
  ofs << "        width:600," << std::endl;
  ofs << "        height:400," << std::endl;
  ofs << "        sliceVisibilityThreshold:0.05 };" << std::endl;
  ofs << "      var optBar = {" << std::endl;
  ofs << "        width:600," << std::endl;
  ofs << "        height:400 };" << std::endl;
  ofs << "" << std::endl;
  ofs << "      jQuery.ajaxSetup ( { cache: false } );" << std::endl;
  ofs << "" << std::endl;
  ofs << "      function startTimeEvent(millisenconds) {" << std::endl;
  ofs << "        timeEvent  = setInterval(function()" << std::endl;
  ofs << "          { updateData() },millisenconds); }" << std::endl;
  ofs << "" << std::endl;
  ofs << "      function stopTimeEvent() { clearInterval (timeEvent); }"
      << std::endl;
  ofs << "" << std::endl;
  ofs << "      function tog(layer_ref) { $(layer_ref).toggle(\"fast\"); }"
      << std::endl;
  ofs << "" << std::endl;
  ofs << "      function updateData() {" << std::endl;
  ofs << "        jQuery.getJSON('data.json', function(json) {" << std::endl;
  ofs << "          console.log(json.table);" << std::endl;
  ofs << "          t = json.table;" << std::endl;
  ofs << "          data = google.visualization.arrayToDataTable(json.table);"
      << std::endl;
  ofs << "          drawVisualization();" << std::endl;
  ofs << "        });" << std::endl;
  ofs << "      }" << std::endl;
  ofs << "" << std::endl;
  // Create and populate the data table
  ofs << "      function drawVisualization() {" << std::endl;
  ofs << "        var barView = new google.visualization.DataView(data);"
      << std::endl;
  ofs << "        var pieView = new google.visualization.DataView(data);"
      << std::endl;
  ofs << std::endl;
  ofs << "        if (t.length > " << max << ") barView.setRows(0, " << max - 1
      << ");" << std::endl;
  ofs << "" << std::endl;
  /*  ofs << "        barView.setColumns([1, 2]);" << std::endl;
   ofs << "" << std::endl;*/
  ofs << "        new google.visualization.BarChart(document." << std::endl;
  ofs << "          getElementById('ectop_div')).draw(barView, optEctop);"
      << std::endl;

  int c_id = 0;
  // Create and draw the visualization for each sensor
  for (unsigned int i = 0; i < _model->getColumnCount(); i++)
    {
      cea::GridColumn gc = _model->getColumn(i);
      if ((gc.tag == cea::MonitorEctop::SENSOR_U64)
          || (gc.tag == cea::MonitorEctop::SENSOR_FLOAT))
        {
          c_id++;
          cea::PIDSensor& s = cea::Monitor::cast<cea::PIDSensor>(gc);
          s.getAlias();

          ofs << std::endl;
          ofs << "        pieView.setColumns([0, " << c_id << "]);"
              << std::endl;
          ofs << "        optPie.title = '" << s.getAlias() << "';"
              << std::endl;
          ofs << "        new google.visualization.PieChart(document."
              << std::endl;
          ofs << "          getElementById('s" << c_id
              << "_pie')).draw(pieView, optPie);" << std::endl;
          ofs << "        barView.setColumns([0," << c_id << "]);" << std::endl;
          ofs << "        new google.visualization.BarChart(document."
              << std::endl;
          ofs << "          getElementById('s" << c_id
              << "_bar')).draw(barView, optBar);" << std::endl;
        }
    }
  ofs << "      }" << std::endl;
  ofs << "      google.setOnLoadCallback(drawVisualization);" << std::endl;
  ofs << "    </script>" << std::endl;
  ofs << "  </head>" << std::endl;
  ofs << "  <body style=\"font-family: Arial;border: 0 none;\">" << std::endl;
  ofs << "    Refresh rate (miliseconds)<input type='text' value='1000' />"
      << std::endl;
  ofs
      << "    <input type='button' value='start' onclick='startTimeEvent(1000)' />"
      << std::endl;
  ofs << "    <input type='button' value='stop' onclick='stopTimeEvent()' />"
      << std::endl;
  ofs << "    <p>" << std::endl;
  ofs << "      <a href='#' onclick='tog(\"#ectop_div\");'>Summary</a>"
      << std::endl;

  // Menu for each sensor ..
  c_id = 0;
  for (unsigned int i = 0; i < _model->getColumnCount(); i++)
    {
      cea::GridColumn gc = _model->getColumn(i);
      if ((gc.tag == cea::MonitorEctop::SENSOR_U64)
          || (gc.tag == cea::MonitorEctop::SENSOR_FLOAT))
        {
          c_id++;
          cea::PIDSensor& s = cea::Monitor::cast<cea::PIDSensor>(gc);
          s.getAlias();
          ofs << "    | <a href='#' onclick='tog(\"#s" << c_id << "_div\");'>"
              << s.getAlias() << "</a>" << std::endl;
        }
    }
  ofs << "    </p>" << std::endl;
  ofs << "    <div id=\"ectop_div\"></div>" << std::endl;

  c_id = 0;
  for (unsigned int i = 0; i < _model->getColumnCount(); i++)
    {
      cea::GridColumn gc = _model->getColumn(i);
      if ((gc.tag == cea::MonitorEctop::SENSOR_U64)
          || (gc.tag == cea::MonitorEctop::SENSOR_FLOAT))
        {
          c_id++;
          ofs << "    <div id='s" << c_id << "_div'>" << std::endl;
          ofs << "      <table>" << std::endl;
          ofs << "        <td><div id='s" << c_id << "_pie'></div></td>";
          ofs << std::endl;
          ofs << "        <td><div id='s" << c_id << "_bar'></div></td>";
          ofs << std::endl;
          ofs << "      </table>" << std::endl;
          ofs << "    </div>" << std::endl;
        }
    }
  ofs << "  </body>" << std::endl;
  ofs << "</html>" << std::endl;

  ofs.flush();
  ofs.close();
}

void
GoogleChart::createJSON()
{
  std::ofstream ofs("/var/www/html/ectop/data.json");

  ofs << "{"
      "  \"columns\": [\"Command\", \"S1\", \"S2\" ],"
      "  \"table\":[ "
      "    [\"Command\", \"S1\", \"S2\" ],"
      "    [\"migration/0\", 11, 11],"
      "    [\"init\", 2, 1 ],"
      "    [\"kthreadd\", 6, 3 ],"
      "    [\"watchdog/0\", 2, 10 ],"
      "    [\"migration/1\", 2, 1 ],"
      "    [\"ksoftirqd/2\", 2, 1 ],"
      "    [\"ksoftirqd/0\", 2, 11],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ],"
      "    [\"kthreadd\", 2, 1 ]"
      "  ]"
      "}";
  ofs.flush();
  ofs.close();
}
