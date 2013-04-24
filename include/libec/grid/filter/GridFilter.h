///////////////////////////////////////////////////////////////////////////////
/// @file		GridFilter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Filter for GridModel, GridView, GridLog, Monitor
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRID_FILTER_H__
#define LIBEC_GRID_FILTER_H__

#include <iostream>
#include <string>
#include <list>
#include <vector>

#include "../../value/Value.h"
#include "../../tools/Tools.h"
#include "../view/GridView.h"
#include "../model/GridModel.h"
#include "../model/GridRow.h"

namespace cea
{

  /// @brief Filter for GridModel, GridView, GridLog, Monitor
  class GridFilter
  {
  public:

    /* Typedef/Struct */
    /// @brief Type of Operation
    enum OPERATOR
    {
      EGAL, ///< Operator =
      DIFF, ///< Operator != or <>
      SUP, ///< Operator >
      INF, ///< Operator <
      SUPEGAL, ///< Operator >= or =>
      INFEGAL ///< Operator <= or =<
    };

    /// @brief Value of filter element
    struct FilterValue
    {
      /// @brief Representing the loaded value
      std::string strValue;
      /// @brief When refresh call this value is typed as the
      ///			corresponding GridColumn and set to strValue
      Value value;
    };

    /// @brief Filter Value List
    typedef std::list<FilterValue*> ValueList;

    /// @brief Filter Element containing all values
    struct FilterElement
    {
      /// @brief Column Id - Id of column from GridModel \n
      ///			If -1 search column by name
      int colId;
      /// @brief Column Name - Name of column to search
      ///			used only if colId == -1
      std::string colName;
      /// @brief Pointer to the Column in GridModel
      GridColumn* col;
      /// @brief Operator used for comparing cell values
      OPERATOR op;
      /// @brief Values to compare to cells
      ValueList params;
    };

    /// @brief Filter Element List
    typedef std::list<FilterElement*> FilterElementList;
    /// @brief Filter List
    typedef std::list<FilterElementList*> FilterList;

  public:

    /* Functions */
    /// @brief Destroy all the elements created in memory
    ~GridFilter();

    /// @brief Apply all filter and return true if one of them passed
    /// @param row GridRow of a GridModel
    bool
    applyFilter(GridRow& row);

    /// @brief Apply all filter and return true if one of them passed
    /// @param row RowView of a GridView
    bool
    applyFilter(GridView::RowView& row);

    /// @brief Retrieve all GridColumn pointer
    ///
    /// This function need to be call each time the concerned GridModel
    /// changed its columns.
    ///
    /// @param model GridModel which contains all Columns used for filter
    void
    refresh(GridModel& model);

    /// @brief Erase from memory all filter elements and clear lists
    void
    clearAllFilter();

    /// @brief Set the filter code
    /// @param code Filter in specific formatted string
    /// @return true if enought memory to properly create the filter
    /// @see cea::GridFilter Header doc
    bool
    set(std::string code);

    /// @brief Retrieve the filter code
    /// @return Same string passed to set function
    /// @see cea::GridFilter::set
    std::string
    get() const;

  protected:

    /* Members */
    std::string _filterCode; ///< Code of Filter
    FilterList filters; ///< List of all Filters linked by OR operator

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridFilter
///	@ingroup grid
///
/// <h3>Description</h3>
/// <!--===========-->
///	A filter is defined by a string with a similar mathematical format
///
/// Filter understand 8 operator (=,!=,>,<,<=,>=, &, |)
///	Also filter can understand any kind of value (string, int, float, list)
///
/// <h3>Example of format</h3>
/// <!--=================-->
///	@code
///		(PID > 100) & (PID < 150) | (NAME = "chrome", "calc")
/// @endcode
///
/// Example of format with list values:
///	@code
///		PID = 10,20,25,30,45,"test" | $2 > 10,80,90
/// @endcode
///
/// <h3>Format of Filter</h3>
/// <!--================-->
/// @code
///        
///     FILTER  :==   COLUMN + OP + VALUE
///                 | FILTER + �|� + FILTER            -- Or operator
///                 | FILTER + �;� + FILTER            -- Or operator
///
///     COLUMN  :==   NAME 
///                 | '$' + ID
///     NAME    :== GridColumn::_name
///     ID      :== 1 to GridModel::columns.size() (start at index 1)
///
///
///     OP      :==   '=' | '==' | ':='                -- Egal
///                 | '!=' | '<>'                      -- Different
///                 | '>=' | '=>'                      -- Superior or egal
///                 | '<=' | '=<'                      -- Inferior or egal
///                 | '<'                              -- Inferior 
///                 | '>'                              -- Superior
///
///     VALUE   :==   Integer
///                 | Unsigned integer
///                 | Double
///                 | '"' + String + '"'
///                 | "'" + String + "'"
///                 | VALUE + ',' + VALUE
///
/// @endcode
///////////////////////////////////////////////////////////////////////////////
