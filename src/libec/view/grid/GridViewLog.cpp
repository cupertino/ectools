#include <libec/grid/view/GridViewLog.h>
#include <libec/grid/view/GridView.h>
#include <libec/grid/log/GridLog.h>
#include <libec/grid/filter/GridFilter.h>

namespace cea
{

  /** +Constructor */
  GridViewLog::GridViewLog() :
      _log(*(new GridLog)), _isLogActivate(false), _logFilter(*(new GridFilter))
  {
    _log.setLogFilter(&_logFilter);
  }

  /** +Destructor */
  GridViewLog::~GridViewLog()
  {
    delete (&_logFilter);
    delete (&_log);
  }

  /** +getLog */
  GridLog&
  GridViewLog::getLog()
  {
    return _log;
  }
  /** +setLog */
  void
  GridViewLog::setLog(const BaseLog& logType, bool activate)
  {
    _log.set(logType);
    _isLogActivate = true;
  }
  /** +setLog */
  void
  GridViewLog::setLog(const BaseFormat& logFormat, bool activate)
  {
    _log.set(logFormat);
    _isLogActivate = true;
  }
  /** +setLog */
  void
  GridViewLog::setLog(const BaseLog& logType, const BaseFormat& logFormat,
      bool activate)
  {
    _log.set(logType, logFormat);
    _isLogActivate = activate;
  }
  /** +setLog */
  void
  GridViewLog::setLog(bool activate)
  {
    _isLogActivate = true;
  }
  /** +logAll */
  void
  GridViewLog::logAll()
  {
    GridView* grd = getGridViewLogRef();
    if (grd != 0)
      {
        _log.log(grd);
      }
  }
  /** +logFlush */
  void
  GridViewLog::logFlush()
  {
    _log.flush();
  }

  /** +isLogActivate */
  bool
  GridViewLog::isLogActivate() const
  {
    return _isLogActivate;
  }
  /** +activateLog */
  void
  GridViewLog::activateLog(bool activate)
  {
    _isLogActivate = activate;
  }

  /** +setLogFilter */
  void
  GridViewLog::setLogFilter(const std::string& filter)
  {
    _logFilter.set(filter);
    refreshLogFilter();
  }

  /** +getLogFilter */
  std::string
  GridViewLog::getLogFilter() const
  {
    return _logFilter.get();
  }

  /** +refreshLogFilter */
  void
  GridViewLog::refreshLogFilter()
  {
    GridView* grd = getGridViewLogRef();
    if (grd != 0)
      {
        grd->refreshAFilter(_logFilter);
      }
  }

}
