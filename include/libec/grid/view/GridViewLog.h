///////////////////////////////////////////////////////////////////////////////
/// @file		GridViewLog.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Log implementation of the GridView
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDVIEWLOG_H__
#define LIBEC_GRIDVIEWLOG_H__

#include <iostream>
#include <map>
#include <list>

#include "../../log/Log.h"

namespace cea
{

  /* Prototype */
  class GridLog;
  class GridView;
  class GridFilter;

  /// @brief Log implementation of the GridView
  class GridViewLog
  {
  public:

    /* Constructor/Destructor */
    /// @brief Default constructor
    GridViewLog();

    /// @brief Delete all created memory
    virtual
    ~GridViewLog();

    /* Log class property */
    /// @brief Get GridLog class
    ///
    /// This function is usefull to configurate some log options.
    /// (see GridLog)
    ///
    /// @return The GridLog class used by GridViewLog
    GridLog&
    getLog();
    /// @brief Set log type and active the log system
    /// @param logType Type of the log
    /// @param activate Activate the log
    /// @see GridLog::set
    void
    setLog(const BaseLog& logType, bool activate = true);
    /// @brief Set log type and active the log system
    /// @param logFormat Format of the log
    /// @param activate Activate the log
    /// @see GridLog::set
    void
    setLog(const BaseFormat& logFormat, bool activate = true);
    /// @brief Set log type and active the log system
    /// @param logType Type of the log
    /// @param logFormat Format of the log
    /// @param activate Activate the log
    /// @see GridLog::set
    void
    setLog(const BaseLog& logType, const BaseFormat& logFormat, bool activate =
        true);

    /// @brief Active/deactivate the log system
    /// @param activate Log system activation status
    void
    setLog(bool activate = true);
    /// @brief Log all the grid view
    /// @see GridLog::log
    void
    logAll();
    /// @brief Flush log
    /// @see GridLog::flush
    void
    logFlush();

    /* Log Option */
    /// @brief Check is log activate
    /// @return true if log activate
    bool
    isLogActivate() const;
    /// @brief Activate/deactivate the log system
    /// @param activate Activate the log
    void
    activateLog(bool activate);

    /// @brief Set filter log
    /// @param filter Filter in specific formatted string
    /// @see GridFilter::set
    void
    setLogFilter(const std::string& filter);

    /// @brief Get log filter
    /// @return the log filter
    std::string
    getLogFilter() const;

    /// @brief Refresh Log Filter
    /// @see GridFilter::refresh
    void
    refreshLogFilter();

  protected:

    /* Protected - Members */

    /// @brief Log integration
    /// @return Pointer of this current GridView
    virtual GridView*
    getGridViewLogRef() = 0;

    GridLog& _log; ///< Log system
    bool _isLogActivate; ///< Indicate if log is activate

    GridFilter& _logFilter; ///< Log filter

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridViewLog
///	@ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
