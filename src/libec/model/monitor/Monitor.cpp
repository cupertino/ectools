#include <libec/monitor/Monitor.h>
#include <libec/monitor/feeder/MonitorFeeder.h>

#include <libec/grid/controller/terminal/TermGridCtrl.h>
#include <libec/grid/view/html/HTMLGridView.h>
#include <libec/grid/view/html/HTMLChartGridView.h>
#include <libec/grid/view/terminal/TermGridView.h>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /* Constructor/Destructor */

  /** +Construtor */
  Monitor::Monitor() :
      _frequency(1000), _lastTick(0)
  {
    eventMode = true;
  }
  Monitor::Monitor(cea_time_t frequency) :
      _frequency(frequency), _lastTick(0)
  {
    eventMode = true;
  }

  /** +Destructor */
  Monitor::~Monitor()
  {
    /* Clear Feeder */
    /* Delete in Column feeders */
    for (FeederList::iterator it = _columnFeeders.begin();
        it != _columnFeeders.end(); ++it)
      {
        (*it)->disconnectMonitor(*this);
      }
    /* Delete in Row feeders */
    for (FeederList::iterator it = _rowFeeders.begin(); it != _rowFeeders.end();
        ++it)
      {
        (*it)->disconnectMonitor(*this);
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Update section  */

  /** +getFrequency */
  cea_time_t
  Monitor::getFrequency() const
  {
    return _frequency;
  }
  /** +setFrequency */
  void
  Monitor::setFrequency(cea_time_t freq)
  {
    _frequency = freq;
  }

  /** #needUpdate */
  bool
  Monitor::needUpdate() const
  {
    /* Check the Frequency if there is a frequency */
    if (_frequency > 0)
      {
        /* Check the frequency */
        if (Tools::tick() - _lastTick < _frequency)
          {
            return false;
          }
      }
    /* Update needed */
    return true;
  }

  /** +update */
  void
  Monitor::update()
  {
    /* Check the Frequency if there is a frequency */
    if (needUpdate())
      {
        /* Keep the new tick */
        _lastTick = Tools::tick();
        /* Do Update */
        onUpdate();
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Feeder section  */

  /** +connectRowFeeder */
  void
  Monitor::connectRowFeeder(MonitorFeeder* feeder)
  {
    if (feeder != 0)
      {
        feeder->connectMonitor(*this, true);
      }
  }

  /** +connectColumnFeeder */
  void
  Monitor::connectColumnFeeder(MonitorFeeder* feeder)
  {
    if (feeder != 0)
      {
        feeder->connectMonitor(*this, false);
      }
  }
  /** +disconnectFeeder */
  void
  Monitor::disconnectFeeder(MonitorFeeder* feeder)
  {
    if (feeder != 0)
      {
        feeder->disconnectMonitor(*this);
      }
  }

  /** +clearFeeder */
  void
  Monitor::clearFeeder()
  {
    /* Delete in Column feeders */
    for (FeederList::iterator it = _columnFeeders.begin();
        it != _columnFeeders.end(); /*NOP*/)
      {
        (*it)->disconnectMonitor(*this);
        it = _columnFeeders.erase(it);
      }
    /* Delete in Row feeders */
    for (FeederList::iterator it = _rowFeeders.begin(); it != _rowFeeders.end();
    /*NOP*/)
      {
        (*it)->disconnectMonitor(*this);
        it = _rowFeeders.erase(it);
      }
  }

  /** #autoDisconnectFeeder */
  void
  Monitor::autoDisconnectFeeder(MonitorFeeder* feeder)
  {
    /* Delete in Column feeders */
    for (FeederList::iterator it = _columnFeeders.begin();
        it != _columnFeeders.end(); ++it)
      {
        if ((*it) == feeder)
          {
            _columnFeeders.erase(it);
            break;
          }
      }
    /* Delete in Row feeders */
    for (FeederList::iterator it = _rowFeeders.begin(); it != _rowFeeders.end();
        ++it)
      {
        if ((*it) == feeder)
          {
            _rowFeeders.erase(it);
            return;
          }
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Controller Section */

  /** +handleEvent */
  void
  Monitor::handleEvent(const GridCtrl::Event& e)
  {
    for (ControllerList::iterator c = loadedControllers.begin();
        c != loadedControllers.end(); ++c)
      {
        (*c)->handleEvent(e);
      }
  }

  /* -------------------------------------------------------------------------- */
  /* View Section */

  void
  Monitor::renderViews()
  {
    bool bHaveRendered = false;
    /* Render views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((*v)->needRender())
          {
            /* Call Before renderViews once time */
            if (!bHaveRendered)
              {
                beforeRenderViews();
                bHaveRendered = true;
              }
            /* Render view */
            (*v)->refreshAutoSize();
            (*v)->render();
          }
      }
    /* Call After renderViews */
    if (bHaveRendered)
      {
        afterRenderViews();
      }
  }

  void
  Monitor::beforeRenderViews()
  {
    /* DO NOTHING - By default */;
  }

  void
  Monitor::afterRenderViews()
  {
    /* DO NOTHING - By default */;
  }

  /* -------------------------------------------------------------------------- */
  /* Row/Column mapped section  */

  Monitor::Column&
  Monitor::addMappedColumn(void* linkObject, const std::string& title,
      Value::Type type, int tag, unsigned int minimumSize,
      unsigned int percentWidth)
  {
    /* Find if not already mapped */
    ColumnMap::iterator it = mappedColumns.find(linkObject);
    if (it == mappedColumns.end())
      {
        /* Add the mapped column */
        GridColumn& col = addColumn(title, type, tag, minimumSize,
            percentWidth);
        if (&col != &ncolumn)
          {
            /* Set Data */
            setGridItemData(col, linkObject);
            /* Map the object */
            mappedColumns.insert(ColumnMap::value_type(linkObject, &col));
            /* Return the current column mapped */
            return col;
          }
        else
          {
            return ncolumn;
          }
      }
    else
      {
        /* Return the current mapped column */
        return *(it->second);
      }
  }

  Monitor::Column&
  Monitor::getMappedColumn(void* linkObject)
  {
    /* Find the corresponding columns mapped */
    ColumnMap::iterator it = mappedColumns.find(linkObject);
    if (it != mappedColumns.end())
      {
        return *(it->second);
      }
    else
      {
        return ncolumn;
      }
  }

  Monitor::Row&
  Monitor::addMappedRow(void* linkObject, int tag)
  {
    /* Find if not already mapped */
    RowMap::iterator it = mappedRows.find(linkObject);
    if (it == mappedRows.end())
      {
        /* Add the mapped row */
        GridRow& row = addRow(tag);
        if (&row != &nrow)
          {
            /* Set Data */
            setGridItemData(row, linkObject);
            /* Map the object */
            mappedRows.insert(RowMap::value_type(linkObject, &row));
            /* Return the current row mapped */
            return row;
          }
        else
          {
            return nrow;
          }
      }
    else
      {
        /* Return the current mapped column */
        return *(it->second);
      }
  }

  Monitor::Row&
  Monitor::getMappedRow(void* linkObject)
  {
    /* Find the corresponding rows mapped */
    RowMap::iterator it = mappedRows.find(linkObject);
    if (it != mappedRows.end())
      {
        return *(it->second);
      }
    else
      {
        return nrow;
      }
  }

  void
  Monitor::onGridRowDelete(GridRow& r)
  {
    /* Erase from mapped row */
    mappedRows.erase(r.getData());
  }

  void
  Monitor::onGridRowClear()
  {
    mappedRows.clear();
  }

  void
  Monitor::onGridColumnDelete(GridColumn& c)
  {
    /* Erase from mapped row */
    mappedColumns.erase(c.getData());
  }

  void
  Monitor::onGridColumnClear()
  {
    mappedColumns.clear();
  }

  /* -------------------------------------------------------------------------- */
  /* Argument section */

  const Value&
  Monitor::getArg(const Monitor::Column& col, unsigned int id)
  {
    return col.getArg(id);
  }

  const Value&
  Monitor::getArg(const Monitor::Row& row, unsigned int id)
  {
    return row.getArg(id);
  }

  Monitor&
  Monitor::setArg(Monitor::Column& col, unsigned int id, const Value& value)
  {
    col.setArg(id, value);
    return *this;
  }

  Monitor&
  Monitor::setArg(Monitor::Row& row, unsigned int id, const Value& value)
  {
    row.setArg(id, value);
    return *this;
  }

  /* -------------------------------------------------------------------------- */
  /* Feeder Function section */

  void
  Monitor::onFeedCreateRow(int tag, void* obj)
  {
    Row& r = addMappedRow(obj, tag);
    /* Raise event */
    if (&r != &nrow)
      {
        onRowCreate(r);
      }
  }

  void
  Monitor::onFeedUpdateRow(int tag, void* obj)
  {
    /* Find the row */
    RowMap::iterator it = mappedRows.find(obj);
    /* Update it */
    if (it != mappedRows.end())
      {
        onRowUpdate(*(it->second));
      }
    else
      {
        /* Add the rows */
        onFeedCreateRow(tag, obj);
      }
  }

  void
  Monitor::onFeedDeleteRow(int tag, void* obj)
  {
    /* Find the Row */
    RowMap::iterator it = mappedRows.find(obj);
    if (it != mappedRows.end())
      {
        /* Raise event */
        onRowDelete(*(it->second));
        /* Erase the row */
        removeRow(*(it->second));
      }
  }

  void
  Monitor::onFeedCreateColumn(int tag, void* obj)
  {
    /* BY default add a column INT */
    Column& c = addMappedColumn(obj, "UNKNOWN", Value::INT, tag, 0);
    /* Raise event */
    if (&c != &ncolumn)
      {
        onColumnCreate(c);
      }
  }

  void
  Monitor::onFeedUpdateColumn(int tag, void* obj)
  {
    /* Find the column */
    ColumnMap::iterator it = mappedColumns.find(obj);
    /* Update it */
    if (it != mappedColumns.end())
      {
        onColumnUpdate(*(it->second));
      }
    else
      {
        /* Add the rows */
        onFeedCreateColumn(tag, obj);
      }
  }

  void
  Monitor::onFeedDeleteColumn(int tag, void* obj)
  {
    /* Find the Column */
    ColumnMap::iterator it = mappedColumns.find(obj);
    if (it != mappedColumns.end())
      {
        /* Raise event */
        onColumnDelete(*(it->second));
        /* Erase the row */
        removeColumn(*(it->second));
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Monitor event - section */

  void
  Monitor::onUpdate()
  {
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        if (_filter.applyFilter(*(*r)))
          {
            onRowUpdate(*(*r));
          }
      }
  }

  void
  Monitor::onRowCreate(Row& r)
  {
    /* DO NOTHING - By default */
  }

  void
  Monitor::onRowLoaded(Row& r)
  {
    onRowCreate(r);
  }

  void
  Monitor::onRowUpdate(Row& r)
  {
    /* DO NOTHING - By default */
  }

  void
  Monitor::onRowDelete(Row& r)
  {
    /* DO NOTHING - By default */
  }

  void
  Monitor::onColumnCreate(Column& c)
  {
    /* DO NOTHING - By default */
  }

  void
  Monitor::onColumnUpdate(Column& c)
  {
    /* DO NOTHING - By default */
  }

  void
  Monitor::onColumnDelete(Column& c)
  {
    /* DO NOTHING - By default */
  }

  /* ----------------------------------------------------------------------- */
  /* MonitorLoader function */

  void
  Monitor::connectLoadedView(GridView* loadedView)
  {
    connectView(*loadedView);
    loadedViews.push_back(loadedView);
  }

  void
  Monitor::connectLoadedController(GridCtrl* loadedCtrl, GridView& loadedView)
  {
    loadedCtrl->connectToView(loadedView);
    loadedControllers.push_back(loadedCtrl);
  }

  Monitor&
  Monitor::getMonitorRef()
  {
    return *this;
  }

}
