///////////////////////////////////////////////////////////////////////////////
/// @file		MonitorFeeder.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base monitor feeder used to add Row or Column on Monitor
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MONITORFEEDER_H__
#define LIBEC_MONITORFEEDER_H__

#include <list>
#include "FeederType.h"

#include "../Monitor.h"

namespace cea
{

  /// @brief Base monitor feeder used to add Row or Column on Monitor
  class MonitorFeeder
  {
  public:

    /* Destructor */
    /// @brief Deconnect all connected Monitor
    virtual
    ~MonitorFeeder();

    /// @brief Deconnect all Monitor
    void
    clearMonitor();

    /* Friend class */
    friend class Monitor;

  protected:

    /// @brief Add an item to all connected monitors
    ///
    /// This function is used to feed all monitor
    ///
    /// @param itemType Tag of item added
    /// @param element Generic pointer to map to added item in monitors
    void
    feedCreateItem(FeederType itemType, void* element);

    /// @brief Update an item to all connected monitors
    ///
    /// This function is used to update feeded item in connected monitors
    ///
    /// @param itemType Tag of item added
    /// @param element Generic pointer concerned
    void
    feedUpdateItem(FeederType itemType, void* element);

    /// @brief Delete an item to all connected monitors
    ///
    /// This function is used to deleted feeded item in connected monitors
    ///
    /// @param itemType Tag of item added
    /// @param element Generic pointer concerned
    void
    feedDeleteItem(FeederType itemType, void* element);

  private:

    /// @brief Connect a monitor for row or column feeding
    /// @param m Monitor to connect
    /// @param modeRow If true Then row feeding
    ///				   Else column feeding
    void
    connectMonitor(Monitor& m, bool modeRow = true);
    /// @brief Deconnect a monitor
    /// @param m Monitor to deconnect
    void
    disconnectMonitor(Monitor& m);

    /// @brief Monitor list
    typedef std::list<Monitor*> MonitorList;

    /* Members */
    /// @brief List of connected Monitors for row feeding
    MonitorList _rowMonitors;
    /// @brief List of connected Monitors for column feeding
    MonitorList _columnMonitors;
  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::MonitorFeeder
///	@ingroup monitor
///
/// This class provide a simple system to manage identical item in a Monitor.
/// 
/// For example BaseProcessEnumerator is a kind of MonitorFeeder.
///
///////////////////////////////////////////////////////////////////////////////
