#include <libec/grid/model/GridCell.h>
#include <libec/grid/model/GridItem.h>
#include <libec/grid/model/GridColumn.h>
#include <libec/grid/model/GridRow.h>
#include <libec/grid/model/GridModel.h>
#include <libec/grid/view/GridView.h>

namespace cea
{

  /** +Constructor */
  GridCell::GridCell(GridModel& grid, GridRow& r, GridColumn& c) :
      grid(grid), row(r), column(c), value(c.getType()), _updateCounter(0)
  {
    value.reset();
  }

  /** +get */
  const Value&
  GridCell::get() const
  {
    return value;
  }
  /** +set */
  void
  GridCell::set(const Value& v)
  {
    if (value != v)
      {
        /* Update Counter et Row Counter */
        incUpdateCounter();
        row.incUpdateCounter();
        column.incUpdateCounter();
        grid.incUpdateCounter();
        /* Set Value */
        Value old = value;
        value = v;
        /* Call refresh cell */
        grid.viewRefreshCell(this, old);
      }
  }

  /** +getGrid */
  GridModel&
  GridCell::getGrid() const
  {
    return grid;
  }
  /** +getRow */
  GridRow&
  GridCell::getRow() const
  {
    return row;
  }
  /** +getColumn */
  GridColumn&
  GridCell::getColumn() const
  {
    return column;
  }

  /** +getUpdateCount */
  unsigned int
  GridCell::getUpdateCount() const
  {
    return _updateCounter;
  }
  /** +incUpdateCounter */
  void
  GridCell::incUpdateCounter()
  {
    /* Update the counter (cyclic) */
    _updateCounter++;
    if (_updateCounter > 1234567890)
      {
        _updateCounter = 0;
      }
  }

  /** #changeType */
  void
  GridCell::changeType(Value::Type type)
  {
    if (value.getType() != type)
      {
        /* Update counter */
        incUpdateCounter();
        row.incUpdateCounter();
        column.incUpdateCounter();
        grid.incUpdateCounter();
        /* Change the type */
        value.setType(type);
      }
  }

}
