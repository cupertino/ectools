#include <libec/grid/log/GridLog.h>
#include <libec/grid/view/GridView.h>
#include <libec/grid/model/GridRow.h>
#include <libec/grid/model/GridColumn.h>
#include <libec/grid/model/GridCell.h>
#include <libec/grid/model/GridModel.h>

namespace cea
{

  /** +Constructor */
  GridLog::GridLog()
  {
    ;
  }
  GridLog::GridLog(const BaseLog& logType) :
      Log(logType)
  {
    ;
  }
  GridLog::GridLog(const BaseLog& logType, const BaseFormat& logFormat) :
      Log(logType, logFormat)
  {
    ;
  }

  /** +log */
  void
  GridLog::log(GridRow* row, FormatBlockType type)
  {
    /* Check if row exist */
    if (row != 0)
      {
        if ((_filter == 0) || (_filter->applyFilter(*row)))
          {
            /* Open Block */
            openBlockType(type);
            /* Log all the row cells */
            for (GridRow::CellMap::iterator it = row->cells_begin();
                it != row->cells_end(); ++it)
              {
                if (it->second != 0)
                  {
                    GridCell& c = *(it->second);
                    /* Log the Cell */
                    write(c.getColumn().getTitle(), c.get());
                  }
              }
            /* Close Block */
            closeBlockType(type);
          }
      }
  }

  /** +log */
  void
  GridLog::log(GridView* view, GridView::RowView* row, FormatBlockType type)
  {
    /* Check if row exist */
    if ((row != 0) && (view != 0) && (row->ref != 0))
      {
        if ((_filter == 0) || (_filter->applyFilter(*(row->ref))))
          {
            /* Open Block */
            openBlockType(type);
            /* Get Iterator */
            GridView::ColumnView* cFirst = view->getFirstColumnView();
            GridView::ColumnTable::iterator c = 0;
            if (cFirst != 0)
              c = cFirst->it;
            /* Log all the row cells visible on view */
            for (; c != 0; c = c->next)
              {
                if ((c->value != 0) && (c->value->visible))
                  {
                    /* Get cell */
                    const Value& v = row->ref->getCell(*(c->value->ref)).get();
                    write(c->value->ref->getTitle(), v);
                  }
              }
            /* Close Block */
            closeBlockType(type);
          }
      }
  }

  /** +log */
  void
  GridLog::log(GridView* view)
  {
    /* Check if view exist */
    if (view != 0)
      {
        /* Open Grid Block */
        openBlock("grid");
        /* Add Comment */
        std::string cmt = "Entire log of grid on ";
        cmt.append(Tools::nowToString());
        addComment(cmt);
        /* Get Iterator */
        GridView::RowView* rFirst = view->getFirstRowView();
        GridView::RowTable::iterator r = 0;
        if (rFirst != 0)
          r = rFirst->it;
        /* Log all the row */
        for (; r != 0; r = r->next)
          {
            if ((r->value->visible)
                && ((_filter == 0) || (_filter->applyFilter(*(r->value->ref)))))
              {
                log(view, r->value);
              }
          }
        /* Close Grid Block */
        closeBlock("grid");
      }
  }

  /** +log */
  void
  GridLog::log(GridColumn* col)
  {
    /* Check if column exist */
    if (col != 0)
      {
        /* Open Block */
        openBlock("column");
        /* Add Comment */
        std::string cmt = "Entire log of column <";
        cmt.append(col->getTitle());
        cmt.append("> on ");
        cmt.append(Tools::nowToString());
        addComment(cmt);
        unsigned int i = 0;
        /* Log all the column cells */
        for (GridColumn::CellMap::iterator it = col->cells_begin();
            it != col->cells_end(); ++it)
          {
            if ((it->second != 0)
                && ((_filter == 0)
                    || (_filter->applyFilter(it->second->getRow()))))
              {
                GridCell& c = *(it->second);
                std::string tmp = "row";
                tmp.append(Tools::CStr(i));
                write(tmp, c.get());
              }
            i++;
          }
        /* Close Block */
        closeBlock("column");
      }
  }

  /** Set the log filter */
  void
  GridLog::setLogFilter(GridFilter* filter)
  {
    _filter = filter;
  }

  /** Get the log filter */
  GridFilter*
  GridLog::getLogFilter() const
  {
    return _filter;
  }

}
