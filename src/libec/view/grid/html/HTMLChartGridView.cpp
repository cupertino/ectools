#include <libec/grid/view/html/HTMLChartGridView.h>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /* Constructor */

  /** +constructor */
  HTMLChartGridView::HTMLChartGridView() :
      _keyColumn(0), _valueColumn(0)
  {
    _htmlHeader.assign(
        "<!doctype html>\n<html>\n \
						\t<head>\n \
						\t\t<title>Auto-generated page by libec</title>\n \
						\t\t<link rel='stylesheet' type='text/css' \
									href='cea_chart.css' />\n \
						\t</head>\n \
						\t<body>\n");
    _htmlEnd.assign("\n\t</body>\n</html>");
  }
  HTMLChartGridView::HTMLChartGridView(GridModel& model) :
      HTMLGridView(model), _keyColumn(0), _valueColumn(0)
  {
    _htmlHeader.assign(
        "<!doctype html>\n<html>\n \
						\t<head>\n \
						\t\t<title>Auto-generated page by libec</title>\n \
						\t\t<link rel='stylesheet' type='text/css' \
									href='cea_chart.css' />\n \
						\t</head>\n \
						\t<body>\n");
    _htmlEnd.assign("\n\t</body>\n</html>");
  }

  /* -------------------------------------------------------------------------- */
  /* Render Method */

  /** +renderColumnHeader */
  void
  HTMLChartGridView::renderColumnHeader(const GridView::ColumnView& col,
      int colId, const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    if (col.ref == _keyColumn)
      {
        addChartHeaderKey(text);
      }
    else if (col.ref == _valueColumn)
      {
        addChartHeaderValue(text);
      }
  }
  /** +renderRow */
  void
  HTMLChartGridView::renderRow(const GridView::RowView& row, int rowId, int x,
      int y, int viewWidth)
  {
    /* DO NOTHING - by default */;
  }
  /** +renderCell */
  void
  HTMLChartGridView::renderCell(const GridCell& c, int rowId, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    if (&(c.getColumn()) == _keyColumn)
      {
        addChartKey(text);
      }
    else if (&(c.getColumn()) == _valueColumn)
      {
        addChartValue(text);
      }
  }

  /** +renderSumBar */
  void
  HTMLChartGridView::renderSumBar(int x, int y, int viewWidth)
  {
    /* DO NOTHING - by default */;
  }
  /** +renderSumBarItem */
  void
  HTMLChartGridView::renderSumBarItem(const GridView::ColumnView& col,
      int colId, const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* DO NOTHING - by default */;
  }

  /** #beforeRender */
  void
  HTMLChartGridView::beforeRender()
  {
    _html.clear();
  }

  /** #afterRender */
  void
  HTMLChartGridView::afterRender(int viewWidth, int viewHeight)
  {
    /* Save the file if outputFile set */
    if (!(_outputFile.empty()))
      {
        saveToFile (_outputFile);
      }
  }

  /** #getShowText */
  void
  HTMLChartGridView::getShowText(const std::string& txt, unsigned int colSize,
      const GridStyle& style, std::string& retText)
  {
    /* Set Main text */
    retText.assign(txt);
  }

  /* -------------------------------------------------------------------------- */
  /* Pie chart Method */

  /** +setChartColumn */
  void
  HTMLChartGridView::setChartColumn(const GridColumn& colTitle,
      const GridColumn& colData)
  {
    _keyColumn = &colTitle;
    _valueColumn = &colData;
  }

  /** #addChartHeaderKey */
  void
  HTMLChartGridView::addChartHeaderKey(const std::string& txt)
  {
    /* DO NOTHING - by default */;
  }
  /** #addChartHeaderValue */
  void
  HTMLChartGridView::addChartHeaderValue(const std::string& txt)
  {
    /* DO NOTHING - by default */;
  }
  /** #addChartKey */
  void
  HTMLChartGridView::addChartKey(const std::string& txt)
  {
    /* DO NOTHING - by default */;
  }
  /** #addChartValue */
  void
  HTMLChartGridView::addChartValue(const std::string& txt)
  {
    /* DO NOTHING - by default */;
  }

  /* -------------------------------------------------------------------------- */
  /* Load Method */

  /** +loadSpecificXML */
  void
  HTMLChartGridView::loadSpecificXML(GridModel& model, XMLReader &xml,
      const XMLReader::Balise &balise)
  {
    /* Load html property */
    HTMLGridView::loadSpecificXML(model, xml, balise);

    /* Get chart property */
    const XMLReader::Balise* b = balise.getChild("chart");
    if (b == 0)
      {
        _keyColumn = 0;
        _valueColumn = 0;
      }
    else
      {
        /* Get the GridColumn */
        GridColumn& colKey = model.getGridColumnByName(b->getAttr("key"));
        GridColumn& colValue = model.getGridColumnByName(b->getAttr("value"));
        /* Set chart column */
        if (model.isValid(colKey))
          {
            _keyColumn = &colKey;
          }
        else
          {
            _keyColumn = 0;
          }
        if (model.isValid(colValue))
          {
            _valueColumn = &colValue;
          }
        else
          {
            _valueColumn = 0;
          }
      }
  }

}
