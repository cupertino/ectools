#include <libec/grid/view/GridView.h>

#include <libec/grid/filter/GridFilter.h>
#include <libec/grid/log/GridLog.h>

#include <libec/tools/Color.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  /* ------------------------------------------------------------------------ */
  /* GridView::AutoSize */

  GridView::AutoSize::AutoSize() :
      autoWidth(true), autoHeight(true), offsetW(0), offsetH(0)
  {
    ;
  }

  GridView::AutoSize::AutoSize(int offsetW, int offsetH) :
      autoWidth(true), autoHeight(true), offsetW(offsetW), offsetH(offsetH)
  {
    ;
  }

  GridView::AutoSize::AutoSize(bool autoWidth, bool autoHeight) :
      autoWidth(autoWidth), autoHeight(autoHeight), offsetW(0), offsetH(0)
  {
    ;
  }

  GridView::AutoSize::AutoSize(bool autoWidth, int offsetW, bool autoHeight,
      int offsetH) :
      autoWidth(autoWidth), autoHeight(autoHeight), offsetW(offsetW), offsetH(
          offsetH)
  {
    ;
  }

  /* ------------------------------------------------------------------------ */
  /* GridView - Static variable */

  /* Static invalid member */
  GridStyle GridView::nstyle;

  /* Static value - Default Style */
  GridStyle GridView::defaultColumnStyle;
  GridStyle GridView::defaultColumnHeaderStyle(GridStyle::CENTER, Color::Black,
      Color::DarkGray, true);
  GridStyle GridView::defaultColumnSumStyle(GridStyle::RIGHT, Color::Black,
      Color::DarkGray, true);
  GridStyle GridView::defaultRowStyle;

  /* ------------------------------------------------------------------------ */
  /* Constructor */

  /** +constructor */
  GridView::GridView() :
      left(0), top(0), _width(1), _height(10), _sortEnabled(false), _sortRefColumn(
          0), _sortOrder(ASCENDING), _sortNeeded(false), _eventMode(false), _lastUID(
          0), _updateCounter(1), _lastRenderUpdateCounter(0), _sumColumnEnableTotal(
          0), _firstRowView(0), _firstColumnView(0), _renderAllItem(true), _filter(
          *(new GridFilter)), _headerAdditionalLine(false), _sumAdditionalLine(
          false)
  {
    ;
  }
  GridView::GridView(GridModel& model) :
      left(0), top(0), _width(1), _height(10), _sortEnabled(false), _sortRefColumn(
          0), _sortOrder(ASCENDING), _sortNeeded(false), _eventMode(false), _lastUID(
          0), _updateCounter(1), _lastRenderUpdateCounter(0), _sumColumnEnableTotal(
          0), _firstRowView(0), _firstColumnView(0), _renderAllItem(true), _filter(
          *(new GridFilter)), _headerAdditionalLine(false), _sumAdditionalLine(
          false)
  {
    model.connectView(*this);
  }

  /** Destructor */
  GridView::~GridView()
  {
    delete (&_filter);
    /* Clear all mapped row */
    for (RowMap::iterator it = rowsMap.begin(); it != rowsMap.end(); ++it)
      {
        delete it->second;
      }
    /* Clear all mapped column */
    for (ColumnMap::iterator it = columnsMap.begin(); it != columnsMap.end();
        ++it)
      {
        delete it->second;
      }
    /* Clear all style */
    for (StyleMap::iterator s = stylesMap.begin(); s != stylesMap.end(); ++s)
      {
        delete s->second;
      }
  }

  /** #getGridViewLogRef */
  GridView*
  GridView::getGridViewLogRef()
  {
    return this;
  }

  /** #getGridViewLoaderRef */
  GridView*
  GridView::getGridViewLoaderRef()
  {
    return this;
  }

  /* -------------------------------------------------------------------------- */
  /* GridModel function */

  /** -addColumn */
  void
  GridView::addColumn(GridColumn* c)
  {
    /* Check if column already exist */
    ColumnMap::iterator colMapped = columnsMap.find(c);
    /* Add column */
    if (colMapped == columnsMap.end())
      {
        /* Create the column */
        ColumnView* colView = new ColumnView();
        if (colView != 0)
          {
            /* Set Option */
            colView->uid = ++_lastUID;
            colView->visible = true;
            colView->ref = c;
            colView->fixed = false;
            colView->fixedAtEnd = false;
            colView->fixedOffset = 0;
            colView->style = &defaultColumnStyle;
            colView->headerStyle = &defaultColumnHeaderStyle;
            colView->sumStyle = &defaultColumnSumStyle;
            colView->sumEnable = false;
            colView->needSumRecalc = false;
            /* Map Item */
            colView->it = columns.push_back(colView);
            columnsMap.insert(ColumnMap::value_type(c, colView));
            /* Set Style */
            privSetColumnStyle(colView, c->tag);
            /* SmartResize Column */
            smartResizeColumns();
            /* Set the scroll if first item */
            if (_firstColumnView == 0)
              {
                _firstColumnView = colView;
              }
            /* Raise event if event mode */
            if (_eventMode)
              {
                ColumnTable::iterator after = columns.back();
                if (after != 0)
                  {
                    onColumnAdded(*colView, after->value);
                  }
                else
                  {
                    onColumnAdded(*colView, 0);
                  }
              }
            /* Increment counter */
            _updateCounter++;
            /* Update Filters */
            refreshFilters();
            /* Check if item need to be fixed */
            if (c->isFixed())
              {
                fixColumn(*c, c->isFixedAtEnd(), c->getFixedOffset());
              }
          }
      }
  }
  /** -removeColumn */
  void
  GridView::removeColumn(GridColumn* c)
  {
    /* Find the ColumnView in the map */
    ColumnMap::iterator colMapped = columnsMap.find(c);
    /* Remove the column */
    if (colMapped != columnsMap.end())
      {
        /* Retrieve the ColumnView associated */
        ColumnView* colView = colMapped->second;
        /* Check if columnview have sum enable */
        if (colView->sumEnable)
          _sumColumnEnableTotal--;
        /* Raise event if event mode */
        if (_eventMode)
          onColumnDelete(*colView);
        /* Change the scroll if column is _firstColumnView */
        if (_firstColumnView == colView)
          {
            if (colView->it == columns.begin())
              {
                if (colView->it->next != 0)
                  {
                    _firstColumnView = colView->it->next->value;
                  }
                else
                  {
                    _firstColumnView = 0;
                  }
              }
            else
              {
                if (colView->it->prev != 0)
                  {
                    _firstColumnView = colView->it->prev->value;
                  }
                else
                  {
                    _firstColumnView = 0;
                  }
              }
          }
        /* Remove from list */
        if (colView->it != 0)
          {
            columns.erase(colView->it);
          }
        /* Delete from memory */
        delete colView;
        /* Delete from map */
        columnsMap.erase(colMapped);
        /* Delete from fixed map */
        fixedColumns.erase(colView);
        /* Check if the column is the sort ref column */
        if (_sortRefColumn == c)
          {
            _sortRefColumn = 0;
            _sortEnabled = true;
          }
        /* SmartResize Column */
        smartResizeColumns();
        /* Increment counter */
        _updateCounter++;
        /* Refresh filters */
        refreshFilters();
      }
  }
  /** -clearColumn */
  void
  GridView::clearColumn()
  {
    if (columns.size() > 0)
      {
        /* Raise event if event mode */
        if (_eventMode)
          onColumnClear();
        /* Clear all mapped column */
        for (ColumnMap::iterator it = columnsMap.begin();
            it != columnsMap.end(); ++it)
          {
            delete it->second;
          }
        /* Clear all ColumnView in list */
        columns.clear();
        columnsMap.clear();
        fixedColumns.clear();
        /* Increment Counter */
        _updateCounter++;
      }
    _sortEnabled = false;
    _sumColumnEnableTotal = 0;
    _firstColumnView = 0;
    /* Refresh filters */
    refreshFilters();
  }

  /** -addRow */
  void
  GridView::addRow(GridRow* r)
  {
    /* Check if row already exist */
    RowMap::iterator rowMapped = rowsMap.find(r);
    /* Add row */
    if (rowMapped == rowsMap.end())
      {
        /* Create the row */
        RowView* rowView = new RowView();
        if (rowView != 0)
          {
            /* Set Option */
            rowView->uid = ++_lastUID;
            rowView->ref = r;
            rowView->style = &defaultRowStyle;
            rowView->styleFromFilter = 0;
            rowView->fixed = false;
            rowView->fixedAtEnd = false;
            rowView->fixedOffset = 0;
            /* Change the scroll if row is the first one */
            if (_firstRowView == 0)
              {
                _firstRowView = rowView;
              }
            /* Search the row after position */
            RowView* rowInsertAfter = 0; //getRowViewOrderedAfter(rowView);
            /* Map Item */
            if ((rowInsertAfter == 0) || (rowView == rowInsertAfter))
              {
                rowView->it = rows.push_back(rowView);
              }
            else
              {
                rowView->it = rows.insert(rowView, rowInsertAfter->it);
              }
            if (_sortEnabled)
              _sortNeeded = true;
            rowsMap.insert(RowMap::value_type(r, rowView));
            /* Raise event if event mode */
            if (_eventMode)
              onRowAdded(*rowView, rowInsertAfter);
            /* Increment counter */
            _updateCounter++;
            /* Check if visible */
            rowView->visible = applyFilter(rowView);
            /* Increment all Column Sum if row visible */
            if ((rowView->visible) && (_sumColumnEnableTotal > 0))
              {
                incSumColumn(rowView);
              }
            /* Log Item */
            if ((rowView->visible) && (_isLogActivate))
              {
                _log.log(this, rowView, ITEM_ADDED);
              }
            /* Set Style */
            privSetRowStyle(rowView, r->tag);
            /* Check if item need to be fixed */
            if (r->isFixed())
              {
                fixRow(*r, r->isFixedAtEnd(), r->getFixedOffset());
              }
          }
      }
  }
  /** -removeRow */
  void
  GridView::removeRow(GridRow* r)
  {
    /* Find the RowView in the map */
    RowMap::iterator rowMapped = rowsMap.find(r);
    /* Remove the Row */
    if (rowMapped != rowsMap.end())
      {
        /* Retrieve the ColumnView associated */
        RowView* rowView = rowMapped->second;
        /* Log Item */
        if ((rowView->visible) && (_isLogActivate))
          {
            _log.log(this, rowView, ITEM_ERASED);
          }
        /* Increment all Column Sum */
        if (_sumColumnEnableTotal > 0)
          decSumColumn(rowView);
        /* Raise event if event mode */
        if (_eventMode)
          onRowDelete(*rowView);
        /* Change the scroll if row is _firstRowView */
        if (_firstRowView == rowView)
          {
            if (rowView->it == rows.begin())
              {
                if (rowView->it->next != 0)
                  {
                    _firstRowView = rowView->it->next->value;
                  }
                else
                  {
                    _firstRowView = 0;
                  }
              }
            else
              {
                if (rowView->it->prev != 0)
                  {
                    _firstRowView = rowView->it->prev->value;
                  }
                else
                  {
                    _firstRowView = 0;
                  }
              }
          }
        /* Remove the RowView in the list */
        if (rowView->it != 0)
          {
            rows.erase(rowView->it);
          }
        /* Delete from memory */
        delete rowView;
        /* Delete from map */
        rowsMap.erase(rowMapped);
        /* Delete from fixed map */
        fixedRows.erase(rowView);
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** -clearRow */
  void
  GridView::clearRow()
  {
    if (rows.size() > 0)
      {
        /* Raise event if event mode */
        if (_eventMode)
          onRowClear();
        /* Clear all mapped row */
        for (RowMap::iterator it = rowsMap.begin(); it != rowsMap.end(); ++it)
          {
            delete it->second;
          }
        /* Clear all RowView in list */
        rows.clear();
        rowsMap.clear();
        fixedRows.clear();
        clearAllSumColumn();
        /* Increment Counter */
        _updateCounter++;
      }
    _firstRowView = 0;
  }

  /** -refreshCell */
  void
  GridView::refreshCell(GridRow* row, GridColumn* col, const Value& oldValue,
      const Value& newValue)
  {
    /* Associated Row/Column View */
    RowMap::iterator itr = rowsMap.find(row);
    if (itr == rowsMap.end())
      return;
    ColumnMap::iterator itc = columnsMap.find(col);
    if (itc == columnsMap.end())
      return;
    RowView* r = itr->second;
    ColumnView* c = itc->second;
    if ((r == 0) || (c == 0))
      return;
    /* Filter the row */
    privSetRowVisibility(r, applyFilter(r));
    /* Log Item */
    if ((r->visible) && (_isLogActivate))
      {
        _log.log(this, r, ITEM_UPDATED);
      }
    /* Recalcul the Column sum */
    if (_sumColumnEnableTotal > 0)
      {
        if ((r->visible) && (row->isSumConsidered()))
          {
            decSumColumn(c, oldValue);
            incSumColumn(c, newValue);
          }
      }
    /* Resort the row if sort activated */
    if ((_sortEnabled) && (col == _sortRefColumn))
      {
        _sortNeeded = true;
      }
    /* Raise event if event mode */
    if (_eventMode)
      onCellChange(*r, *c, newValue);
    /* Increment Counter */
    _updateCounter++;
  }

  /** -refreshColumn */
  void
  GridView::refreshColumn(GridColumn* col)
  {
    smartResizeColumns();
  }

  /** -sortARow */
  void
  GridView::sortARow(RowView* r)
  {
    if (_sortEnabled)
      {
        /* Find the good position */
        RowView* afterRow = getRowViewOrderedAfter(r);
        /* Move the row */
        if (r != afterRow)
          {
            if (afterRow != 0)
              {
                /* Move the row */
                RowTable::iterator afit = rows.move(r->it, afterRow->it);
                if (_firstRowView == r->it->value)
                  {
                    _firstRowView = afit->value;
                  }
                /* Raise event onRowMoved if event mode */
                if (_eventMode)
                  onRowMoved(*r, afterRow);
                /* Increment update counter */
                _updateCounter++;
              }
            else
              {
                /* Move the row */
                RowTable::iterator afit = rows.move(r->it, 0);
                if (_firstRowView == r->it->value)
                  {
                    _firstRowView = afit->value;
                  }
                /* Raise event onRowMoved if event mode */
                if (_eventMode)
                  onRowMoved(*r, 0);
                /* Increment update counter */
                _updateCounter++;
              }
          }
      }
  }

  /** -getRowViewOrderedAfter */
  GridView::RowView*
  GridView::getRowViewOrderedAfter(GridView::RowView* r)
  {
    /* If sort activate find the good position */
    if (_sortEnabled)
      {
        /* Transform sort order to int */
        int orderCmp = ((_sortOrder == ASCENDING) ? -1 : 1);
        /* Check if in rows list */
        if (r->it == 0)
          return r;
        /* Get the values to compare */
        const Value& v1 = r->it->value->ref->getCell(*_sortRefColumn).get();
        /* Get the Iterator of previous or next if (!prev) */
        RowTable::iterator itCmp = r->it->prev;
        if (itCmp == 0)
          itCmp = r->it->next;
        const Value* v2 = &(itCmp->value->ref->getCell(*_sortRefColumn).get());
        /* Move until found the correct position */
        int cmpVal = v1.compare(*v2);
        if (cmpVal == 0)
          {
            /* Return the current position */
            return r;
          }
        else if (cmpVal == orderCmp)
          {
            /* Need to go from down to up */
            for (; itCmp != 0; itCmp = itCmp->prev)
              {
                v2 = &(itCmp->value->ref->getCell(*_sortRefColumn).get());
                if ((itCmp->prev == 0) || (v1.compare(*v2) == 0))
                  {
                    return itCmp->value;
                  }
              }
          }
        else
          {
            /* Need to go from up to down */
            for (; itCmp != 0; itCmp = itCmp->next)
              {
                v2 = &(itCmp->value->ref->getCell(*_sortRefColumn).get());
                if ((itCmp->next == 0) || (v1.compare(*v2) == 0))
                  {
                    return itCmp->value;
                  }
              }
          }
        /* If arrive here return current position */
        return r;
      }
    else
      {
        return r;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Sort section */

  /** +sort */
  void
  GridView::sort(unsigned int byColumnViewId, GridView::SortOrder sortOrder)
  {
    GridColumn& c = getColumn(byColumnViewId);
    if (&c != &(GridModel::ncolumn))
      {
        if ((!_sortEnabled) || (_sortOrder != sortOrder)
            || (_sortRefColumn != &c))
          {
            _sortRefColumn = &c;
            _sortOrder = sortOrder;
            _sortEnabled = true;
            _sortNeeded = true;
            /* Increment counter */
            _updateCounter++;
          }
      }
  }
  /** +sort */
  void
  GridView::sort(const GridColumn& byColumn, GridView::SortOrder sortOrder)
  {
    if ((!_sortEnabled) || (_sortOrder != sortOrder)
        || (_sortRefColumn != &byColumn))
      {
        _sortOrder = sortOrder;
        _sortRefColumn = &byColumn;
        _sortEnabled = true;
        _sortNeeded = true;
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** +disableSort */
  void
  GridView::disableSort()
  {
    if (_sortEnabled)
      {
        _sortEnabled = false;
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** +sortRows */
  void
  GridView::sortRows()
  {
    if (_sortEnabled)
      {
        /* Init */
        RowTable::iterator r = rows.begin(), mm;
        int cmpOrder = (_sortOrder == ASCENDING ? -1 : 1);
        /* Sort */
        while (r != 0)
          {
            /* Search the min/max row */
            mm = privSearchMinMaxRow(r, _sortRefColumn, cmpOrder);
            /* Swap */
            r->value->it = mm;
            mm->value->it = r;
            rows.swap(r, mm);
            /* Advance */
            r = r->next;
          }
        /* Increment counter */
        _updateCounter++;
        /* Set at the start the row view */
        r = rows.begin();
        if (r != 0)
          _firstRowView = r->value;
      }
  }
  /** +isSortEnabled */
  bool
  GridView::isSortEnabled() const
  {
    return _sortEnabled;
  }
  /** +getSortRefColumnViewId */
  unsigned int
  GridView::getSortRefColumnViewId() const
  {
    if ((!_sortEnabled) || (_sortRefColumn == 0))
      {
        return GridView::npos;
      }
    else
      {
        return getColumnViewId(*_sortRefColumn);
      }
  }
  /** +getSortRefColumn */
  const GridColumn&
  GridView::getSortRefColumn() const
  {
    if ((!_sortEnabled) || (_sortRefColumn == 0))
      {
        return GridModel::ncolumn;
      }
    else
      {
        return *_sortRefColumn;
      }
  }
  /** +getSortOrder */
  GridView::SortOrder
  GridView::getSortOrder() const
  {
    return _sortOrder;
  }

  /** -privSearchMinMaxRow */
  GridView::RowTable::iterator
  GridView::privSearchMinMaxRow(GridView::RowTable::iterator start,
      const GridColumn* col, int cmpOrder)
  {
    /* Init */
    RowTable::iterator cur, mm;
    cur = start;
    mm = cur;
    /* Find */
    while (cur != 0)
      {
        if (cur->value->ref->getCell(*col).get().compare(
            mm->value->ref->getCell(*col).get()) == cmpOrder)
          {
            mm = cur;
          }
        cur = cur->next;
      }
    /* Return the min/max row */
    return mm;
  }

  /* -------------------------------------------------------------------------- */
  /* Move/Show/Hide Column section */

  /** #privSetColumnVisibility */
  GridView&
  GridView::privSetColumnVisibility(GridView::ColumnView* c, bool visible)
  {
    /* Change visibility if needed */
    if ((c != 0) && (c->visible != visible))
      {
        c->visible = visible;
        /* SmartResize Column */
        smartResizeColumns();
        /* Raise event if event mode */
        if (_eventMode)
          onColumnChange(*c);
        /* Increment Counter */
        _updateCounter++;
      }
    /* Return the ref to the current object */
    return *this;
  }

  /** #privMoveColumn */
  GridView&
  GridView::privMoveColumn(GridView::ColumnView* c, int afterViewId)
  {
    /* Move */
    if (c != 0)
      {
        bool moveDone = false;
        ColumnView* afterView = 0;
        if (afterViewId < 0)
          {
            if (c->it != columns.begin())
              {
                ColumnTable::iterator afit = columns.move(c->it, 0);
                if (_firstColumnView == c->it->value)
                  {
                    _firstColumnView = afit->value;
                  }
                moveDone = true;
              }
          }
        else
          {
            /* Found the afterViewId */
            ColumnTable::iterator afterIt = columns.getIt(
                (unsigned int) (afterViewId));
            if (afterIt != c->it)
              {
                ColumnTable::iterator afit = columns.move(c->it, afterIt);
                if (_firstColumnView == c->it->value)
                  {
                    _firstColumnView = afit->value;
                  }
                moveDone = true;
                afterView = afterIt->value;
              }
          }
        /* Update if changed */
        if (moveDone)
          {
            /* SmartResize Column */
            smartResizeColumns();
            /* Raise event if event mode */
            if (_eventMode)
              onColumnMoved(*c, afterView);
            /* Increment Counter */
            _updateCounter++;
          }
      }
    /* Return the ref to the current object */
    return *this;
  }

  /** +setColumnVisibility */
  GridView&
  GridView::setColumnVisibility(const GridColumn& col, bool visible)
  {
    /* Find the ColumnView associated */
    ColumnMap::iterator c = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* If found set visibility */
    if (c != columnsMap.end())
      {
        return privSetColumnVisibility(c->second, visible);
      }
    else
      {
        /* Return the ref to the current object */
        return *this;
      }
  }
  /** +setColumnVisibility */
  GridView&
  GridView::setColumnVisibility(unsigned int colViewId, bool visible)
  {
    /* Find the Column View associated */
    ColumnView* c = columns[colViewId];
    /* Set Column visibility */
    if (columns.isValid(c))
      {
        return privSetColumnVisibility(c, visible);
      }
    else
      {
        return *this;
      }
  }

  /** +showColumn */
  GridView&
  GridView::showColumn(const GridColumn& col)
  {
    return setColumnVisibility(col, true);
  }
  /** +showColumn */
  GridView&
  GridView::showColumn(unsigned int colViewId)
  {
    return setColumnVisibility(colViewId, true);
  }
  /** +hideColumn */
  GridView&
  GridView::hideColumn(const GridColumn& col)
  {
    return setColumnVisibility(col, false);
  }
  /** +hideColumn */
  GridView&
  GridView::hideColumn(unsigned int colViewId)
  {
    return setColumnVisibility(colViewId, false);
  }
  /** +hideAllColumn */
  GridView&
  GridView::hideAllColumn()
  {
    for (ColumnTable::iterator it = columns.begin(); it != 0; it = it->next)
      {
        privSetColumnVisibility(it->value, false);
      }
    return *this;
  }
  /** +showAllColumn */
  GridView&
  GridView::showAllColumn()
  {
    for (ColumnTable::iterator it = columns.begin(); it != 0; it = it->next)
      {
        privSetColumnVisibility(it->value, true);
      }
    return *this;
  }

  /** +isColumnVisible */
  bool
  GridView::isColumnVisible(const GridColumn& col) const
  {
    /* Find the ColumnView associated */
    ColumnMap::const_iterator c = columnsMap.find(
        &(const_cast<GridColumn&>(col)));
    /* If found set visibility */
    if (c != columnsMap.end())
      {
        return c->second->visible;
      }
    else
      {
        return false;
      }
  }
  /** +isColumnVisible */
  bool
  GridView::isColumnVisible(unsigned int colViewId) const
  {
    /* Find the ColViewId associated */
    ColumnView* c = columns.getValue_const(colViewId);
    /* Check if visible */
    if (columns.isValid(c))
      {
        return c->visible;
      }
    else
      {
        return false;
      }
  }
  /** +getColumnViewId */
  unsigned int
  GridView::getColumnViewId(const GridColumn& col) const
  {
    /* Find the ColumnView associated */
    ColumnMap::const_iterator c = columnsMap.find(
        &(const_cast<GridColumn&>(col)));
    /* If found set visibility */
    if (c != columnsMap.end())
      {
        return columns.getPos(c->second->it);
      }
    else
      {
        return npos;
      }
  }
  /** +getFirstColumnView */
  GridView::ColumnView*
  GridView::getFirstColumnView()
  {
    ColumnTable::iterator it = columns.begin();
    if (it != 0)
      {
        return it->value;
      }
    else
      {
        return 0;
      }
  }

  /** +fixColumn */
  GridView&
  GridView::fixColumn(const GridColumn& col, bool fixAtTheEnd, int offset)
  {
    /* Find the ColumnView associated */
    ColumnMap::iterator c = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* If found unfix col */
    if ((c != columnsMap.end())
        && ((!(c->second->fixed)) || (c->second->fixedAtEnd != fixAtTheEnd)
            || (c->second->fixedOffset != offset)))
      {
        c->second->fixed = true;
        c->second->fixedAtEnd = fixAtTheEnd;
        c->second->fixedOffset = offset;
        fixedColumns.insert(c->second);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +fixColumn */
  GridView&
  GridView::fixColumn(unsigned int colViewId, bool fixAtTheEnd, int offset)
  {
    /* Get the ColumnView */
    ColumnView* c = columns[colViewId];
    /* If found unfix col */
    if ((columns.isValid(c))
        && ((!(c->fixed)) || (c->fixedAtEnd != fixAtTheEnd)
            || (c->fixedOffset != offset)))
      {
        c->fixed = true;
        c->fixedAtEnd = fixAtTheEnd;
        c->fixedOffset = offset;
        fixedColumns.insert(c);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }

  /** +moveColumn */
  GridView&
  GridView::moveColumn(const GridColumn& col, int afterViewId)
  {
    /* Find the ColumnView associated */
    ColumnMap::iterator c = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* If found set visibility */
    if (c != columnsMap.end())
      {
        return privMoveColumn(c->second, afterViewId);
      }
    else
      {
        /* Return the ref to the current object */
        return *this;
      }
  }
  /** +moveColumn */
  GridView&
  GridView::moveColumn(unsigned int colViewId, int afterViewId)
  {
    /* Find the Column View associated */
    ColumnView* c = columns[colViewId];
    /* Move Column */
    if (columns.isValid(c))
      {
        return privMoveColumn(c, afterViewId);
      }
    else
      {
        return *this;
      }
  }

  /** +unfixColumn */
  GridView&
  GridView::unfixColumn(const GridColumn& col)
  {
    /* Find the ColumnView associated */
    ColumnMap::iterator c = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* If found unfix col */
    if ((c != columnsMap.end()) && (c->second->fixed))
      {
        c->second->fixed = false;
        fixedColumns.erase(c->second);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +unfixColumn */
  GridView&
  GridView::unfixColumn(unsigned int colViewId)
  {
    /* Get the ColumnView */
    ColumnView* c = columns[colViewId];
    /* If found unfix col */
    if ((columns.isValid(c)) && (c->fixed))
      {
        c->fixed = false;
        fixedColumns.erase(c);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }

  /* -------------------------------------------------------------------------- */
  /* Move/Show/Hide Row section */

  /** #privSetRowVisibility */
  GridView&
  GridView::privSetRowVisibility(GridView::RowView* r, bool visible)
  {
    /* Change visibility if needed */
    if ((r != 0) && (r->visible != visible))
      {
        r->visible = visible;
        /* Raise event if event mode */
        if (_eventMode)
          onRowChange(*r);
        /* Increment Counter */
        _updateCounter++;
        /* Update column sum */
        if (_sumColumnEnableTotal > 0)
          {
            if (visible)
              {
                incSumColumn(r);
              }
            else
              {
                decSumColumn(r);
              }
          }
      }
    /* Return the ref to the current object */
    return *this;
  }

  /** #privMoveRow */
  GridView&
  GridView::privMoveRow(RowView* r, int afterViewId)
  {
    /* Move the row */
    if (r != 0)
      {
        bool moveDone = false;
        RowView* afterView = 0;
        if (afterViewId < 0)
          {
            if (r->it != rows.begin())
              {
                RowTable::iterator afit = rows.move(r->it, 0);
                if (_firstRowView == r->it->value)
                  {
                    _firstRowView = afit->value;
                  }
                moveDone = true;
              }
          }
        else
          {
            /* Found the afterViewId */
            RowTable::iterator afterIt = rows.getIt(
                (unsigned int) (afterViewId));
            if (afterIt != r->it)
              {
                RowTable::iterator afit = rows.move(r->it, afterIt);
                if (_firstRowView == r->it->value)
                  {
                    _firstRowView = afit->value;
                  }
                moveDone = true;
                afterView = afterIt->value;
              }
          }
        /* Update if changed */
        if (moveDone)
          {
            /* Raise event if event mode */
            if (_eventMode)
              onRowMoved(*r, afterView);
            /* Increment Counter */
            _updateCounter++;
          }
      }
    /* Return the ref to the current object */
    return *this;
  }

  /** +setRowVisibility */
  GridView&
  GridView::setRowVisibility(const GridRow& row, bool visible)
  {
    /* Find the RowView associated */
    RowMap::iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found set visibility */
    if (r != rowsMap.end())
      {
        return privSetRowVisibility(r->second, visible);
      }
    /* Return the ref to the current object */
    return *this;
  }
  /** +setRowVisibility */
  GridView&
  GridView::setRowVisibility(unsigned int rowViewId, bool visible)
  {
    /* Dont allow row move if sort */
    if (_sortEnabled)
      return *this;
    /* Find the Row View associated */
    RowView* r = rows[rowViewId];
    /* Set Row visibility */
    if (rows.isValid(r))
      {
        return privSetRowVisibility(r, visible);
      }
    else
      {
        return *this;
      }
  }

  /** +showRow */
  GridView&
  GridView::showRow(const GridRow& row)
  {
    return setRowVisibility(row, true);
  }
  /** +showRow */
  GridView&
  GridView::showRow(unsigned int rowViewId)
  {
    return setRowVisibility(rowViewId, true);
  }
  /** +hideRow */
  GridView&
  GridView::hideRow(const GridRow& row)
  {
    return setRowVisibility(row, false);
  }
  /** +hideRow */
  GridView&
  GridView::hideRow(unsigned int rowViewId)
  {
    return setRowVisibility(rowViewId, false);
  }
  /** +isRowVisible */
  bool
  GridView::isRowVisible(const GridRow& row) const
  {
    /* Find the RowView associated */
    RowMap::const_iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found set visibility */
    if (r != rowsMap.end())
      {
        return r->second->visible;
      }
    else
      {
        return false;
      }
  }
  /** +isRowVisible */
  bool
  GridView::isRowVisible(unsigned int rowViewId) const
  {
    /* Find the ColViewId associated */
    RowView* r = rows.getValue_const(rowViewId);
    /* Check if visible */
    if (rows.isValid(r))
      {
        return r->visible;
      }
    else
      {
        return false;
      }
  }
  /** +getRowViewId */
  unsigned int
  GridView::getRowViewId(const GridRow& row) const
  {
    /* Find the RowView associated */
    RowMap::const_iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found set visibility */
    if (r != rowsMap.end())
      {
        return rows.getPos(r->second->it);
      }
    else
      {
        return npos;
      }
  }
  /** +getFirstRowView */
  GridView::RowView*
  GridView::getFirstRowView()
  {
    RowTable::iterator it = rows.begin();
    if (it != 0)
      {
        return it->value;
      }
    else
      {
        return 0;
      }
  }

  /** +fixRow */
  GridView&
  GridView::fixRow(const GridRow& row, bool fixAtTheEnd, int offset)
  {
    /* Find the RowView associated */
    RowMap::iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found unfix row */
    if ((r != rowsMap.end())
        && ((!(r->second->fixed)) || (r->second->fixedAtEnd != fixAtTheEnd)
            || (r->second->fixedOffset != offset)))
      {
        r->second->fixed = true;
        r->second->fixedAtEnd = fixAtTheEnd;
        r->second->fixedOffset = offset;
        fixedRows.insert(r->second);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +fixRow */
  GridView&
  GridView::fixRow(unsigned int rowViewId, bool fixAtTheEnd, int offset)
  {
    /* Get the RowView */
    RowView* r = rows[rowViewId];
    /* If found unfix row */
    if ((rows.isValid(r))
        && ((!(r->fixed)) || (r->fixedAtEnd != fixAtTheEnd)
            || (r->fixedOffset != offset)))
      {
        r->fixed = true;
        r->fixedAtEnd = fixAtTheEnd;
        r->fixedOffset = offset;
        fixedRows.insert(r);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +unfixRow */
  GridView&
  GridView::unfixRow(const GridRow& row)
  {
    /* Find the RowView associated */
    RowMap::iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found unfix row */
    if ((r != rowsMap.end()) && (r->second->fixed))
      {
        r->second->fixed = false;
        fixedRows.erase(r->second);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }
  /** +unfixRow */
  GridView&
  GridView::unfixRow(unsigned int rowViewId)
  {
    /* Get the RowView */
    RowView* r = rows[rowViewId];
    /* If found unfix row */
    if ((rows.isValid(r)) && (r->fixed))
      {
        r->fixed = false;
        fixedRows.erase(r);
        /* Increment update counter */
        _updateCounter++;
      }
    /* Return the reference of this object */
    return *this;
  }

  /** +moveRow */
  GridView&
  GridView::moveRow(const GridRow& row, int afterViewId)
  {
    /* Dont allow row move if sort */
    if (_sortEnabled)
      return *this;
    /* Find the RowView associated */
    RowMap::iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found set visibility */
    if (r != rowsMap.end())
      {
        return privMoveRow(r->second, afterViewId);
      }
    else
      {
        /* Return the ref to the current object */
        return *this;
      }
  }
  /** +moveRow */
  GridView&
  GridView::moveRow(unsigned int rowViewId, int afterViewId)
  {
    /* Dont allow row move if sort */
    if (_sortEnabled)
      return *this;
    /* Find the Row View associated */
    RowView* r = rows[rowViewId];
    /* Move Row */
    if (rows.isValid(r))
      {
        return privMoveRow(r, afterViewId);
      }
    else
      {
        return *this;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Sum-bar section */

  /** -privSetColumnSum */
  GridView&
  GridView::privSetColumnSum(GridView::ColumnView* col, bool enable)
  {
    if ((col != 0) && (col->sumEnable != enable))
      {
        if (enable)
          {
            /* Set sum property */
            col->sumEnable = true;
            /* Increment the sum column count */
            _sumColumnEnableTotal++;
            /* Compute the current sum of the column */
            calculSumColumn(col);
          }
        else
          {
            /* Set sum property */
            col->sumEnable = false;
            /* Decrement the sum column count */
            _sumColumnEnableTotal--;
          }
        /* Increment counter */
        _updateCounter++;
      }
    /* Return the reference to the current object */
    return *this;
  }

  /** +setColumnSum */
  GridView&
  GridView::setColumnSum(const GridColumn& col, bool enable)
  {
    /* Find the Column View associated */
    ColumnMap::iterator it = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* Set Column sum */
    if (it != columnsMap.end())
      {
        return privSetColumnSum(it->second, enable);
      }
    else
      {
        return *this;
      }
  }
  /** +setColumnSum */
  GridView&
  GridView::setColumnSum(unsigned int colViewId, bool enable)
  {
    /* Find the Column View associated */
    ColumnView* c = columns[colViewId];
    /* Set Column sum */
    if (columns.isValid(c))
      {
        return privSetColumnSum(c, enable);
      }
    else
      {
        return *this;
      }
  }
  /** +isColumnSumEnabled */
  bool
  GridView::isColumnSumEnabled(const GridColumn& col) const
  {
    /* Find the Column View associated */
    ColumnMap::const_iterator it = columnsMap.find(
        &(const_cast<GridColumn&>(col)));
    /* Set Column sum */
    if (it != columnsMap.end())
      {
        return ((it->second != 0) && (it->second->sumEnable));
      }
    else
      {
        return false;
      }
  }
  /** +isColumnSumEnabled */
  bool
  GridView::isColumnSumEnabled(unsigned int colViewId) const
  {
    /* Find the Column View associated */
    const ColumnView* c = columns.getValue_const(colViewId);
    /* Set Column sum */
    if ((c != 0) && (c->sumEnable))
      {
        return true;
      }
    else
      {
        return false;
      }
  }

  /** -calculSumColumn */
  void
  GridView::calculSumColumn(ColumnView* col)
  {
    if ((col != 0) && (col->sumEnable))
      {
        /* Reset sum */
        col->sum.reset();
        col->sum.setType(col->ref->getType());
        /* Get all the Cells */
        GridColumn::CellMap& cells = col->ref->cells;
        for (GridColumn::CellMap::iterator c = cells.begin(); c != cells.end();
            ++c)
          {
            if (c->second->getRow().isSumConsidered())
              {
                col->sum += c->second->get();
              }
          }
        col->needSumRecalc = false;
      }
  }
  /** -clearAllSumColumn */
  void
  GridView::clearAllSumColumn()
  {
    /* Reset all the sum value */
    if (_sumColumnEnableTotal > 0)
      {
        for (ColumnTable::iterator c = columns.begin(); c != 0; c = c->next)
          {
            if (c->value->sumEnable)
              {
                c->value->sum.reset();
              }
          }
      }
  }
  /** -incSumColumn */
  void
  GridView::incSumColumn(GridView::ColumnView* col, const Value& v)
  {
    if ((col != 0) && (col->sumEnable))
      {
        col->needSumRecalc = true; //col->sum += v;
      }
  }
  /** -incSumColumn */
  void
  GridView::incSumColumn(GridView::RowView* r)
  {
    if ((r->ref != 0) && (r->ref->isSumConsidered()))
      {
        /* Get all the Rows Cells */
        GridRow::CellMap& cells = r->ref->cells;
        /* For each cell in row and all column calcul sum */
        for (GridRow::CellMap::iterator c = cells.begin(); c != cells.end();
            ++c)
          {
            //const Value& v = c->second->get();
            for (ColumnTable::iterator col = columns.begin(); col != 0; col =
                col->next)
              {
                col->value->needSumRecalc = true; //col->value->sum += v;
              }
          }
      }
  }
  /** -decSumColumn */
  void
  GridView::decSumColumn(GridView::ColumnView* col, const Value& v)
  {
    if ((col != 0) && (col->sumEnable))
      {
        col->needSumRecalc = true; //col->sum -= v;
      }
  }
  /** -decSumColumn */
  void
  GridView::decSumColumn(GridView::RowView* r)
  {
    if ((r->ref != 0) && (r->ref->isSumConsidered()))
      {
        /* Get all the Rows Cells */
        GridRow::CellMap& cells = r->ref->cells;
        /* For each cell in row and all column calcul sum */
        for (GridRow::CellMap::iterator c = cells.begin(); c != cells.end();
            ++c)
          {
            //const Value& v = c->second->get();
            for (ColumnTable::iterator col = columns.begin(); col != 0; col =
                col->next)
              {
                col->value->needSumRecalc = true; //col->value->sum -= v;
              }
          }
      }
  }

  /* -------------------------------------------------------------------------- */
  /* List helpers section */

  /** +getRowViewCount */
  unsigned int
  GridView::getRowViewCount() const
  {
    return rows.size();
  }
  /** +getRow */
  GridRow&
  GridView::getRow(unsigned int rowViewId)
  {
    /* Search the column by id */
    RowView* r = rows[rowViewId];
    if ((rows.isValid(r)) && (r->ref != 0))
      {
        return (*r->ref);
      }
    else
      {
        return GridModel::nrow;
      }
  }
  /** +getColumnViewCount */
  unsigned int
  GridView::getColumnViewCount() const
  {
    return columns.size();
  }
  /** +getColumn */
  GridColumn&
  GridView::getColumn(unsigned int colViewId)
  {
    /* Search the column by id */
    ColumnView* c = columns[colViewId];
    if ((columns.isValid(c)) && (c->ref != 0))
      {
        return (*c->ref);
      }
    else
      {
        return GridModel::ncolumn;
      }
  }
  /** +getColumnView */
  GridView::ColumnView*
  GridView::getColumnView(int offsetViewId, bool considerFixedColumn)
  {
    if (considerFixedColumn)
      {
        for (RenderList::iterator c = _renderColumns.begin();
            c != _renderColumns.end(); ++c)
          {
            if (offsetViewId == 0)
              {
                return c->col;
              }
            offsetViewId--;
          }
      }
    else
      {
        ColumnTable::iterator it = columns.advance(_firstColumnView->it,
            offsetViewId);
        if (it != 0)
          {
            return it->value;
          }
        else
          {
            return 0;
          }
      }
    return 0;
  }

  /* -------------------------------------------------------------------------- */
  /* Style section */

  /** +setStyle */
  GridView&
  GridView::setStyle(unsigned int id, const GridStyle& style)
  {
    /* Find the style */
    StyleMap::iterator it = stylesMap.find(id);
    /* Set the style */
    if (it != stylesMap.end())
      {
        *(it->second) = style;
      }
    else
      {
        /* Create the style */
        GridStyle* s = new GridStyle(style);
        if (s != 0)
          {
            /* Map the style */
            stylesMap.insert(StyleMap::value_type(id, s));
            /* Find all row to set the style */
            for (RowTable::iterator r = rows.begin(); r != rows.end();
                r = r->next)
              {
                if ((r->value->ref != 0))
                  {
                    privSetRowStyle(r->value, r->value->ref->tag);
                  }
              }
            /* Find all column to set the style */
            for (ColumnTable::iterator c = columns.begin(); c != columns.end();
                c = c->next)
              {
                if ((c->value->ref != 0))
                  {
                    privSetColumnStyle(c->value, c->value->ref->tag);
                  }
              }
          }
      }
    /* Return the reference of the current object */
    return *this;
  }
  /** +getStyle */
  const GridStyle&
  GridView::getStyle(unsigned int id)
  {
    return style(id);
  }
  /** +style */
  GridStyle&
  GridView::style(unsigned int id)
  {
    /* Find the style */
    StyleMap::iterator it = stylesMap.find(id);
    /* Return the style */
    if (it != stylesMap.end())
      {
        return *(it->second);
      }
    else
      {
        return defaultRowStyle;
      }
  }
  /** +isValid */
  bool
  GridView::isValid(const GridStyle& style) const
  {
    return (&style != &nstyle);
  }

  /** -privSetColumnStyle */
  GridView&
  GridView::privSetColumnStyle(GridView::ColumnView* c, int styleId,
      int headerStyleId, int sumStyleId)
  {
    if (c != 0)
      {
        /* Find the Cell Style */
        StyleMap::iterator cellStyle = stylesMap.find(styleId);
        /* Set the style if exist */
        if (cellStyle != stylesMap.end())
          {
            if (c->style != cellStyle->second)
              {
                c->style = cellStyle->second;
                /* Increment counter */
                _updateCounter++;
              }
          }
        /* Find the Header Style */
        StyleMap::iterator headerStyle = stylesMap.find(headerStyleId);
        /* Set the style if exist */
        if (headerStyle != stylesMap.end())
          {
            if (c->headerStyle != headerStyle->second)
              {
                c->headerStyle = headerStyle->second;
                /* Increment counter */
                _updateCounter++;
              }
          }
        /* Find the Sum Style */
        StyleMap::iterator sumStyle = stylesMap.find(sumStyleId);
        /* Set the style if exist */
        if (sumStyle != stylesMap.end())
          {
            if (c->sumStyle != sumStyle->second)
              {
                c->sumStyle = sumStyle->second;
                /* Increment counter */
                _updateCounter++;
              }
          }
      }
    /* Return the reference to the current object */
    return *this;
  }
  /** -privSetRowStyle */
  GridView&
  GridView::privSetRowStyle(GridView::RowView* r, int styleId)
  {
    if (r != 0)
      {
        if (styleId == -1)
          {
            // Set default style
            r->style = &(GridView::defaultRowStyle);
          }
        else
          {
            /* Find the Cell Style */
            StyleMap::iterator s = stylesMap.find(styleId);
            /* Set the style if exist */
            if (s != stylesMap.end())
              {
                if (r->style != s->second)
                  {
                    r->style = s->second;
                    /* Increment counter */
                    _updateCounter++;
                  }
              }
          }
      }
    /* Return the reference to the current object */
    return *this;
  }

  /** +setColumnStyle */
  GridView&
  GridView::setColumnStyle(const GridColumn& col, int styleId,
      int headerStyleId, int sumStyleId)
  {
    /* Find the ColumnView associated */
    ColumnMap::iterator c = columnsMap.find(&(const_cast<GridColumn&>(col)));
    /* If found set style */
    if (c != columnsMap.end())
      {
        return privSetColumnStyle(c->second, styleId, headerStyleId, sumStyleId);
      }
    else
      {
        /* Return the reference to the current object */
        return *this;
      }
  }
  /** +setColumnStyle */
  GridView&
  GridView::setColumnStyle(unsigned int colViewId, int styleId,
      int headerStyleId, int sumStyleId)
  {
    /* Find the ColumnView associated */
    ColumnView* c = columns[colViewId];
    /* If found set style */
    if (columns.isValid(c))
      {
        return privSetColumnStyle(c, styleId, headerStyleId, sumStyleId);
      }
    else
      {
        /* Return the reference to the current object */
        return *this;
      }
  }
  /** +setRowStyle */
  GridView&
  GridView::setRowStyle(const GridRow& row, int styleId)
  {
    /* Find the RowView associated */
    RowMap::iterator r = rowsMap.find(&(const_cast<GridRow&>(row)));
    /* If found set style */
    if (r != rowsMap.end())
      {
        return privSetRowStyle(r->second, styleId);
      }
    else
      {
        /* Return the reference to the current object */
        return *this;
      }
  }
  /** +setRowStyle */
  GridView&
  GridView::setRowStyle(unsigned int rowViewId, int styleId)
  {
    /* Find the RowView associated */
    RowView* r = rows[rowViewId];
    /* If found set style */
    if (rows.isValid(r))
      {
        return privSetRowStyle(r, styleId);
      }
    else
      {
        /* Return the reference to the current object */
        return *this;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Filter function */

  /** #applyFilter */
  bool
  GridView::applyFilter(GridView::RowView* r)
  {
    bool filterFound = false;
    if (r == 0)
      return false;
    /* Apply filter of all style filter */
    for (StyleFilterMap::iterator it = filtersStyle.begin();
        it != filtersStyle.end(); ++it)
      {
        if (it->first->applyFilter(*r))
          {
            privSetRowStyle(r, it->second);
            r->styleFromFilter = it->first;
            filterFound = true;
          }
      }
    /* If Style not found reset default style */
    if ((!filterFound) && (r->styleFromFilter != 0))
      {
        r->style = &defaultRowStyle;
        r->styleFromFilter = 0;
      }
    /* Return true if main filter is ok */
    return _filter.applyFilter(*r);
  }

  /** +refreshFilters */
  void
  GridView::refreshFilters()
  {
    /* Get Grid Model */
    GridModel* grdModel = getModel();
    if (grdModel == 0)
      return;

    /* Refresh Main Filter */
    _filter.refresh(*grdModel);
    /* Refresh Log Filter */
    refreshLogFilter();
    /* Refresh Style-Filter */
    for (StyleFilterMap::iterator it = filtersStyle.begin();
        it != filtersStyle.end(); ++it)
      {
        it->first->refresh(*grdModel);
      }
  }
  /** #refreshFilter */
  void
  GridView::refreshAFilter(GridFilter& filter)
  {
    /* Get Grid Model */
    GridModel* grdModel = getModel();
    if (grdModel != 0)
      {
        filter.refresh(*grdModel);
      }
  }

  /** +setFilter */
  void
  GridView::setFilter(const std::string& str)
  {
    _filter.set(str);
    refreshAFilter(_filter);
  }

  /** +getFilter */
  std::string
  GridView::getFilter() const
  {
    return _filter.get();
  }

  /** +addStyleFilter */
  GridView&
  GridView::addStyleFilter(const std::string& filter, int styleId)
  {
    /* Create the filter */
    GridFilter* f = new GridFilter;
    if (f != 0)
      {
        refreshAFilter(*f);
        f->set(filter);
        filtersStyle.insert(StyleFilterMap::value_type(f, styleId));
      }
    /* Return the reference to the current object */
    return *this;
  }
  /** +removeStyleFilter */
  GridView&
  GridView::removeStyleFilter(unsigned int id)
  {
    /* Delete all the filter style */
    for (StyleFilterMap::iterator it = filtersStyle.begin();
        it != filtersStyle.end(); ++it)
      {
        if (id == 0)
          {
            /* Delete from memory filter */
            delete it->first;
            /* Remove from map */
            filtersStyle.erase(it);
            /* Exit loop */
            break;
          }
        else
          {
            id--;
          }
      }
    /* Return the reference to the current object */
    return *this;
  }
  /** +getStyleFilterCount */
  unsigned int
  GridView::getStyleFilterCount() const
  {
    return filtersStyle.size();
  }
  /** +clearStyleFilter */
  GridView&
  GridView::clearStyleFilter()
  {
    /* Delete all the filter style */
    for (StyleFilterMap::iterator it = filtersStyle.begin();
        it != filtersStyle.end(); ++it)
      {
        /* Delete from memory filter */
        delete it->first;
      }
    /* Clear the filter style */
    filtersStyle.clear();
    /* Return the reference to the current object */
    return *this;
  }

  /* ------------------------------------------------------------------------- */
  /* Model function */

  GridModel*
  GridView::getModel()
  {
    /* Get GridModel from a Column */
    if (columns.size() > 0)
      {
        /* Get a ref to the grid Model */
        GridColumn* c = columns[0]->ref;
        if (c != 0)
          {
            return &(c->getGrid());
          }
      }
    /* If Not connected to a model return 0 */
    return 0;
  }

  /* -------------------------------------------------------------------------- */
  /* View function */

  /** +scrollRowTo */
  void
  GridView::scrollRowTo(const GridRow& row)
  {
    /* Find the row view associated */
    RowMap::iterator it = rowsMap.find(&(const_cast<GridRow&>(row)));
    if (it != rowsMap.end())
      {
        _firstRowView = it->second;
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** +scrollRowTo */
  void
  GridView::scrollRowTo(unsigned int rowViewId)
  {
    /* Search the row */
    RowView* r = rows[rowViewId];
    if (rows.isValid(r))
      {
        _firstRowView = r;
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** +scrollColumnTo */
  void
  GridView::scrollColumnTo(const GridColumn& col)
  {
    /* Find the column view associated */
    ColumnMap::iterator it = columnsMap.find(&(const_cast<GridColumn&>(col)));
    if (it != columnsMap.end())
      {
        _firstColumnView = it->second;
        /* Increment counter */
        _updateCounter++;
      }
  }
  /** +scrollColumnTo */
  void
  GridView::scrollColumnTo(unsigned int colViewId)
  {
    /* Search the row */
    ColumnView* c = columns[colViewId];
    if (columns.isValid(c))
      {
        _firstColumnView = c;
        /* Increment counter */
        _updateCounter++;
      }
  }

  /** +scrollView */
  void
  GridView::scrollView(int rowViewOffset, int columnViewOffset)
  {
    /* Advance the _firstRowView */
    if ((_firstRowView != 0) && (rowViewOffset != 0))
      {
        RowTable::iterator r = rows.advance(_firstRowView->it, rowViewOffset);
        if (r != 0)
          _firstRowView = r->value;
        /* Increment counter */
        _updateCounter++;
      }
    /* Advance the _firstColumnView */
    if ((_firstColumnView != 0) && (columnViewOffset != 0))
      {
        ColumnTable::iterator c = columns.advance(_firstColumnView->it,
            columnViewOffset);
        if (c != 0)
          _firstColumnView = c->value;
        /* Increment counter */
        _updateCounter++;
      }

  }
  /** +getFirstVisibleRow */
  GridRow&
  GridView::getFirstVisibleRow() const
  {
    if ((_firstRowView != 0) && (_firstRowView->ref != 0))
      {
        return *(_firstRowView->ref);
      }
    else
      {
        return GridModel::nrow;
      }
  }
  /** +getFirstVisibleColumn */
  GridColumn&
  GridView::getFirstVisibleColumn() const
  {
    if ((_firstColumnView != 0) && (_firstColumnView->ref != 0))
      {
        return *(_firstColumnView->ref);
      }
    else
      {
        return GridModel::ncolumn;
      }
  }

  /** +isRowRendered */
  bool
  GridView::isRowRendered(const GridRow& row)
  {
    /* Find the row view associated & the link */
    RowMap::iterator it = rowsMap.find(&(const_cast<GridRow&>(row)));
    if (it == rowsMap.end())
      return false;
    /* Get the rowview */
    RowView* cur = it->second;
    if ((_renderAllItem) || (!(cur->visible)))
      return true;
    /* Check if the row is rendered */
      {
        /* Prepare the size */
        int y = top + 1; /* +1 is for column header bar */
        /* Get View start pos */
        RowTable::iterator startRow = rows.begin();
        if ((_firstRowView != 0) && (_firstRowView->it != 0))
          {
            startRow = _firstRowView->it;
          }
        /* Calcul maximum */
        int h = top + _height;
        if (_sumColumnEnableTotal > 0)
          {
            h -= 2; /* Sum Bar + sep */
          }
        /* Find the row */
        for (RowTable::iterator r = startRow; r != 0; r = r->next)
          {
            if (r->value->visible)
              {
                /* Check if reach height */
                if (y >= h)
                  break;
                /* Check if the rowview is the one searched */
                if (r->value == cur)
                  return true;
                /* Next row */
                y += 1;
              }
          }
      }
    /* If row will not be rendered then return false */
    return false;
  }
  /** +isColumnRendered */
  bool
  GridView::isColumnRendered(const GridColumn& col)
  {
    /* Find the Column view associated & the link */
    ColumnMap::iterator it = columnsMap.find(&(const_cast<GridColumn&>(col)));
    if (it == columnsMap.end())
      return false;
    /* Get the ColumnView */
    ColumnView* cur = it->second;
    if (!(cur->visible))
      return false;
    if (_renderAllItem)
      return true;
    /* Check if the Column is rendered */
      {
        /* Prepare the size */
        int x = left;
        /* Get View start pos */
        ColumnTable::iterator startColumn = columns.begin();
        if ((_firstColumnView != 0) && (_firstColumnView->it != 0))
          {
            startColumn = _firstColumnView->it;
          }
        /* Find the Column */
        for (ColumnTable::iterator c = startColumn; c != 0; c = c->next)
          {
            if (c->value->visible)
              {
                /* Check if the rowview is the one searched */
                if (c->value == cur)
                  return true;
                /* Advance */
                x = x + c->value->w;
                /* Check if reach width */
                if (x > left + _width)
                  break;
              }
          }
      }
    /* If Column will not be rendered then return false */
    return false;
  }

  /** +move */
  GridView&
  GridView::move(int x, int y, int w, int h)
  {
    left = x;
    top = y;
    size(w, h);
    /* Return the reference to the current object */
    return *this;
  }
  /** +size */
  GridView&
  GridView::size(int w, int h)
  {
    if (h != 0)
      {
        _height = h;
      }
    /* Smart resize */
    if ((w != 0) && (_width != w))
      {
        _width = w;
        smartResizeColumns();
      }
    /* Return the reference to the current object */
    return *this;
  }
  /** +getLeft */
  int
  GridView::getLeft() const
  {
    return left;
  }
  /** +getTop */
  int
  GridView::getTop() const
  {
    return top;
  }
  /** +getHeight */
  int
  GridView::getHeight() const
  {
    return _height;
  }
  /** +getWidth */
  int
  GridView::getWidth() const
  {
    return _width;
  }

  /** +setAutoSize */
  void
  GridView::setAutoSize(const GridView::AutoSize& autosize)
  {
    _autoSize = autosize;
  }

  /** +getAutoSize */
  const GridView::AutoSize&
  GridView::getAutoSize() const
  {
    return _autoSize;
  }

  /** +refreshAutoSize */
  void
  GridView::refreshAutoSize()
  {
    /* DO NOTHING - By Default */;
  }

  /** #smartResizeColumns */
  void
  GridView::smartResizeColumns()
  {
    /* Algo: fix follow their percent and if percent don't fit
     size = size of title + 5 */
    int realWidth = 0;
    ColumnView* lastCol = 0;
    /* Calcul size for each column */
    for (ColumnTable::iterator it = columns.begin(); it != 0; it = it->next)
      {
        if (it->value->visible)
          {
            /* Get the column */
            GridColumn* c = it->value->ref;
            /* Retrieve percent size */
            unsigned int percentW = c->getPercentSize();
            unsigned int minW = c->getMinimumSize();
            unsigned int w = 0;
            if (percentW > 0)
              {
                w = (unsigned int) (((float) (percentW) / 100.f) * _width);
              }
            unsigned int len = c->getTitle().length() + 6;
            /* Set the Size */
            if (w < minW)
              w = minW;
            if (w < len)
              w = len;
            realWidth += w;
            lastCol = it->value;
            lastCol->w = w;
          }
      }
    /* Set last column the rest size */
    /*if ((lastCol != 0) && (realWidth < _width)) {
     lastCol->w += (_width - realWidth);
     }*/
  }

  /* -------------------------------------------------------------------------- */
  /* Render function */

  /** +setRenderMode */
  GridView&
  GridView::setRenderMode(bool renderAllItem)
  {
    _renderAllItem = renderAllItem;
    return *this;
  }
  /** +isRenderAllItem */
  bool
  GridView::isRenderAllItem() const
  {
    return _renderAllItem;
  }

  /** +update */
  void
  GridView::update()
  {
    /* Do Sort */
    if (_sortNeeded)
      {
        sortRows();
        _sortNeeded = false;
      }
    /* Recalcul all the column sum necessary */
    for (ColumnTable::iterator c = columns.begin(); c != 0; c = c->next)
      {
        if (c->value->needSumRecalc)
          {
            calculSumColumn(c->value);
          }
      }
    /* Update Log */
    _log.update();
  }

  /** +forceRender */
  void
  GridView::forceRender()
  {
    _updateCounter++;
  }

  /** +render */
  bool
  GridView::needRender() const
  {
    return (_lastRenderUpdateCounter != _updateCounter);
  }

  /** +render */
  void
  GridView::render()
  {
    /* If event mode no render needed */
    if (_eventMode)
      return;
    /* Update */
    update();
    refreshRenderList();
    beforeRender();
    /* Init */
    std::string txt;
    /* Position */
    int x = left;
    int y = top;
    int w = left + _width;
    int h = top + _height;
    int vW = w, vH = h;
    /* Draw the column header */
    for (RenderList::iterator c = _renderColumns.begin();
        c != _renderColumns.end(); ++c)
      {
        /* Render column Header */
        ColumnView& v = *(c->col);
        getShowText(v.ref->getTitle(), c->w, *(v.headerStyle), txt);
        renderColumnHeader(v, c->pos, txt, c->x, c->y, c->w, *(v.headerStyle));
        /* Advance */
        x += c->w;
        if ((!_renderAllItem) && (x >= w))
          break;
      }
    /* Draw the Rows */
    vW = x;
    y++;
    if (_sumColumnEnableTotal > 0)
      {
        if (_sumAdditionalLine)
          h -= 2;
        else
          h--;
      }
    unsigned int rid = 0;
    for (RenderList::iterator r = _renderRows.begin(); r != _renderRows.end();
        ++r)
      {
        /* Render Row */
        x = left;
        RowView& v = *(r->row);
        renderRow(v, r->pos, r->x, r->y - 1, vW);

        if ((r->pos == 0) && (_headerAdditionalLine))
          y++;
        GridStyle* style = 0;
        /* Render each cell */
        for (RenderList::iterator c = _renderColumns.begin();
            c != _renderColumns.end(); ++c)
          {
            /* Get cell */
            const GridCell& cell = v.ref->getCell(*(c->col->ref));
            /* Render It */
            getShowText(cell.get(), c->w, *(c->col->style), txt);
            if (r->row->style != &defaultRowStyle)
              {
                style = r->row->style;
              }
            else
              {
                style = c->col->style;
              }
            renderCell(cell, r->pos, c->pos, txt, c->x, r->y, c->w, *(style));

            /* Advance */
            x += c->w;
            if ((!_renderAllItem) && (x >= w))
              break;
          }
        /* Advance */
        if ((!_renderAllItem) && (y > h))
          break;
        rid++;
        y++;
      }
    /* Draw the SumBar */
    if (_sumColumnEnableTotal > 0)
      {
        /* Render sum bar */
        x = left;
        //if (rid == _renderRows.size()) y--;
        if (_sumAdditionalLine)
          {
            renderSumBar(left, y, vW);
            y++;
          }
        /* Render sum cell */
        for (RenderList::iterator c = _renderColumns.begin();
            c != _renderColumns.end(); ++c)
          {
            /* Render It */
            if (c->col->sumEnable)
              {
                getShowText(c->col->sum, c->w, *(c->col->sumStyle), txt);
              }
            else
              {
                getShowText("", c->w, *(c->col->sumStyle), txt);
              }
            /* Render */
            renderSumBarItem(*(c->col), c->pos, txt, c->x, y, c->w,
                *(c->col->sumStyle));
            /* Advance */
            x += c->w;
            if ((!_renderAllItem) && (x >= w))
              break;
          }
        y++;
        //if (rid == _renderRows.size()) y++;
      }
    /* After Render function */
    vH = y;
    afterRender(vW - left, vH - top);
    /* Keep the current update count */
    _lastRenderUpdateCounter = _updateCounter;
  }

  /** +renderColumnHeader */
  void
  GridView::renderColumnHeader(const ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    /* Set Sort order tips */
    if ((_sortEnabled) && (_sortRefColumn == col.ref))
      {
        std::string tmp(text);
        /* Draw order tips */
        if (tmp.length() > 2)
          {
            tmp[tmp.length() - 2] = ((_sortOrder == ASCENDING) ? '^' : 'v');
          }
        /* Draw text */
        std::cout << tmp;
      }
    else
      {
        std::cout << text;
      }
  }
  /** +renderRow */
  void
  GridView::renderRow(const RowView& row, int rowId, int x, int y,
      int viewWidth)
  {
    if ((rowId == 0) && (_headerAdditionalLine))
      {
        std::cout << std::endl;
        std::string line;
        line.resize(viewWidth, '-');
        std::cout << line;
      }
    std::cout << std::endl;
  }
  /** +renderCell */
  void
  GridView::renderCell(const GridCell& c, int rowId, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    std::cout << text;
  }

  /** +renderSumBar */
  void
  GridView::renderSumBar(int x, int y, int viewWidth)
  {
    std::cout << std::endl;
    std::string line;
    line.resize(viewWidth, '_');
    std::cout << line;
    std::cout << std::endl;
  }
  /** +renderSumBarItem */
  void
  GridView::renderSumBarItem(const GridView::ColumnView& col, int colId,
      const std::string& text, int x, int y, int colSize,
      const GridStyle& style)
  {
    std::cout << text;
  }

  /** #getShowText */
  void
  GridView::getShowText(const std::string& txt, unsigned int colSize,
      const GridStyle& style, std::string& retText)
  {
    /* Set Main text */
    retText.assign(txt);
    /* Add prefix and Suffix */
    if (!(style.prefix.empty()))
      {
        retText.insert(0, style.prefix);
      }
    if (!(style.suffix.empty()))
      {
        retText.append(style.suffix);
      }
    /* Truncat */
    if (style.border)
      {
        colSize--;
      }
    if ((colSize > 0) && (retText.length() > colSize))
      {
        retText.erase(colSize);
      }
    /* Add Space to align */
    if ((colSize > 0) && (retText.length() < colSize))
      {
        switch (style.align)
          {
        case GridStyle::LEFT:
          retText.resize(colSize, ' ');
          break;
        case GridStyle::RIGHT:
          retText.insert(retText.begin(), colSize - retText.length(), ' ');
          break;
        case GridStyle::CENTER:
          int leftSize = (colSize / 2) - (retText.length() / 2);
          if (leftSize > 0)
            {
              retText.insert(retText.begin(), leftSize, ' ');
            }
          retText.resize(colSize, ' ');
          break;
          }
      }
    /* Add border if border */
    if (style.border)
      {
        retText.resize(retText.length() + 1, '|');
      }
  }

  /** #beforeRender */
  void
  GridView::beforeRender()
  {
    /* DO NOTHING - by default */
  }

  /** #afterRender */
  void
  GridView::afterRender(int viewWidth, int viewHeight)
  {
    /* DO NOTHING - by default */
  }

  /** Prepare the list of row/column to render */
  void
  GridView::refreshRenderList()
  {
    if (_lastRenderUpdateCounter != _updateCounter)
      {
        /* Get Position */
        int x = left;
        int y = top;
        int h = top + _height - 1;
        unsigned int cid = 0, rid = 0;
        std::string txt;
        if ((!_renderAllItem) && (y >= top + _height))
          return;
        /* Clear List */
        _renderColumns.clear();
        _renderRows.clear();
        /* Get view start pos */
        ColumnTable::iterator startCol = columns.begin();
        if ((_firstColumnView != 0) && (_firstColumnView->it != 0))
          {
            startCol = _firstColumnView->it;
          }
        RowTable::iterator startRow = rows.begin();
        if ((_firstRowView != 0) && (_firstRowView->it != 0))
          {
            startRow = _firstRowView->it;
          }
        /* Search how many column can be render */
        for (ColumnTable::iterator c = startCol; c != columns.end(); c =
            c->next)
          {
            if ((c->value->visible) && (!(c->value->fixed)))
              {
                /* Add Column Render */
                RenderViewItem renderView;
                renderView.col = c->value;
                renderView.row = 0;
                renderView.pos = cid;
                renderView.x = x;
                renderView.y = y;
                renderView.w = c->value->w;
                renderView.h = 1;
                _renderColumns.push_back(renderView);
                /* Advance */
                x += c->value->w;
                /* Check if reach width */
                if ((!_renderAllItem) && (x >= left + _width))
                  break;
                cid++;
              }
          }
        y++;
        x = left;
        /* Calcul maximum y */
        if (_sumColumnEnableTotal > 0)
          {
            if (_sumAdditionalLine)
              h -= 2;
            else
              h--;
          }
        /* Search how many row can be render */
        for (RowTable::iterator r = startRow; r != 0; r = r->next)
          {
            if ((r->value->visible) && (!(r->value->fixed)))
              {
                /* Add Row Render */
                if ((rid == 0) && (_headerAdditionalLine))
                  y++;
                RenderViewItem renderView;
                renderView.col = 0;
                renderView.row = r->value;
                renderView.pos = rid;
                renderView.x = x;
                renderView.y = y;
                renderView.w = 0;
                renderView.h = 1;
                _renderRows.push_back(renderView);
                /* Next row */
                y++;
                /* Check if reach height */
                if ((!_renderAllItem) && (y >= h))
                  break;
                rid++;
              }
          }
        /* Set fixed columns fixed */
        for (FixedColumnTable::iterator it = fixedColumns.begin();
            it != fixedColumns.end(); ++it)
          {
            /* Get the postion where to set the column */
            unsigned int searchId = (*it)->fixedOffset;
            if ((*it)->fixedAtEnd)
              {
                searchId = _renderColumns.size() - (*it)->fixedOffset - 1;
              }
            /* Set the column at the right position */
            unsigned int id = 0;
            int offsetX = 0;
            int offsetPos = 0;
            for (RenderList::iterator c = _renderColumns.begin();
                c != _renderColumns.end(); ++c)
              {
                if (id == searchId)
                  {
                    RenderViewItem renderView(*c);
                    renderView.col = (*it);
                    offsetX += (*it)->w;
                    offsetPos += 1;
                    renderView.w = (*it)->w;
                    if (searchId >= _renderColumns.size() - 1)
                      {
                        renderView.pos++;
                        renderView.x += c->w;

                        _renderColumns.push_back(renderView);
                        break;
                      }
                    else
                      {
                        c = _renderColumns.insert(c, renderView);
                      }
                  }
                else
                  {
                    c->x += offsetX;
                    c->pos += offsetPos;
                  }
                id++;
              }
          }

        /* Set fixed rows fixed */
        for (FixedRowTable::iterator it = fixedRows.begin();
            it != fixedRows.end(); ++it)
          {
            /* Get the postion where to set the row */
            unsigned int searchId = (*it)->fixedOffset;
            if ((*it)->fixedAtEnd)
              {
                searchId = _renderRows.size() - (*it)->fixedOffset - 1;
              }
            /* Set the row at the right position */
            unsigned int id = 0;
            int offsetY = 0;
            for (RenderList::iterator r = _renderRows.begin();
                r != _renderRows.end(); ++r)
              {
                if (id == searchId)
                  {
                    RenderViewItem renderView(*r);
                    renderView.row = (*it);
                    renderView.pos++;
                    if (searchId >= _renderRows.size() - 1)
                      {
                        renderView.y++;
                        _renderRows.push_back(renderView);
                        break;
                      }
                    else
                      {
                        r = _renderRows.insert(r, renderView);
                      }
                    offsetY = 1;
                  }
                else
                  {
                    r->y += offsetY;
                  }
                id++;
              }
          }

      }
  }

  /* -------------------------------------------------------------------------- */
  /* Properties - function */

  /** +getAdditionalHeaderLine */
  bool
  GridView::getAdditionalHeaderLine() const
  {
    return _headerAdditionalLine;
  }

  /** +getAdditionalSumLine */
  bool
  GridView::getAdditionalSumLine(bool additionalLine)
  {
    return _sumAdditionalLine;
  }

  /** +setAdditionalHeaderLine */
  void
  GridView::setAdditionalHeaderLine(bool additionalLine)
  {
    if (_headerAdditionalLine != additionalLine)
      {
        _headerAdditionalLine = additionalLine;
        _updateCounter++;
      }
  }

  /** +setAdditionalSumLine */
  void
  GridView::setAdditionalSumLine(bool additionalLine)
  {
    if (_sumAdditionalLine != additionalLine)
      {
        _sumAdditionalLine = additionalLine;
        _updateCounter++;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Event - Mode - function */

  /** #onRowAdded */
  void
  GridView::onRowAdded(GridView::RowView& r, GridView::RowView* afterRow)
  {
    /* DO NOTHING - by default */;
  }
  /** #onRowDelete */
  void
  GridView::onRowDelete(GridView::RowView& r)
  {
    /* DO NOTHING - by default */;
  }
  /** #onRowChange */
  void
  GridView::onRowChange(GridView::RowView& r)
  {
    /* DO NOTHING - by default */;
  }
  /** #onRowMoved */
  void
  GridView::onRowMoved(GridView::RowView& r, GridView::RowView* afterRow)
  {
    /* DO NOTHING - by default */;
  }
  /** #onRowClear */
  void
  GridView::onRowClear()
  {
    /* DO NOTHING - by default */;
  }

  /** #onColumnAdded */
  void
  GridView::onColumnAdded(GridView::ColumnView& c,
      GridView::ColumnView* afterColumn)
  {
    /* DO NOTHING - by default */;
  }
  /** #onColumnDelete */
  void
  GridView::onColumnDelete(GridView::ColumnView& c)
  {
    /* DO NOTHING - by default */;
  }
  /** #onColumnChange */
  void
  GridView::onColumnChange(GridView::ColumnView& c)
  {
    /* DO NOTHING - by default */;
  }
  /** #onColumnMoved */
  void
  GridView::onColumnMoved(GridView::ColumnView& c,
      GridView::ColumnView* afterColumn)
  {
    /* DO NOTHING - by default */;
  }
  /** #onColumnClear */
  void
  GridView::onColumnClear()
  {
    /* DO NOTHING - by default */;
  }

  /** #onCellChange */
  void
  GridView::onCellChange(GridView::RowView& r, GridView::ColumnView& c,
      const Value& newValue)
  {
    /* DO NOTHING - by default */;
  }

}
