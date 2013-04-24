#include "JsonGridView.h"
#include <libec/tools.h>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /* Constructor */

  /** +constructor */
  JsonGridView::JsonGridView()
  {
    _JsonHeader.assign("{ \"table\": [");
    _JsonEnd.assign("] }");
  }
  JsonGridView::JsonGridView(GridModel& model) :
      GridView(model)
  {
    _JsonHeader.assign("{ \"table\": [");
    _JsonEnd.assign("] }");
  }

  /* -------------------------------------------------------------------------- */
  /* Render Method */

  /** +renderColumnHeader */
  void
  JsonGridView::renderColumnHeader(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Render Column Header */
    _Json.append("\"" + text + "\", ");
  }
  /** +renderRow */
  void
  JsonGridView::renderRow(const GridView::RowView& row, int rowId, int x, int y,
      int viewWidth)
  {
    /* Render Row */
    _Json.append("], [");
  }

  /** +renderCell */
  void
  JsonGridView::renderCell(const GridCell& c, int rowId, int colId,
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
  JsonGridView::renderSumBar(int x, int y, int viewWidth)
  {
    /* Render Column Header */
//    _Json.append("</tr>\n\t<tr class='sum'>");
  }
  /** +renderSumBarItem */
  void
  JsonGridView::renderSumBarItem(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
//    /* Render Sum Bar */
//    _Json.append("<td>");
//    _Json.append(style.prefix);
//    _Json.append(text);
//    _Json.append(style.suffix);
//    _Json.append("</td>");
  }

  /** #beforeRender */
  void
  JsonGridView::beforeRender()
  {
    _Json.assign(" [ ");
  }

  /** #afterRender */
  void
  JsonGridView::afterRender(int viewWidth, int viewHeight)
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
  JsonGridView::getShowText(const std::string& txt, unsigned int colSize,
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
  JsonGridView::setJsonSource(const std::string& header, const std::string& end)
  {
    _JsonHeader.assign(header);
    _JsonEnd.assign(end);
  }

  /** +setOutput */
  void
  JsonGridView::setOutput(const std::string& outputFile)
  {
    _outputFile.assign(outputFile);
  }

  /** +getOutput */
  std::string
  JsonGridView::getOutput() const
  {
    return _outputFile;
  }

  /** +getContent */
  std::string
  JsonGridView::getContent() const
  {
    return _Json;
  }

  /** +saveToFile */
  bool
  JsonGridView::saveToFile(const std::string& outputFile)
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
  JsonGridView::loadSpecificXML(GridModel& model, XMLReader &xml,
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

