#include <libec/monitor/feeder/MonitorFeeder.h>

namespace cea
{

  /** +Destructor */
  MonitorFeeder::~MonitorFeeder()
  {
    clearMonitor();
  }

  /* +connectMonitor */
  void
  MonitorFeeder::connectMonitor(Monitor& m, bool modeRow)
  {
    if (modeRow)
      {
        /* Check if is not already present */
        for (MonitorList::iterator it = _rowMonitors.begin();
            it != _rowMonitors.end(); ++it)
          {
            if ((*it) == &m)
              return;
          }
        /* Add if not present */
        _rowMonitors.push_back(&m);
      }
    else
      {
        /* Check if is not already present in columns-monitor */
        for (MonitorList::iterator it = _columnMonitors.begin();
            it != _columnMonitors.end(); ++it)
          {
            if ((*it) == &m)
              return;
          }
        /* Add if not present */
        _columnMonitors.push_back(&m);
      }
  }

  /** +disconnectMonitor */
  void
  MonitorFeeder::disconnectMonitor(Monitor& m)
  {
    /* Find the monitor on row and erase it */
    for (MonitorList::iterator it = _rowMonitors.begin();
        it != _rowMonitors.end(); ++it)
      {
        if ((*it) == &m)
          {
            _rowMonitors.erase(it);
            break;
          }
      }
    /* Find the monitor on column and erase it */
    for (MonitorList::iterator it = _columnMonitors.begin();
        it != _columnMonitors.end(); ++it)
      {
        if ((*it) == &m)
          {
            _columnMonitors.erase(it);
            break;
          }
      }
  }

  /** +clearMonitor */
  void
  MonitorFeeder::clearMonitor()
  {
    /* For each row-monitor remove this feeder */
    for (MonitorList::iterator it = _rowMonitors.begin();
        it != _rowMonitors.end();)
      {
        /* Call the autoDisconnectFeeder on monitor */
        (*it)->autoDisconnectFeeder(this);
        /* Remove the monitor */
        it = _rowMonitors.erase(it);
      }
    /* For each column-monitor remove this feeder */
    for (MonitorList::iterator it = _columnMonitors.begin();
        it != _columnMonitors.end();)
      {
        /* Call the autoDisconnectFeeder on monitor */
        (*it)->autoDisconnectFeeder(this);
        /* Remove the monitor */
        it = _columnMonitors.erase(it);
      }
  }

  /* #feedCreateItem */
  void
  MonitorFeeder::feedCreateItem(FeederType itemType, void * element)
  {
    /* On each row-monitor create the row */
    for (MonitorList::iterator it = _rowMonitors.begin();
        it != _rowMonitors.end(); ++it)
      {
        (*it)->onFeedCreateRow((signed int) (itemType), element);
      }
    /* On each column-monitor create the row */
    for (MonitorList::iterator it = _columnMonitors.begin();
        it != _columnMonitors.end(); ++it)
      {
        (*it)->onFeedCreateColumn((signed int) (itemType), element);
      }
  }

  /* #feedUpdateItem */
  void
  MonitorFeeder::feedUpdateItem(FeederType itemType, void * element)
  {
    /* On each row-monitor update the row */
    for (MonitorList::iterator it = _rowMonitors.begin();
        it != _rowMonitors.end(); ++it)
      {
        (*it)->onFeedUpdateRow((signed int) (itemType), element);
      }
    /* On each column-monitor update the row */
    for (MonitorList::iterator it = _columnMonitors.begin();
        it != _columnMonitors.end(); ++it)
      {
        (*it)->onFeedUpdateColumn((signed int) (itemType), element);
      }
  }

  /* #feedDeleteRow */
  void
  MonitorFeeder::feedDeleteItem(FeederType itemType, void * element)
  {
    /* On each row-monitor update the row */
    for (MonitorList::iterator it = _rowMonitors.begin();
        it != _rowMonitors.end(); ++it)
      {
        (*it)->onFeedDeleteRow((signed int) (itemType), element);
      }
    /* On each column-monitor update the row */
    for (MonitorList::iterator it = _columnMonitors.begin();
        it != _columnMonitors.end(); ++it)
      {
        (*it)->onFeedDeleteColumn((signed int) (itemType), element);
      }
  }

}
