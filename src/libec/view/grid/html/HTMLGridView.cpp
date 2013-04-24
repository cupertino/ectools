#include <libec/grid/view/html/HTMLGridView.h>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /* Constructor */

  /** +constructor */
  HTMLGridView::HTMLGridView()
  {
    _htmlHeader.assign(
        "<!doctype html>\n<html>\n"
	"  <head>\n"
	"    <title>Auto-generated page by libec</title>\n"
	"    <link rel='stylesheet' type='text/css' href='style.css' />\n"
	"  </head>\n"
	"  <body>\n");
    _htmlEnd.assign("\n  </body>\n</html>");
  }
  HTMLGridView::HTMLGridView(GridModel& model) :
      GridView(model)
  {
    _htmlHeader.assign(
        "<!doctype html>\n<html>\n"
	"  <head>\n"
	"    <title>Auto-generated page by libec</title>\n"
	"    <link rel='stylesheet' type='text/css' href='style.css' />\n"
	"  </head>\n"
	"  <body>\n");
    _htmlEnd.assign("\n  </body>\n</html>");
  }

  /* -------------------------------------------------------------------------- */
  /* Render Method */

  /** +renderColumnHeader */
  void
  HTMLGridView::renderColumnHeader(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Column Header */
    _html.append("<th>");
    _html.append(style.prefix);
    _html.append(text);
    _html.append(style.suffix);
    _html.append("</th>");
  }
  /** +renderRow */
  void
  HTMLGridView::renderRow(const GridView::RowView& row, int rowId, int x, int y,
      int viewWidth)
  {
    /* Render Row */
    _html.append("</tr>\n\t<tr>");
  }
  /** +renderCell */
  void
  HTMLGridView::renderCell(const GridCell& c, int rowId, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Cell */
    _html.append("<td>");
    _html.append(style.prefix);
    _html.append(text);
    _html.append(style.suffix);
    _html.append("</td>");
  }

  /** +renderSumBar */
  void
  HTMLGridView::renderSumBar(int x, int y, int viewWidth)
  {
    /* Render Column Header */
    _html.append("</tr>\n\t<tr class='sum'>");
  }
  /** +renderSumBarItem */
  void
  HTMLGridView::renderSumBarItem(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Sum Bar */
    _html.append("<td>");
    _html.append(style.prefix);
    _html.append(text);
    _html.append(style.suffix);
    _html.append("</td>");
  }

  /** #beforeRender */
  void
  HTMLGridView::beforeRender()
  {
    _html.assign("<table>\n\t<tr>");
  }

  /** #afterRender */
  void
  HTMLGridView::afterRender(int viewWidth, int viewHeight)
  {
    _html.append("</tr>\n</table>");
    /* Save the file if outputFile set */
    if (!(_outputFile.empty()))
      {
        saveToFile (_outputFile);
      }
  }

  /** #getShowText */
  void
  HTMLGridView::getShowText(const std::string& txt, unsigned int colSize,
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

  /** +setHTMLSource */
  void
  HTMLGridView::setHTMLSource(const std::string& header, const std::string& end)
  {
    _htmlHeader.assign(header);
    _htmlEnd.assign(end);
  }

  /** +setOutput */
  void
  HTMLGridView::setOutput(const std::string& outputFile)
  {
    _outputFile.assign(outputFile);
  }

  /** +getOutput */
  std::string
  HTMLGridView::getOutput() const
  {
    return _outputFile;
  }

  /** +getContent */
  std::string
  HTMLGridView::getContent() const
  {
    return _html;
  }

  /** +saveToFile */
  bool
  HTMLGridView::saveToFile(const std::string& outputFile)
  {
    bool fileSaved = false;

    std::ofstream f;
    /* Open the file */
    f.open(outputFile.c_str(), std::ios::out);
    /* Flush data */
    if ((f.is_open()) && (f.good()))
      {
        /* Write data */
        f << _htmlHeader;
        f << _html;
        f << _htmlEnd;
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
  HTMLGridView::loadSpecificXML(GridModel& model, XMLReader &xml,
      const XMLReader::Balise &balise)
  {
    const XMLReader::Balise* b = balise.getChild("html-header");
    if (b != 0)
      {
        _htmlHeader.assign(b->text);
      }
    b = balise.getChild("html-footer");
    if (b != 0)
      {
        _htmlEnd.assign(b->text);
      }
    _outputFile.assign(balise.getBaliseAttr("output", "file"));
  }

}

