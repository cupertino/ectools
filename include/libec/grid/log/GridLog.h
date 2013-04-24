///////////////////////////////////////////////////////////////////////////////
/// @file		GridLog.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Log implementation of a grid
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDLOG_H__
#define LIBEC_GRIDLOG_H__

#include <iostream>
#include <time.h>

#include "../view/GridView.h"
#include "../model/GridModel.h"

#include "../../log/Log.h"

#include "../filter/GridFilter.h"

namespace cea
{

  /// @brief Log implementation of a grid
  class GridLog : public Log
  {
  public:

    /* Constructor */
    /// @brief Default Constructor
    GridLog();
    /// @brief Create a GridLog with specific logger
    /// @param logType Logger used for logging
    GridLog(const BaseLog& logType);
    /// @brief Create a GridLog with specific logger and format
    /// @param logType Logger used for logging
    /// @param logFormat Format used for logging
    GridLog(const BaseLog& logType, const BaseFormat& logFormat);

    /* Log - Function */
    /// @brief Log a row from a GridModel
    ///
    /// Log only cells from column accepted by filter
    /// (see GridLog::setLogFilter)
    ///
    /// @param row GridRow to log
    /// @param type Block Type for format
    ///				(see BaseFormatter::openBlockType)
    /// @see setLogFilter, BaseFormatter::openBlockType
    void
    log(GridRow* row, FormatBlockType type = ITEM_DEFAULT);

    /// @brief Log a row from a GridView
    ///
    /// Log only cells from column accepted by filter
    /// (see GridLog::setLogFilter)
    ///
    /// @param view GridView container of the row
    /// @param row GridView::RowView to log
    /// @param type Block Type for format (see BaseFormatter::openBlockType)
    /// @see setLogFilter, BaseFormatter::openBlockType
    void
    log(GridView* view, GridView::RowView* row, FormatBlockType type =
        ITEM_DEFAULT);

    /// @brief Log all row from a GridView
    ///
    /// Log only cells of rows:columns accepted by filter (see setLogFilter)
    ///
    /// @param view GridView to log
    /// @see setLogFilter
    void
    log(GridView* view);

    /// @brief Log all cells associated with a column from a GridModel
    ///
    /// Log only cells accepted by filter (see GridLog::setLogFilter)
    ///
    /// @param col GridColumn to log
    /// @see setLogFilter
    void
    log(GridColumn* col);

    /* Filter - Property */
    /// @brief Set log filter
    ///
    /// This filter is used to select which rows/columns/cells to log. \n
    /// For details on Filter format see GridFilter. \n \n
    /// If no filter is wanted set filter param to 0.
    ///
    /// @param filter Pointer to a GridFilter each time you log an element
    ///					make sure that the filter is refresh
    ///					(see GridFilter::refresh).
    /// @see GridFilter
    void
    setLogFilter(GridFilter* filter);

    /// @brief Get the log filter
    /// @return Pointer to GridFilter used.
    GridFilter*
    getLogFilter() const;

  protected:

    /* Members */
    GridFilter* _filter; ///< Filter used for log by default no filter (=0)

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridLog
///	@ingroup grid
///
///////////////////////////////////////////////////////////////////////////////
