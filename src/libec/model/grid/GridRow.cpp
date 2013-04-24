#include <libec/grid/model/GridCell.h>
#include <libec/grid/model/GridColumn.h>
#include <libec/grid/model/GridRow.h>
#include <libec/grid/model/GridModel.h>
#include <libec/grid/view/GridView.h>

namespace cea
{

  /** +Constructor */
  GridRow::GridRow(GridModel& grid, int tag) :
      GridItem(grid, tag), _sumConsidered(true)
  {
    ;
  }

  /** +getCell */
  const GridCell&
  GridRow::getCell(const GridColumn& col) const
  {
    /* Search the cell */
    CellMap::const_iterator c = cells.find(&(const_cast<GridColumn&>(col)));
    /* Return it if found otherwise return ncell */
    if (c == cells.end())
      {
        return GridModel::ncell;
      }
    else
      {
        return *(c->second);
      }
  }
  /** +getCell */
  const GridCell&
  GridRow::getCell(unsigned int colId) const
  {
    /* Get the col from colId */
    GridColumn& c = grid.getColumn(colId);
    /* Get the Cell */
    if (&c != &GridModel::ncolumn)
      {
        return getCell(c);
      }
    else
      {
        return GridModel::ncell;
      }
  }
  /** +getValue */
  const Value&
  GridRow::getValue(const GridColumn& col) const
  {
    /* Search the cell */
    CellMap::const_iterator c = cells.find(&(const_cast<GridColumn&>(col)));
    /* Return it's value if found otherwise return nvalue */
    if (c == cells.end())
      {
        return GridModel::nvalue;
      }
    else
      {
        return c->second->get();
      }
  }
  /** +getValue */
  const Value&
  GridRow::getValue(unsigned int colId) const
  {
    /* Get the col from colId */
    GridColumn& c = grid.getColumn(colId);
    /* Get the Value */
    if (&c != &GridModel::ncolumn)
      {
        return getValue(c);
      }
    else
      {
        return GridModel::nvalue;
      }
  }

  /** +setValue */
  GridRow&
  GridRow::setValue(const GridColumn& col, const Value& v)
  {
    /* Search the cell */
    CellMap::iterator c = cells.find(&(const_cast<GridColumn&>(col)));
    /* Set the Value */
    if (c == cells.end())
      {
        /* Create the cell */
        GridCell* cell = grid.createCell(*this, const_cast<GridColumn&>(col));
        if (cell != 0)
          {
            cell->set(v);
          }
      }
    else
      {
        /* Set the value */
        c->second->set(v);
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +setValue */
  GridRow&
  GridRow::setValue(unsigned int colId, const Value& v)
  {
    /* Get the col from colId */
    GridColumn& c = grid.getColumn(colId);
    /* Set the Value */
    if (&c != &GridModel::ncolumn)
      {
        setValue(c, v);
      }
    /* Return the reference of this object */
    return *this;
  }

  /** +setSum */
  GridRow&
  GridRow::setSum(bool activate)
  {
    _sumConsidered = activate;
    /* Return the reference of this object */
    return *this;
  }

  /** +isSumConsidered */
  bool
  GridRow::isSumConsidered() const
  {
    return _sumConsidered;
  }

  /** +cells_begin */
  GridRow::CellMap::iterator
  GridRow::cells_begin()
  {
    return cells.begin();
  }
  /** +cells_end */
  GridRow::CellMap::iterator
  GridRow::cells_end()
  {
    return cells.end();
  }

}
