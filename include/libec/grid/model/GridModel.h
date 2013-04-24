///////////////////////////////////////////////////////////////////////////////
/// @file		GridModel.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base class for grid model
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDMODEL_H__
#define LIBEC_GRIDMODEL_H__

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "GridRow.h"
#include "GridColumn.h"
#include "GridItem.h"
#include "GridCell.h"

#include "../../value/Value.h"

namespace cea
{

  /* Prototype */
  class GridView;
  class GridViewLoader;
  class GridFilter;

  /// @brief Base class for grid model
  class GridModel
  {
  public:

    /* Static Value */
    /// @brief Represent an invalid position
    static const unsigned int npos = (unsigned int) (-1);
    /// @brief Represent an invalid Value
    static Value nvalue;
    /// @brief Represent an invalid Grid
    static GridModel ngrid;
    /// @brief Represent an invalid GridColumn
    static GridColumn ncolumn;
    /// @brief Represent an invalid GridRow
    static GridRow nrow;
    /// @brief Represent an invalid GridCell
    static GridCell ncell;

    /* Constructor/Destructor */
    /// @brief Default constructor
    GridModel();
    /// @brief Copy constructor (copy all row/column/cell from GridModel)
    /// @param grid GridModel to copy
    GridModel(const GridModel& grid);

    /// @brief Delete all row/column/cell created
    virtual
    ~GridModel();

    /* Column section */
    /// @brief Clear all the column
    /// @return Reference to the current object
    GridModel&
    clearColumn();

    /// @brief Add a column
    /// @param title Title of the column to add
    /// @param type Type of the column to add
    /// @param tag Column tag used to identify the column
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
    /// @return Reference to the created GridColumn. \n
    ///         Or reference to ncolumn if column not added
    GridColumn&
    addColumn(const std::string& title, const Value::Type type, int tag = -1,
        unsigned int minimumSize = 0, unsigned int percentWidth = 0);

    /// @brief Remove a GridColumn
    /// @param col GridColumn to remove
    /// @return Reference to the current object
    GridModel&
    removeColumn(GridColumn& col);
    /// @brief Remove a GridColumn by id
    /// @param colId GridColumn id to remove
    /// @return Reference to the current object
    GridModel&
    removeColumn(unsigned int colId);
    /// @brief Get GridColumn count
    /// @return Number of GridColumn
    unsigned int
    getColumnCount();
    /// @brief Get GridColumn id
    /// @param col GridColumn considered
    /// @return GridColumn id or npos if col invalid
    unsigned int
    getColumnId(const GridColumn& col);
    /// @brief Get a GridColumn by id
    /// @param id GridColumn id to get
    /// @return Reference to the GridColumn. \n
    ///         Or reference to ncolumn if id invalid
    GridColumn&
    getColumn(unsigned int id);
    /// @brief Test if a GridColumn is valid
    /// @param col GridColumn to test
    /// @return true if col is valid
    bool
    isValid(const GridColumn& col);

    /* Row section */
    /// @brief Clear all the row
    /// @return Reference to the current object
    GridModel&
    clearRow();
    /// @brief Add a row
    /// @param tag Row tag used to identify the row
    /// @return Reference to the created GridRow. \n
    ///         Or reference to nrow if row not added
    GridRow&
    addRow(int tag = -1);
    /// @brief Remove a GridRow
    /// @param row GridRow to remove
    /// @return Reference to the current object
    GridModel&
    removeRow(GridRow& row);
    /// @brief Remove a GridRow by id
    /// @param rowId GridRow id to remove
    /// @return Reference to the current object
    GridModel&
    removeRow(unsigned int rowId);
    /// @brief Get GridRow count
    /// @return Number of GridRow
    unsigned int
    getRowCount();
    /// @brief Get GridRow id
    /// @param row GridRow considered
    /// @return GridRow id or npos if row invalid
    unsigned int
    getRowId(const GridRow& row);
    /// @brief Get a GridRow by id
    /// @param id GridRow id to get
    /// @return Reference to the GridRow. \n
    ///         Or reference to nrow if id invalid
    GridRow&
    getRow(unsigned int id);
    /// @brief Test if a GridRow is valid
    /// @param row GridRow to test
    /// @return true if row is valid
    bool
    isValid(const GridRow& row);

    /* Cell section */
    /// @brief Get a GridCell by GridRow and GridColumn
    /// @param row GridRow considered
    /// @param col GridColumn considered
    /// @return Const reference to the GridCell. \n
    ///         Or reference to ncell if row or col is invalid
    const GridCell&
    getCell(const GridRow& row, const GridColumn& col);
    /// @brief Get a GridCell by GridRow and GridColumn id
    /// @param row GridRow considered
    /// @param colId GridColumn id considered
    /// @return Const reference to the GridCell. \n
    ///         Or reference to ncell if row or colId is invalid
    const GridCell&
    getCell(const GridRow& row, unsigned int colId);
    /// @brief Get a GridCell by GridRow id and GridColumn
    /// @param rowId GridRow id considered
    /// @param col GridColumn considered
    /// @return Const reference to the GridCell. \n
    ///         Or reference to ncell if rowId or col is invalid
    const GridCell&
    getCell(unsigned int rowId, const GridColumn& col);
    /// @brief Get a GridCell by GridRow id and GridColumn id
    /// @param rowId GridRow id considered
    /// @param colId GridColumn id considered
    /// @return Const reference to the GridCell. \n
    ///         Or reference to ncell if rowId or colId is invalid
    const GridCell&
    getCell(unsigned int rowId, unsigned int colId);
    /// @brief Test if a GridCell is valid
    /// @param cell GridCell to test
    /// @return true if cell is valid
    bool
    isValid(const GridCell& cell);

    /// @brief Get a GridCell Value by GridRow and GridColumn
    /// @param row GridRow considered
    /// @param col GridColumn considered
    /// @return Const reference to the GridCell Value. \n
    ///         Or reference to nvalue if row or col is invalid
    const Value&
    getValue(const GridRow& row, const GridColumn& col);
    /// @brief Get a GridCell Value by GridRow and GridColumn id
    /// @param row GridRow considered
    /// @param colId GridColumn id considered
    /// @return Const reference to the GridCell Value. \n
    ///         Or reference to ncell if row or colId is invalid
    const Value&
    getValue(const GridRow& row, unsigned int colId);
    /// @brief Get a GridCell Value by GridRow id and GridColumn
    /// @param rowId GridRow id considered
    /// @param col GridColumn considered
    /// @return Const reference to the GridCell Value. \n
    ///         Or reference to nvalue if rowId or col is invalid
    const Value&
    getValue(unsigned int rowId, const GridColumn& col);
    /// @brief Get a GridCell Value by GridRow id and GridColumn id
    /// @param rowId GridRow id considered
    /// @param colId GridColumn id considered
    /// @return Const reference to the GridCell Value. \n
    ///         Or reference to nvalue if rowId or colId is invalid
    const Value&
    getValue(unsigned int rowId, unsigned int colId);
    /// @brief Test if a Value is valid
    /// @param v Value to test
    /// @return true if v is valid
    bool
    isValid(const Value& v);

    /// @brief Set GridCell Value by GridRow and GridColumn
    /// @param row GridRow considered
    /// @param col GridColumn considered
    /// @param v Value to set
    /// @return Reference to the current object
    GridModel&
    setValue(GridRow& row, GridColumn& col, const Value& v);
    /// @brief Set GridCell Value by GridRow and GridColumn id
    /// @param row GridRow considered
    /// @param colId GridColumn id considered
    /// @param v Value to set
    /// @return Reference to the current object
    GridModel&
    setValue(GridRow& row, unsigned int colId, const Value& v);
    /// @brief Set GridCell Value by GridRow id and GridColumn
    /// @param rowId GridRow id considered
    /// @param col GridColumn considered
    /// @param v Value to set
    /// @return Reference to the current object
    GridModel&
    setValue(unsigned int rowId, GridColumn& col, const Value& v);
    /// @brief Set GridCell Value by GridRow id and GridColumn id
    /// @param rowId GridRow id considered
    /// @param colId GridColumn id considered
    /// @param v Value to set
    /// @return Reference to the current object
    GridModel&
    setValue(unsigned int rowId, unsigned int colId, const Value& v);

    /* Filter Section */
    /// @brief Set filter value
    /// @param str Value of the filter
    void
    setFilter(const std::string& str);
    /// @brief Get filter string value
    /// @return Filter string value
    std::string
    getFilter() const;

    /* View */
    /// @brief Clear all the connected view
    /// @return Reference to the current object
    GridModel&
    clearView();
    /// @brief Connect a view
    /// @param view GridView to connect
    /// @return Reference to the current object
    GridModel&
    connectView(GridView& view);
    /// @brief Deconnect a view
    /// @param view GridView to deconnect
    /// @return Reference to the current object
    GridModel&
    removeView(GridView& view);

    /// @brief Check if one of the connected GridView have the
    ///        GridRow rendered
    /// @param r GridRow to test
    /// @return true if at least one of connected GridView have the row
    ///         rendered.
    bool
    isRowVisible(const GridRow& r);
    /// @brief Check if one of the connected GridView have the
    ///        GridColumn rendered
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView have the column
    ///         rendered.
    bool
    isColumnVisible(const GridColumn& c);
    /// @brief Check if one of the connected GridView have the
    ///        GridCell rendered
    /// @param r GridRow to test
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView have the cell
    ///         rendered.
    bool
    isCellVisible(const GridRow& r, const GridColumn& c);
    /// @brief Check if one of the connected GridView have the
    ///        GridColumn as sort reference
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView have column
    ///         as sort reference.
    bool
    isViewSortByColumn(const GridColumn& c);
    /// @brief Check if one of the connected GridView have GridColumn
    ///         sum activate
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView have column
    ///         sum activate.
    bool
    isViewColumnSumEnabled(const GridColumn& c);
    /// @brief Check if one of the connected GridView need the GridColumn
    ///        values.
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView need the column
    ///         values.
    bool
    isViewNeedData(const GridColumn& c);
    /// @brief Check if one of the connected GridView need the GridCell
    ///        values.
    /// @param r GridRow to test
    /// @param c GridColumn to test
    /// @return true if at least one of connected GridView need cell value
    bool
    isViewNeedData(const GridRow& r, const GridColumn& c);

    /* Tools Function */
    /// @brief Clear all the rows, columns and cells
    /// @return Reference to the current object
    GridModel&
    clear();

    /// @brief Test if id is valid
    /// @param id Id to test
    /// @return true if id is valid
    bool
    isValid(unsigned int id);

    /// @brief Get the current update counter value
    ///
    /// This function is usefull to know if the model have changed.
    ///
    /// <h3>Example</h3>
    /// @code
    ///		// A GridModel
    ///		GridModel& grid;
    ///
    ///		// Store in memory the return of this function
    ///		unsigned int storedUpdateCount = grid.getUpdateCount();
    ///
    ///		// Execute piece of code
    ///		// ...
    ///		// ...
    ///
    ///		// Check if grid model have changed
    ///		if (grid.getUpdateCount() != storedUpdateCount) {
    ///			// Grid model changed
    ///		}else{
    ///			// Grid model not changed
    ///		}
    /// @endcode
    ///
    /// @return Update counter value
    unsigned int
    getUpdateCount() const;

    /// @brief Increment update counter
    ///
    /// This function can be used to simulate an update of the GridModel
    ///
    void
    incUpdateCounter();

    /* Column/Item section */
    /// @brief Set a name to a GridColumn
    /// @param col GridColumn to change
    /// @param name Name to set
    void
    setGridColumnName(GridColumn& col, const std::string& name);

    /// @brief Get a GridColumn by name
    /// @param name GridColumn name to retrieve
    /// @param ignoreCase Ignore case option
    /// @return Reference to the GridColumn. \n
    ///         Or reference to ncolumn if column not found
    GridColumn&
    getGridColumnByName(const std::string& name, bool ignoreCase = true);

    /// @brief Set GridItem data
    /// @param it GridRow or GridColumn to change
    /// @param data Generic pointer to set
    void
    setGridItemData(GridItem& it, void* data);

    /* Friend class */
    friend class GridColumn;
    friend class GridRow;
    friend class GridCell;
    friend class GridView;
    friend class GridViewLoader;
    friend class GridFilter;

  protected:

    /* Protected - functions */
    /// @brief Create a cell and associated it to a row and a column
    /// @param forRow GridRow considered
    /// @param forColumn GridColumn considered
    /// @return Pointer to the GridCell created. \n
    ///         Or 0 if no GridCell created.
    GridCell*
    createCell(GridRow& forRow, GridColumn& forColumn);

    /// @brief Event triggered after a GridRow created
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @param r GridRow created
    /// @see eventMode
    virtual void
    onGridRowCreated(GridRow& r);
    /// @brief Event triggered before a GridRow deleted
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @param r GridRow deleted
    /// @see eventMode
    virtual void
    onGridRowDelete(GridRow& r);
    /// @brief Event triggered before all GridRow deletion
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @see eventMode
    virtual void
    onGridRowClear();

    /// @brief Event triggered after a GridColumn created
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @param c GridColumn created
    /// @see eventMode
    virtual void
    onGridColumnCreated(GridColumn& c);
    /// @brief Event triggered before a GridColumn deleted
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @param c GridColumn deleted
    /// @see eventMode
    virtual void
    onGridColumnDelete(GridColumn& c);
    /// @brief Event triggered before all GridColumn deletion
    ///        (only if event mode)
    ///
    /// This function can be overridden. \n
    /// By default do nothing.
    ///
    /// @see eventMode
    virtual void
    onGridColumnClear();

    /* Protected Member */
    /// @brief Typedef of GridColumn tag map
    typedef std::multimap<int, GridColumn*> ColumnTagMap;
    /// @brief Typedef of GridColumn list
    typedef std::vector<GridColumn*> ColumnList;

    /// @brief Typedef of GridRow tag map
    typedef std::multimap<int, GridRow*> RowTagMap;
    /// @brief Typedef of GridRow list
    typedef std::vector<GridRow*> RowList;

    /// @brief Typedef of GridCell list
    typedef std::set<GridCell*> CellList;

    /// @brief Typedef of GridView list
    typedef std::list<GridView*> ViewList;

    ColumnTagMap columnTags; ///< GridColumn tag map
    ColumnList columns; ///< List of GridColumn created

    RowTagMap rowTags; ///< GridRow tag map
    RowList rows; ///< List of GridRow created

    CellList cells; ///< List of GridCell created

    ViewList views; ///< List of GridView connected

    /// @brief Update counter used to know change:
    ///        each time the GridModel change this counter is incremented.
    unsigned int _updateCounter;

    /// @brief Event Mode status
    ///
    /// Event mode are just for extend class of GridModel. \n
    ///
    /// To activate event mode set eventMode on constructor of the
    /// extended GridModel.
    ///
    /// @see onGridRowCreated, onGridRowDelete, onGridRowClear
    /// @see onGridColumnCreated, onGridColumnDelete, onGridColumnClear
    bool eventMode;

    GridFilter& _filter; ///< Filter ready to use

  private:

    /* Private - function */
    /// @brief Add a GridColumn to all connected GridView
    /// @param col GridColumn to add
    void
    privAddColumn(GridColumn* col);

    /// @brief Remove a GridColumn to all connected GridView and removed linked
    ///        cell
    /// @param col GridColumn to remove
    void
    privRemoveColumn(GridColumn* col);
    /// @brief Remove a GridColumn to column tag map
    /// @param col GridColumn to remove
    void
    privRemoveColumnTagMap(GridColumn* col);

    /// @brief Add a GridRow to all connected GridView
    /// @param col GridRow to add
    void
    privAddRow(GridRow* row);
    /// @brief Remove a GridRow to all connected GridView and removed
    ///        linked cell
    /// @param col GridRow to remove
    void
    privRemoveRow(GridRow* row);
    /// @brief Remove a GridRow to column tag map
    /// @param col GridRow to remove
    void
    privRemoveRowTagMap(GridRow* row);

    /** Private function - Refresh Cell to View */
    /// @brief Refresh a GridCell to all connected GridView
    /// @param cell GridCell to refresh
    /// @param oldValue Old value of the GridCell
    void
    viewRefreshCell(GridCell* cell, const Value& oldValue);
    /** Private function - Refresh Column to View */
    /// @brief Refresh a GridColumn to all connected GridView
    /// @param col GridColumn to refresh
    void
    viewRefreshColumn(GridColumn* col);

    /// @brief Generic tag iterator class
    template<typename T, typename LST, int TAG = -1>
      class tagiterator
      {
      private:
        /* Members */
        /// @brief Contains the considered list
        LST& lst;
        /// @brief Range of item with same tag
        std::pair<typename LST::iterator, typename LST::iterator> ret;
        /// @brief The iterator used
        typename LST::iterator it;

      public:
        /* Constructor */
        /// @brief Create a "tag" iterator for a list
        /// @param l List considered
        tagiterator(LST& l) :
            lst(l)
        {
          ret = lst.equal_range(TAG);
          it = ret.first;
        }
        /// @brief Create a "tag" iterator from a same "tag" iterator
        /// @param i "tag" iterator to copy
        tagiterator(const tagiterator& i) :
            lst(i.lst), ret(i.ret), it(i.it)
        {
          ;
        }
        /* Operator */
        /// @brief Get corresponding GridRow or GridColumn
        /// @return Corresponding item of the iterator
        T&
        operator*()
        {
          return *(it->second);
        }
        ;
        /// @brief Get corresponding GridRow or GridColumn of the iterator
        /// @return Corresponding item of the iterator
        T*
        operator->()
        {
          return ((it->second));
        }
        /// @brief Increment the iterator
        ///
        /// Get next "tag" item in the list.
        ///
        /// @return A reference to the current object
        tagiterator&
        operator++()
        {
          ++it;
          return *this;
        }
        /// @brief Check if reach the end of the corresponding "tag" item
        /// @param v Ignored used only to allow the usage of != C++ operator
        /// @return true if reach the end of the corresponding "tag" item
        bool
        operator!=(int v)
        {
          return (it != ret.second);
        }
      };

  public:

    /// @brief Column "tag" Iterator
    ///
    /// This iterator allow to select only certain item follow their
    /// tag. \n
    ///
    /// By default the iterator . \n
    ///
    /// If all rows/columns need to be updated then use classic iterator
    /// of GridModel::columns or GridModel::rows member.
    ///
    /// <h3>Example</h3>
    /// @code
    ///     for(GridModel::col_iterator<CPU> c; c != 0; ++c) {
    ///			// c-> or *c == GridColumn&
    ///		}
    /// @endcode
    ///
    template<int TAG = -1>
      class col_iterator : public tagiterator<GridColumn, ColumnTagMap, TAG>
      {
      public:
        /// @brief Create a column "tag" iterator
        /// @param grd GridModel considered
        col_iterator(GridModel& grd) :
            tagiterator<GridColumn, ColumnTagMap, TAG>(grd.columnTags)
        {
        }
      };

    /// @brief Row "tag" Iterator
    ///
    /// @copydetails col_iterator
    ///
    template<int TAG = -1>
      class row_iterator : public tagiterator<GridRow, RowTagMap, TAG>
      {
      public:
        /// @brief Create a row "tag" iterator
        /// @param grd GridModel considered
        row_iterator(GridModel& grd) :
            tagiterator<GridRow, RowTagMap, TAG>(grd.rowTags)
        {
        }
      };

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridModel
///	@ingroup grid_model
///
///////////////////////////////////////////////////////////////////////////////
