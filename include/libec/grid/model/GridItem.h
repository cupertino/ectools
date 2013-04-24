///////////////////////////////////////////////////////////////////////////////
/// @file		GridItem.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base of GridColumn and GridRow containning common properties
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDITEM_H__
#define LIBEC_GRIDITEM_H__

#include <iostream>
#include <string>
#include <map>

#include "../../value/Value.h"

namespace cea
{

  /* Prototype */
  class GridModel;

  /// @brief Base of GridColumn and GridRow containning common properties
  class GridItem
  {
  protected:

    /* Protected - Members */
    unsigned int _id; ///< Current id in GridModel - managed by GridModel
    void* data; ///< Associated data (generic pointer managed by developer)

    GridModel& grid; ///< Reference to the parent GridModel

    /// @brief Update counter used to know change:
    ///        each time a new value is set on a associated cell this
    ///        counter is incremented.
    unsigned int _updateCounter;

    bool _fixed; ///< Fixed option
    int _fixedOffset; ///< Fixed offset
    bool _fixedAtEnd; ///< Fixed edge of the view

    /* Constructor/Destructor */
    /// @brief Create GridItem base for GridColumn/GridRow
    /// @param grid GridModel parent
    /// @param tag Tag of the GridItem (see Monitor to understand tag)
    GridItem(GridModel& grid, int tag);

    /// @brief Destroy all memory data created
    ///
    /// The only memory destroyed are memory allocated for args
    /// (see GridItem::setArg and GridItem::getArg)
    ///
    virtual
    ~GridItem();

    /* Protected - property */
    /// @brief Associate data (generic pointer managed by developer)
    /// @param d Generic pointer to the data to associate
    void
    setData(void* d);

  public:

    /* Public - Members */
    /// @brief Tag of the column (see Monitor to understand tag)
    const signed long int tag;

    /* Properties */
    /// @brief Get tha associated data
    /// @return Associated generic pointer Or 0 if no associated data
    void*
    getData() const;

    /// @brief Get GridModel of the cell
    /// @return The GridModel of the cell
    GridModel&
    getGrid() const;

    /* Update counter */
    /// @brief Get the current update counter value
    ///
    /// This function is usefull to know if the row or column have changed.
    ///
    /// <h3>Example</h3>
    /// @code
    ///		// A Row
    ///		GridRow& row;
    ///
    ///		// Store in memory the return of this function
    ///		unsigned int storedRowUpdateCount = row.getUpdateCount();
    ///
    ///		// Execute piece of code
    ///		// ...
    ///		// ...
    ///
    ///		// Check if Row have changed
    ///		if (row.getUpdateCount() != storedRowUpdateCount) {
    ///			// Row changed
    ///		}else{
    ///			// Row not changed
    ///		}
    /// @endcode
    ///
    /// @return Update counter value
    unsigned int
    getUpdateCount() const;

    /// @brief Increment update counter
    ///
    /// This function can be used to simulate an update of the item
    ///
    void
    incUpdateCounter();

    /* Fix option */
    /// @brief Fix a column or a row
    ///
    /// This will not affect the current GridModel connected GridView. \n
    ///
    /// To fix a column use GridView::fixColumn. \n
    /// To fix a row use GridView::fixRow.
    ///
    /// @param fixedOffset Offset from Start or End of visible column in
    ///						GridView.
    /// @param fixedAtEnd If true Fix at first + offset visible position.
    ///                   Else Fix at end - offset visible position
    /// @return Reference to the current object
    GridItem&
    setFixed(int fixedOffset, bool fixedAtEnd = false);
    /// @brief Unfix a colum/row
    ///
    /// This will not affect the current GridModel connected GridView. \n
    ///
    /// To unfix a column use GridView::unfixColumn. \n
    /// To unfix a row use GridView::unfixRow.
    ///
    /// @return Reference to the current object
    GridItem&
    unfix();

    /// @brief Check if the column/row is initially fixed.
    /// @return true if initially fixed
    bool
    isFixed() const;
    /// @brief Get Initial fix offset
    /// @return Initial fixed offset
    int
    getFixedOffset() const;
    /// @brief Get Initial fix edge
    /// @return true If initially fixed at the end of visible item
    bool
    isFixedAtEnd() const;

    /* Args Function */
    /// @brief Associate to an arg id a value
    ///
    /// Arg are usefull system to store extra value for a row or column.
    /// The value are stored as a map identified by an unsigned int. \n
    ///
    /// By this system its possible to store an infinity of arg value of any
    /// type for a row or column. \n
    ///
    /// <b>Remark:</b> By default the args take no memory until the first
    ///			       usage where an map is created in memory.
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // Create a model
    ///     GridModel m;
    ///
    ///     // Add 4 CPU Core Column and use an arg to distinct core
    ///     for(unsigned int i = 0; i < 4; i++) {
    ///         m.addColumn("Core", Value::INT)
    ///          .setArg(0, i) // Set Core ID in arg 0
    ///          .setArg(10, "Core") // Set Core name in arg 10
    ///          ;
    ///     }
    /// @endcode
    ///
    /// @param id Id of arg to set
    /// @param v Value of the arg
    /// @return Reference to the current object
    GridItem&
    setArg(unsigned int id, const Value& v);

    /// @brief Get an arg value
    /// @param id Id of arg to get
    /// @return The arg value if exist otherwise GridModel::nvalue.
    /// @see GridItem::setArg
    const Value&
    getArg(unsigned int id) const;

    /* Friend class */
    friend class GridModel;

  private:

    /* Private Members */
    /// @brief Type of the argument map
    typedef std::map<unsigned int, Value*> ArgMap;

    /// @brief Arg Map not instancied if no arg to avoid useless memory
    ArgMap* args;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridItem
///	@ingroup grid_model
///
///////////////////////////////////////////////////////////////////////////////
