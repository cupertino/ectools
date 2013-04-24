#include "../../MonitorEctop.h"
#include "nvd3HtmlView.h"

#include <libec/tools.h>
#include <libec/grid.h>

namespace cea
{
  /* -------------------------------------------------------------------------- */
  /* Constructor */

  /** +constructor */
  nvd3HtmlView::nvd3HtmlView()
  {
    _model = NULL;
    _JsonHeader.assign("{ \"table\": [");
    _JsonEnd.assign("] }");
  }
  nvd3HtmlView::nvd3HtmlView(GridModel& model) :
      GridView(model)
  {
    _model = &model;

    _JsonHeader.assign("{ \"table\": [");
    _JsonEnd.assign("] }");
  }

  void
  nvd3HtmlView::createStaticFiles(std::string path)
  {
    createHtml(path);
    createCss(path);
    createJS(path);
  }

  void
  nvd3HtmlView::createJson(std::string path)
  {

  }

  void
  nvd3HtmlView::createJS(std::string path)
  {
    std::ofstream ofs;

    path.append("/js/ectop.nv.d3.js");
    ofs.open(path.c_str());

    ofs << "var ectop_data;" << std::endl;
    ofs << "var barChart;" << std::endl;
    ofs << "var chart_pie = [];" << std::endl;
    ofs << "var pie_width = 300;" << std::endl;
    ofs << "var pie_height = 300;" << std::endl;
    ofs << "var firstPie = 1;" << std::endl;
    ofs << "" << std::endl;
    ofs << "jQuery.ajaxSetup ( { cache: false } );" << std::endl;
    ofs << "" << std::endl;
    ofs << "function drawPieChart(id) {" << std::endl;
    ofs << "  d3.select('#s' + id)" << std::endl;
    ofs << "    .datum([ ectop_data[id] ])" << std::endl;
    ofs << "    .transition().duration(1000)" << std::endl;
    ofs << "    .attr('width', pie_width)" << std::endl;
    ofs << "    .attr('height', pie_height)" << std::endl;
    ofs << "    .call(chart_pie[id]);" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "function createPieChart(id) {" << std::endl;
    ofs << "  chart_pie[id] = nv.models.pieChart()" << std::endl;
    ofs << "    .x(function(d) { return d.label })" << std::endl;
    ofs << "    .y(function(d) { return d.value })" << std::endl;
    ofs << "    .showLabels(false)" << std::endl;
    ofs << "    .showLegends(false)" << std::endl;
    ofs << "    .color(d3.scale.category10().range())" << std::endl;
    ofs << "    .labelThreshold(0.5)" << std::endl;
    ofs << "    .width(pie_width)" << std::endl;
    ofs << "    .height(pie_height);" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "function drawBarChart() {" << std::endl;
    ofs << "  d3.select('#barChart')" << std::endl;
    ofs << "    .datum(ectop_data)" << std::endl;
    ofs << "    .transition().duration(500)" << std::endl;
    ofs << "    .call(barChart);" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "nv.addGraph(function() {" << std::endl;
    ofs << "  barChart = nv.models.multiBarHorizontalChart()" << std::endl;
    ofs << "    .x(function(d) { return d.label })" << std::endl;
    ofs << "    .y(function(d) { return d.value })" << std::endl;
    ofs << "    .margin({top: 30, right: 20, bottom: 50, left: 175})"
        << std::endl;
    ofs << "    .showValues(false)" << std::endl;
    ofs << "    .tooltips(true)" << std::endl;
    ofs << "    .showControls(false);" << std::endl;
    ofs << "" << std::endl;
    ofs << "  barChart.yAxis" << std::endl;
    ofs << "    .tickFormat(d3.format(',.2f'));" << std::endl;
    ofs << "" << std::endl;
    ofs << "  drawBarChart();" << std::endl;
    ofs << " " << std::endl;
    ofs << "  nv.utils.windowResize(barChart.update);" << std::endl;
    ofs << "" << std::endl;
    ofs << "  return barChart;" << std::endl;
    ofs << "});" << std::endl;
    ofs << "" << std::endl;
    ofs << "function updateData() {" << std::endl;
    ofs << "  jQuery.getJSON('data.json', function(json) {" << std::endl;
    ofs << "    console.log(json);" << std::endl;
    ofs << "    ectop_data = json;" << std::endl;
    ofs << "  });" << std::endl;
    ofs << "  " << std::endl;
    ofs << "  drawBarChart();" << std::endl;
    ofs << "      " << std::endl;
    ofs << "  var max = ectop_data.length;" << std::endl;
    ofs << "  var i;" << std::endl;
    ofs << "  if(firstPie == 1)" << std::endl;
    ofs << "  {" << std::endl;
    ofs << "    for(i=0; i<max; i=i+1) {" << std::endl;
    ofs << "      createPieChart(i);" << std::endl;
    ofs << "    }" << std::endl;
    ofs << "    firstPie = 0;" << std::endl;
    ofs << "  }" << std::endl;
    ofs << "  for(i=0; i<max; i=i+1) {" << std::endl;
    ofs << "    drawPieChart(i);" << std::endl;
    ofs << "  }" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "function startTimeEvent(millisenconds) {" << std::endl;
    ofs << "  timeEvent  = setInterval(function(){ " << std::endl;
    ofs << "    updateData() },millisenconds); }" << std::endl;
    ofs << "" << std::endl;
    ofs << "function stopTimeEvent() { clearInterval (timeEvent); }"
        << std::endl;
    ofs << "" << std::endl;
    ofs << "function tog(layer_ref) { $(layer_ref).toggle(\"fast\"); }"
        << std::endl;

    ofs.close();
  }

  void
  nvd3HtmlView::createCss(std::string path)
  {
    std::ofstream ofs;

    path.append("/css/ectop.nv.d3.css");
    ofs.open(path.c_str());

    ofs << "body {" << std::endl;
    ofs << "  overflow-y:scroll;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "text {" << std::endl;
    ofs << "  font: 12px sans-serif;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "a {font: 12px sans-serif;}" << std::endl;
    ofs << "a:link {text-decoration:none;}" << std::endl;
    ofs << "a:visited {text-decoration:none;}" << std::endl;
    ofs << "a:hover {text-decoration:underline;}" << std::endl;
    ofs << "a:active {text-decoration:underline;}" << std::endl;
    ofs << "" << std::endl;
    ofs << "#barChart {" << std::endl;
    ofs << "  margin: 10px;" << std::endl;
    ofs << "  min-width: 100px;" << std::endl;
    ofs << "  min-height: 100px;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << "#barChart svg {" << std::endl;
    ofs << "  height: 500px;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << ".pieChart {" << std::endl;
    ofs << "  height: 300px;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << ".pieHeader {" << std::endl;
    ofs << "  text-align:center;" << std::endl;
    ofs << "  font: 18px sans-serif;" << std::endl;
    ofs << "  font-weight:bold;" << std::endl;
    ofs << "}" << std::endl;
    ofs << "" << std::endl;
    ofs << ".pieDiv {" << std::endl;
    ofs << "  float:left;" << std::endl;
    ofs << "  width: 300px;" << std::endl;
    ofs << "}" << std::endl;

    ofs.close();
  }

  void
  nvd3HtmlView::createHtml(std::string path)
  {
    std::ofstream ofs;

    path.append("/index.html");
    ofs.open(path.c_str());

    ofs << "<html>" << std::endl;
    ofs << "  <head>" << std::endl;
    ofs << "    <meta charset=\"utf-8\">" << std::endl;
    ofs << "    <title>ectop</title>" << std::endl;
    ofs << "" << std::endl;
    ofs
        << "    <link href=\"css/nv.d3.css\" rel=\"stylesheet\" type=\"text/css\">"
        << std::endl;
    ofs
        << "    <link href=\"css/ectop.nv.d3.css\" rel=\"stylesheet\" type=\"text/css\">"
        << std::endl;
    ofs << "    " << std::endl;
    ofs << "    <script src=\"js/jquery-1.8.2.min.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/d3.v2.min.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/nv.d3.min.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/utils.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/tooltip.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/legend.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/axis.js\"></script>" << std::endl;
    ofs << "    <script src=\"js/multiBarHorizontal.js\"></script>"
        << std::endl;
    ofs << "    <script src=\"js/multiBarHorizontalChart.js\"></script>"
        << std::endl;
    ofs << "" << std::endl;
    ofs << "    <script src=\"js/ectop.nv.d3.js\"></script>" << std::endl;
    ofs << "  </head>" << std::endl;
    ofs << "" << std::endl;
    ofs << "  <body>" << std::endl;
    ofs << "    <p>" << std::endl;
    ofs
        << "      <input type='button' value='start' onclick='startTimeEvent(1000)'/>"
        << std::endl;
    ofs << "      <input type='button' value='stop' onclick='stopTimeEvent()'/>"
        << std::endl;
    ofs << "    </p>" << std::endl;
    ofs << "    <p>" << std::endl;
    ofs << "      <a href='#' onclick='tog(\"#ectop_div\");'>Summary</a>"
        << std::endl;

    //creates one link to toggle each sensor's div
    int id = 0;
    for (unsigned int i = 0; i < _model->getColumnCount(); i++)
      {
        cea::GridColumn gc = _model->getColumn(i);
        if ((gc.tag == cea::MonitorEctop::SENSOR_U64)
            || (gc.tag == cea::MonitorEctop::SENSOR_FLOAT))
          {
            cea::PIDSensor& s = cea::Monitor::cast<cea::PIDSensor>(gc);
            ofs << "    | <a href='#' onclick='tog(\"#s" << id << "_div\");'>"
                << s.getAlias() << " </a>" << std::endl;
            id++;
          }
      }
    ofs << "    </p>" << std::endl;
    ofs << std::endl;
    ofs << "    <div align=\"justify\" id=\"ectop_div\">" << std::endl;
    ofs << "      <svg id=\"barChart\"></svg>" << std::endl;
    ofs << "    </div>" << std::endl << std::endl;

    id = 0;
    for (unsigned int i = 0; i < _model->getColumnCount(); i++)
      {
        cea::GridColumn gc = _model->getColumn(i);
        if ((gc.tag == cea::MonitorEctop::SENSOR_U64)
            || (gc.tag == cea::MonitorEctop::SENSOR_FLOAT))
          {
            cea::PIDSensor& s = cea::Monitor::cast<cea::PIDSensor>(gc);
            ofs << "    <div class=\"pieDiv\" id=\"s" << id << "_div\">"
                << std::endl;
            ofs << "      <p class=\"pieHeader\">" << s.getAlias() << "</p>"
                << std::endl;
            ofs << "      <svg class=\"pieChart\" id=\"s" << id << "\"></svg>"
                << std::endl;
            ofs << "    </div>" << std::endl;
            id++;
          }
      }

    ofs << "  </body>" << std::endl;
    ofs << "</html>" << std::endl;

    ofs.close();
  }

  /* -------------------------------------------------------------------------- */
  /* Render Method */

  /** +renderColumnHeader */
  void
  nvd3HtmlView::renderColumnHeader(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Column Header */
    _Json.append("\"" + text + "\", ");
  }
  /** +renderRow */
  void
  nvd3HtmlView::renderRow(const GridView::RowView& row, int rowId, int x, int y,
      int viewWidth)
  {
    /* Render Row */
    _Json.append("], [");
  }

  /** +renderCell */
  void
  nvd3HtmlView::renderCell(const GridCell& c, int rowId, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Cell */
    if (Tools::isNumeric(text))
      _Json.append(text + ", ");
    else
      _Json.append("\"" + text + "\", ");
  }

  /** +renderSumBar */
  void
  nvd3HtmlView::renderSumBar(int x, int y, int viewWidth)
  {
  }
  /** +renderSumBarItem */
  void
  nvd3HtmlView::renderSumBarItem(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
  }

  /** #beforeRender */
  void
  nvd3HtmlView::beforeRender()
  {
    _Json.assign(" [ ");
  }

  /** #afterRender */
  void
  nvd3HtmlView::afterRender(int viewWidth, int viewHeight)
  {
    _Json.append(" ] ");
    /* Save the file if outputFile set */
    if (!(_outputFile.empty()))
      {
        saveToFile(_outputFile);
      }
  }

  /** #getShowText */
  void
  nvd3HtmlView::getShowText(const std::string& txt, unsigned int colSize,
      const GridStyle& style, std::string& retText)
  {
    /* Set Main text */
    retText.assign(txt);
    /* Add prefix and Suffix */
    if (!(style.prefix.empty()))
      {
        retText.insert(0, style.prefix);
      }
    if (!(style.suffix.empty()))
      {
        retText.append(style.suffix);
      }
  }

  /** +setJsonSource */
  void
  nvd3HtmlView::setJsonSource(const std::string& header, const std::string& end)
  {
    _JsonHeader.assign(header);
    _JsonEnd.assign(end);
  }

  /** +setOutput */
  void
  nvd3HtmlView::setOutput(const std::string& outputFile)
  {
    _outputFile.assign(outputFile);
  }

  /** +getOutput */
  std::string
  nvd3HtmlView::getOutput() const
  {
    return _outputFile;
  }

  /** +getContent */
  std::string
  nvd3HtmlView::getContent() const
  {
    return _Json;
  }

  /** +saveToFile */bool
  nvd3HtmlView::saveToFile(const std::string& outputFile)
  {
    bool fileSaved = false;

    std::ofstream f;
    /* Open the file */
    f.open(outputFile.c_str(), std::ios::out);
    /* Flush data */
    if ((f.is_open()) && (f.good()))
      {
        /* Write data */
        f << _JsonHeader;
        f << _Json;
        f << _JsonEnd;
        /* File saved */
        fileSaved = true;
      }
    /* Close the file */
    f.close();

    /* Return true if file saved correctly */
    return fileSaved;
  }

  /* -------------------------------------------------------------------------- */
  /* Load Method */

  /** +loadSpecificXML */
  void
  nvd3HtmlView::loadSpecificXML(GridModel& model, XMLReader &xml,
      const XMLReader::Balise &balise)
  {
    const XMLReader::Balise* b = balise.getChild("Json-header");
    if (b != 0)
      {
        _JsonHeader.assign(b->text);
      }
    b = balise.getChild("Json-footer");
    if (b != 0)
      {
        _JsonEnd.assign(b->text);
      }
    _outputFile.assign(balise.getBaliseAttr("output", "file"));
  }

}

