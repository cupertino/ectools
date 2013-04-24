#include <libec/grid/model/GridColumn.h>
#include <libec/grid/model/GridModel.h>
#include <libec/grid/model/GridRow.h>
#include <libec/grid/model/GridCell.h>
#include <libec/grid/view/GridView.h>

namespace cea
{

  /** +Constructor */
  GridColumn::GridColumn(GridModel& grdModel, int tag, const std::string& title,
      Value::Type cellType, unsigned int minimumSize, unsigned int percentWidth) :
      GridItem(grdModel, tag), _title(title), _cellType(cellType), _minimumSize(
          minimumSize), _percentSize(percentWidth)
  {
    ;
  }

  /** +getTitle */
  std::string
  GridColumn::getTitle() const
  {
    return _title;
  }
  /** +setTitle */
  GridColumn&
  GridColumn::setTitle(const std::string& title)
  {
    _title.assign(title);
    return *this;
  }

  /** +getType */
  Value::Type
  GridColumn::getType() const
  {
    return _cellType;
  }
  /** +setType */
  GridColumn&
  GridColumn::setType(Value::Type cellType)
  {
    if (_cellType != cellType)
      {
        /*Set Property */
        _cellType = cellType;
        /* Change the views */
        grid.viewRefreshColumn(this);
      }
    return *this;
  }

  /** +getPercentSize */
  unsigned int
  GridColumn::getPercentSize() const
  {
    return _percentSize;
  }
  /** +getMinimumSize */
  unsigned int
  GridColumn::getMinimumSize() const
  {
    return _minimumSize;
  }

  /** +setSize */
  GridColumn&
  GridColumn::setSize(unsigned int minimumSize, unsigned int percentWidth)
  {
    if ((_percentSize != percentWidth) || (_minimumSize != minimumSize))
      {
        _minimumSize = minimumSize;
        _percentSize = percentWidth;
        /* Change the views */
        grid.viewRefreshColumn(this);
      }
    return *this;
  }

  /** +setMinimumSize */
  GridColumn&
  GridColumn::setMinimumSize(unsigned int size)
  {
    if (_minimumSize != size)
      {
        _minimumSize = size;
        /* Change the views */
        grid.viewRefreshColumn(this);
      }
    return *this;
  }
  /** +setPercentSize */
  GridColumn&
  GridColumn::setPercentSize(unsigned int size)
  {
    if (_percentSize != size)
      {
        _percentSize = size;
        /* Change the views */
        grid.viewRefreshColumn(this);
      }
    return *this;
  }

  /** +getCell */
  const GridCell&
  GridColumn::getCell(const GridRow& row) const
  {
    /* Search the cell */
    CellMap::const_iterator it = cells.find(&(const_cast<GridRow&>(row)));
    /* Return it if found otherwise return ncell */
    if (it == cells.end())
      {
        return GridModel::ncell;
      }
    else
      {
        return *(it->second);
      }
  }
  /** +getCell */
  const GridCell&
  GridColumn::getCell(unsigned int rowId) const
  {
    /* Get the row from rowId */
    GridRow& r = grid.getRow(rowId);
    /* Get the Cell */
    if (&r != &GridModel::nrow)
      {
        return getCell(r);
      }
    else
      {
        return GridModel::ncell;
      }
  }
  /** +getValue */
  const Value&
  GridColumn::getValue(const GridRow& row) const
  {
    /* Search the cell */
    CellMap::const_iterator it = cells.find(&(const_cast<GridRow&>(row)));
    /* Return it's value if found otherwise return nvalue */
    if (it == cells.end())
      {
        return GridModel::nvalue;
      }
    else
      {
        return it->second->get();
      }
  }
  /** +getValue */
  const Value&
  GridColumn::getValue(unsigned int rowId) const
  {
    /* Get the row from rowId */
    GridRow& r = grid.getRow(rowId);
    /* Get the Value */
    if (&r != &GridModel::nrow)
      {
        return getValue(r);
      }
    else
      {
        return GridModel::nvalue;
      }
  }

  /** +setValue */
  GridColumn&
  GridColumn::setValue(const GridRow& row, const Value& v)
  {
    /* Search the cell */
    CellMap::iterator it = cells.find(&(const_cast<GridRow&>(row)));
    /* Set the Value */
    if (it == cells.end())
      {
        /* Create the cell */
        GridCell* cell = grid.createCell(const_cast<GridRow&>(row), *this);
        if (cell != 0)
          {
            cell->set(v);
          }
      }
    else
      {
        /* Set the value */
        it->second->set(v);
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +setValue */
  GridColumn&
  GridColumn::setValue(unsigned int rowId, const Value& v)
  {
    /* Get the row from rowId */
    GridRow& r = grid.getRow(rowId);
    /* Set the Value */
    if (&r != &GridModel::nrow)
      {
        setValue(r, v);
      }
    /* Return the reference of this object */
    return *this;
  }

  /** +cells_begin */
  GridColumn::CellMap::iterator
  GridColumn::cells_begin()
  {
    return cells.begin();
  }
  /** +cells_end */
  GridColumn::CellMap::iterator
  GridColumn::cells_end()
  {
    return cells.end();
  }

}
