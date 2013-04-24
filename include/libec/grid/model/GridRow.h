///////////////////////////////////////////////////////////////////////////////
/// @file		GridRow.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Row of a GridModel
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDROW_H__
#define LIBEC_GRIDROW_H__

#include <iostream>
#include <string>

#include "GridItem.h"
#include "../../value/Value.h"

namespace cea
{

  /* Prototype */
  class GridColumn;
  class GridCell;
  class GridModel;
  class GridView;

  /// @brief Row of a GridModel
  class GridRow : public GridItem
  {
  protected:

    /* Constructor */
    /// @brief Create a Column for a GridModel
    /// @param grid GridModel parent
    /// @param tag Tag of the row (see Monitor to understand tag)
    GridRow(GridModel& grid, int tag);

  public:

    /* Public - Type */
    /// @brief Type of Cells map for quick access
    typedef std::map<GridColumn*, GridCell*> CellMap;

    /* Cell Value */
    /// @brief Get cell associated to this row and a column
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param col Column of the wanted cell
    /// @return GridCell corresponding or GridModel::ncell if no cell found
    const GridCell&
    getCell(const GridColumn& col) const;
    /// @brief Get Cell associated to this row and a column number
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param colId Column number (start from 0)
    /// @return GridCell corresponding or GridModel::ncell if no cell found
    const GridCell&
    getCell(unsigned int colId) const;
    /// @brief Get Value of a cell associated to this row and a column
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param col Column of the wanted cell value
    /// @return Value of the cell or GridModel::nvalue if no cell found
    const Value&
    getValue(const GridColumn& col) const;
    /// @brief Get Value of a cell associated to this row
    ///        and a column number
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param colId Column number (start from 0)
    /// @return Value of the cell or GridModel::nvalue if no cell found
    const Value&
    getValue(unsigned int colId) const;

    /// @brief Set value of a cell associated to this row and a column
    /// @param col Column of the wanted cell
    /// @param v New value of the cell (see GridCell::set)
    /// @return Reference to the current object
    GridRow&
    setValue(const GridColumn& col, const Value& v);
    /// @brief Set value of a cell associated to this row
    ///        and a column number
    /// @param colId Column number (start from 0)
    /// @param v New value of the cell (see GridCell::set)
    /// @return Reference to the current object
    GridRow&
    setValue(unsigned int colId, const Value& v);

    /* Sum consideration */
    /// @brief Set if sum calcul ignore this row
    ///
    /// Sum calcul is the total of all considered row cell for a column
    ///
    /// @param activate If false the Row will be ignored in sum calcul \n
    ///                 Else the Row will be considered \n
    ///                 By default the row is considered in sum calcul
    GridRow&
    setSum(bool activate);
    /// @brief Check if sum calcul ignore this row
    /// @return true if this row is considered in sum calcul
    bool
    isSumConsidered() const;

    /* Iterator */
    /// @brief Get an iterator on the first cell mapped
    /// @return iterator on first cell mapped
    CellMap::iterator
    cells_begin();

    /// @brief Get an iterator on the last cell mapped
    /// @return iterator on last cell mapped
    CellMap::iterator
    cells_end();

    /* Friend class */
    friend class GridColumn;
    friend class GridCell;
    friend class GridModel;
    friend class GridView;

  protected:

    /* Menbers */
    CellMap cells; ///< Map of cells associated with this row
    bool _sumConsidered; ///< Indicate if Column sum calcul ignore this row

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridRow
///	@ingroup grid_model
///
///////////////////////////////////////////////////////////////////////////////
