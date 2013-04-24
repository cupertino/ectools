///////////////////////////////////////////////////////////////////////////////
/// @file		GridCell.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Cell of a GridModel
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDCELL_H__
#define LIBEC_GRIDCELL_H__

#include <iostream>
#include <string>

#include "../../value/Value.h"

namespace cea
{

  /* Prototype */
  class GridColumn;
  class GridRow;
  class GridModel;
  class GridView;

  /// @brief Cell of a GridModel
  class GridCell
  {
  protected:

    /* Constructor */
    /// @brief Create a cell for grid and associated it
    ///        with a row and a column
    /// @param grid GridModel of the cell
    /// @param r GridRow of the cell
    /// @param c GridColumn of the cell
    GridCell(GridModel& grid, GridRow& r, GridColumn& c);

  public:

    /* Value - property */
    /// @brief Get the cell value
    /// @return The Cell value
    const Value&
    get() const;

    /// @brief Set the cell value
    ///
    /// When a new value is set the update counter for row,column,cell
    /// are incremented and GridModel send the information to all the
    /// connected view.
    ///
    /// @param v New value for the cell
    void
    set(const Value& v);

    /* Get GridModel, GridRow, GridColumn */
    /// @brief Get GridModel of the cell
    /// @return The GridModel of the cell
    GridModel&
    getGrid() const;
    /// @brief Get GridRow associated to the cell
    /// @return The GridRow of the cell
    GridRow&
    getRow() const;
    /// @brief Get GridColumn associated to the cell
    /// @return The GridColumn of the cell
    GridColumn&
    getColumn() const;

    /* Update counter */
    /// @brief Get the current update counter value
    ///
    /// This function is usefull to know if the cell have changed.
    ///
    /// <h3>Example</h3>
    /// @code
    ///		// A Cell
    ///		GridCell& cell;
    ///
    ///		// Store in memory the return of this function
    ///		unsigned int storedCellUpdateCount = cell.getUpdateCount();
    ///
    ///		// Execute piece of code
    ///		// ...
    ///		// ...
    ///
    ///		// Check if Cell have changed
    ///		if (cell.getUpdateCount() != storedCellUpdateCount) {
    ///			// Cell changed
    ///		}else{
    ///			// Cell not changed
    ///		}
    /// @endcode
    ///
    /// @return Update counter value
    unsigned int
    getUpdateCount() const;

    /// @brief Increment update counter
    ///
    /// This function can be used to simulate an update of the cell
    ///
    void
    incUpdateCounter();

    /* Friend class */
    friend class GridColumn;
    friend class GridRow;
    friend class GridModel;

  protected:

    /* Protected - function */
    /// @brief Change the type of the cell
    /// @param type New type of the cell to set
    void
    changeType(Value::Type type);

    /* Members */
    GridModel& grid; ///< GridModel parent
    GridRow& row; ///< GridRow associated
    GridColumn& column; ///< GridColumn associated

    Value value; ///< Value of the cell

    /// @brief Update counter used to know change:
    ///        each time a new value is set this counter is incremented
    unsigned int _updateCounter;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridCell
///	@ingroup grid_model
///
///////////////////////////////////////////////////////////////////////////////
