///////////////////////////////////////////////////////////////////////////////
/// @file		GridColumn.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Column of a GridModel
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDCOLUMN_H__
#define LIBEC_GRIDCOLUMN_H__

#include <iostream>
#include <string>
#include <map>

#include "GridItem.h"
#include "../../value/Value.h"

namespace cea
{

  /* Prototype */
  class GridCell;
  class GridRow;
  class GridModel;
  class GridView;

  /// @brief Column of a GridModel
  class GridColumn : public GridItem
  {
  protected:

    /* Constructor */
    /// @brief Create a Column for a GridModel
    /// @param grdModel GridModel parent
    /// @param tag Tag of the column (see Monitor to understand tag)
    /// @param title Title of the column, text used by GridView
    /// @param type Type of the column (see Value::Type)
    /// @param minimumSize Theorical minimum size of the column.
    ///						Normally this value represent minimum size by
    ///						value length.
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    /// @param percentWidth Theorical percent size of the column.
    ///						Normally this value represent percent of a total
    ///						width availible on the view and the unit is
    ///						text char (not pixel).
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    GridColumn(GridModel& grdModel, int tag, const std::string& title,
        Value::Type type, unsigned int minimumSize = 0,
        unsigned int percentWidth = 0);

  public:

    /* Public - Type */
    /// @brief Type of Cells map for quick access
    typedef std::map<GridRow*, GridCell*> CellMap;

    /* Property */
    /// @brief Get title of the column
    /// @return Title of the column
    std::string
    getTitle() const;
    /// @brief Set title of the column
    /// @param title New title of the column, text used by GridView
    /// @return Reference to the current object
    GridColumn&
    setTitle(const std::string& title);

    /// @brief Get type of the column (see Value::Type)
    /// @return Type of the column
    Value::Type
    getType() const;
    /// @brief Set type of the column (see Value::Type)
    ///
    /// When column type change all the associated cell get their values
    /// reset to the initial value depend of the new type
    /// (see Value::setType).
    ///
    /// @param cellType New type of the column
    /// @return Reference to the current object
    GridColumn&
    setType(Value::Type cellType);

    /// @brief Get percent size of the column
    /// @return Percent size of the column
    unsigned int
    getPercentSize() const;
    /// @brief Get minimum size of the column
    /// @return Minimum size of the column
    unsigned int
    getMinimumSize() const;
    /// @brief Set size values of the column
    /// @param minimumSize Theorical minimum size of the column.
    ///						Normally this value represent minimum size by
    ///						value length.
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    /// @param percentWidth Theorical percent size of the column.
    ///						Normally this value represent percent of a total
    ///						width availible on the view and the unit is
    ///						text char (not pixel).
    ///						But since this value is interpreted by GridView
    ///						its important to refer to GridView used to know
    ///						how this value is used.
    /// @return Reference to the current object
    GridColumn&
    setSize(unsigned int minimumSize, unsigned int percentWidth);
    /// @brief Set minimum size of the column
    /// @param size Theorical minimum size of the column.
    ///				Normally this value represent minimum size by
    ///				value length.
    ///				But since this value is interpreted by GridView
    ///				its important to refer to GridView used to know
    ///				how this value is used.
    /// @return Reference to the current object
    GridColumn&
    setMinimumSize(unsigned int size);
    /// @brief Set percent size of the column
    /// @param size Theorical percent size of the column.
    ///				Normally this value represent percent of a total
    ///				width availible on the view and the unit is
    ///				text char (not pixel).
    ///				But since this value is interpreted by GridView
    ///				its important to refer to GridView used to know
    ///				how this value is used.
    /// @return Reference to the current object
    GridColumn&
    setPercentSize(unsigned int size);

    /* Cell Value */
    /// @brief Get cell associated to this column and a row
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param row Row of the wanted cell
    /// @return GridCell corresponding or GridModel::ncell if no cell found
    const GridCell&
    getCell(const GridRow& row) const;
    /// @brief Get Cell associated to this column and a row number
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param rowId Row number (start from 0)
    /// @return GridCell corresponding or GridModel::ncell if no cell found
    const GridCell&
    getCell(unsigned int rowId) const;
    /// @brief Get Value of a cell associated to this column and a row
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param row Row of the wanted cell value
    /// @return Value of the cell or GridModel::nvalue if no cell found
    const Value&
    getValue(const GridRow& row) const;
    /// @brief Get Value of a cell associated to this column
    ///        and a row number
    ///
    /// To test if the return is valid use GridModel::isValid function.
    ///
    /// @param rowId Row number (start from 0)
    /// @return Value of the cell or GridModel::nvalue if no cell found
    const Value&
    getValue(unsigned int rowId) const;

    /// @brief Set value of a cell associated to this column and a row
    /// @param row Row of the wanted cell
    /// @param v New value of the cell (see GridCell::set)
    /// @return Reference to the current object
    GridColumn&
    setValue(const GridRow& row, const Value& v);
    /// @brief Set value of a cell associated to this column
    ///        and a row number
    /// @param rowId Row number (start from 0)
    /// @param v New value of the cell (see GridCell::set)
    /// @return Reference to the current object
    GridColumn&
    setValue(unsigned int rowId, const Value& v);

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
    friend class GridCell;
    friend class GridRow;
    friend class GridModel;
    friend class GridView;

  protected:

    /* Members */
    std::string _name; ///< Name of the column by default empty
    std::string _title; ///< Title of the column used by GridView
    Value::Type _cellType; ///< Type of the column (see Value::Type)

    /// @brief Theorical minimum size of the column.
    ///
    /// Normally this value represent minimum size by value length. \n
    ///	But since this value is interpreted by GridView its important
    /// to refer to GridView used to know how this value is used.
    ///
    unsigned int _minimumSize;
    /// @brief Theorical percent size of the column.
    ///
    /// Normally this value represent percent of a total width availible
    /// on the view and the unit is text char (not pixel).
    ///
    /// But since this value is interpreted by GridView its important to
    /// refer to GridView used to know how this value is used.
    ///
    unsigned int _percentSize;

    CellMap cells; ///< Map of cells associated with this column

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridColumn
///	@ingroup grid_model
///
/// The tag property is extremely important for the monitor since monitor
/// use tag to recognize which type of data is linked on the column.
///
///////////////////////////////////////////////////////////////////////////////
