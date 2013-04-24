#include <libec/grid/view/terminal/TermGridView.h>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /* Constructor */

  /** +constructor */
  TermGridView::TermGridView()
  {
    _renderAllItem = false;
  }
  TermGridView::TermGridView(GridModel& model) :
      GridView(model)
  {
    _renderAllItem = false;
  }

  /* -------------------------------------------------------------------------- */
  /* Position/Size Method */

  void
  TermGridView::refreshAutoSize()
  {
    int w, h;
    if (_autoSize.autoWidth)
      {
        w = Console::getSize().width - _autoSize.offsetW;
      }
    else
      {
        w = _width;
      }
    if (_autoSize.autoHeight)
      {
        h = Console::getSize().height - _autoSize.offsetH;
      }
    else
      {
        h = _height;
      }
    if ((w != _width) || (h != _height))
      {
        size(w, h);
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Render Method */

  /** +renderColumnHeader */
  void
  TermGridView::renderColumnHeader(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Set Sort order tips */
    if ((_sortEnabled) && (_sortRefColumn == col.ref))
      {
        std::string tmp(text);
        /* Draw order tips */
        if (tmp.length() > 2)
          {
            tmp[tmp.length() - 2] = ((_sortOrder == ASCENDING) ? '^' : 'v');
          }
        /* Draw text */
        Console::drawText(tmp, x, y, style);
      }
    else
      {
        Console::drawText(text, x, y, style);
      }
  }
  /** +renderRow */
  void
  TermGridView::renderRow(const GridView::RowView& row, int rowId, int x, int y,
      int viewWidth)
  {
    if ((rowId == 0) && (_headerAdditionalLine))
      {
        std::string line;
        line.resize(viewWidth, '=');
        Console::drawText(line, x, y, GridView::defaultColumnHeaderStyle);
      }
  }
  /** +renderCell */
  void
  TermGridView::renderCell(const GridCell& c, int rowId, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    Console::drawText(text, x, y, style);
  }

  /** +renderSumBar */
  void
  TermGridView::renderSumBar(int x, int y, int viewWidth)
  {
    std::string line;
    line.resize(viewWidth, (char) ('_'));
    Console::drawText(line, x, y, GridView::defaultColumnSumStyle);
  }
  /** +renderSumBarItem */
  void
  TermGridView::renderSumBarItem(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    Console::drawText(text, x, y, style);
  }

  /** #afterRender */
  void
  TermGridView::afterRender(int viewWidth, int viewHeight)
  {
    /* Clear width */
    int w = _width - viewWidth;
    if (w > 0)
      {
        Console::setColor(-1, -1);
        Console::clear(left + viewWidth, top, w, _height + 5);
      }
    /* Clear height */
    int h = _height - viewHeight;
    if (h > 0)
      {
        Console::setColor(-1, -1);
        Console::clear(left, top + viewHeight, _width, h);
      }
  }

}
