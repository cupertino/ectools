#include <libec/grid/model/GridModel.h>
#include <libec/grid/view/GridView.h>
#include <libec/grid/view/GridViewLoader.h>
#include <libec/grid/filter/GridFilter.h>

namespace cea
{

  /* Static value */
  Value GridModel::nvalue;
  GridModel GridModel::ngrid;
  GridColumn GridModel::ncolumn(ngrid, 0, "#NAN", Value::UNKNOWN);
  GridRow GridModel::nrow(ngrid, 0);
  GridCell GridModel::ncell(ngrid, nrow, ncolumn);

  /* -------------------------------------------------------------------------- */
  /* Constructor/Destructor */

  /** +Constructor */
  GridModel::GridModel() :
      _updateCounter(0), eventMode(false), _filter(*(new GridFilter))
  {
    ;
  }

  /** +Constructor */
  GridModel::GridModel(const GridModel& grid) :
      _updateCounter(0), eventMode(false), _filter(*(new GridFilter))
  {
    ;
  }

  /** +Destructor */
  GridModel::~GridModel()
  {
    delete (&_filter);
    /* Delete all the rows */
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        delete (*r);
      }
    /* Clear all the Column */
    for (ColumnList::iterator c = columns.begin(); c != columns.end(); ++c)
      {
        delete (*c);
      }
    /* Clear all the cells */
    for (CellList::iterator c = cells.begin(); c != cells.end(); ++c)
      {
        delete *c;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Column section */

  /** +clearColumn */
  GridModel&
  GridModel::clearColumn()
  {
    /* Fired event if event mode */
    if (eventMode)
      onGridColumnClear();
    /* Clear column in views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->clearColumn();
      }
    /* Clear all the Column */
    for (ColumnList::iterator c = columns.begin(); c != columns.end(); ++c)
      {
        delete (*c);
      }
    columns.clear();
    columnTags.clear();
    /* Refresh Filter */
    _filter.refresh(*this);
    /* Clear all the Row cells map */
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        (*r)->cells.clear();
      }
    /* Clear all the cells */
    for (CellList::iterator c = cells.begin(); c != cells.end(); ++c)
      {
        delete *c;
      }
    /* Return ref to the current object */
    return *this;
  }
  /** +addColumn */
  GridColumn&
  GridModel::addColumn(const std::string& title, const Value::Type type,
      int tag, unsigned int minimumSize, unsigned int percentWidth)
  {
    /* Create the column */
    GridColumn* col = new GridColumn(*this, tag, title, type, minimumSize,
        percentWidth);
    /* Add the column & return it */
    if (col != 0)
      {
        /* Set Id */
        col->_id = columns.size();
        /* "Map" the column and inform view */
        columns.push_back(col);
        privAddColumn(col);
        /* Fired event if event mode */
        if (eventMode)
          onGridColumnCreated(*col);
        /* Return a ref to the created column */
        return *col;
      }
    else
      {
        return ncolumn;
      }
  }
  /** +removeColumn */
  GridModel&
  GridModel::removeColumn(GridColumn& col)
  {
    bool bDeleteDone = false;
    /* Pointer of the column */
    GridColumn* ptrCol = &col;
    /* Search and remove the column */
    for (ColumnList::iterator it = columns.begin(); it != columns.end();
        /*NOP*/)
      {
        if (!bDeleteDone)
          {
            if ((*it) == ptrCol)
              {
                /* Fired event if event mode */
                if (eventMode)
                  onGridColumnDelete(col);
                /* Informn the view & delete the linked cell */
                privRemoveColumn(*it);
                privRemoveColumnTagMap(*it);
                /* Delete from memory */
                delete (*it);
                /* Remove the Column */
                it = columns.erase(it);
                bDeleteDone = true;
              }
            else
              {
                ++it;
              }
          }
        else
          {
            /* Redduce the id of each next column */
            (*it)->_id--;
            ++it;
          }
      }
    /* Return the ref to the current object */
    return *this;
  }
  /** +removeColumn */
  GridModel&
  GridModel::removeColumn(unsigned int id)
  {
    /* Remove the Column if the column exist */
    if (id >= columns.size())
      {
        return *this;
      }
    else
      {
        return removeColumn(*(columns[id]));
      }
  }

  /** +getColumnCount */
  unsigned int
  GridModel::getColumnCount()
  {
    return columns.size();
  }
  /** +getColumnId */
  unsigned int
  GridModel::getColumnId(const GridColumn& col)
  {
    return col._id;
  }
  /** +getColumn */
  GridColumn&
  GridModel::getColumn(unsigned int id)
  {
    /* Return the Column */
    if (id >= columns.size())
      {
        return ncolumn;
      }
    else
      {
        return *(columns[id]);
      }
  }
  /** +isValid */
  bool
  GridModel::isValid(const GridColumn& col)
  {
    return ((&col) != (&ncolumn));
  }

  /* -------------------------------------------------------------------------- */
  /* Row section */

  /** +clearRow */
  GridModel&
  GridModel::clearRow()
  {
    /* Fired event if event mode */
    if (eventMode)
      onGridRowClear();
    /* Clear row in views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->clearRow();
      }
    /* Delete all the rows */
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        delete (*r);
      }
    rows.clear();
    rowTags.clear();
    /* Clear all the Column cells map */
    for (ColumnList::iterator c = columns.begin(); c != columns.end(); ++c)
      {
        (*c)->cells.clear();
      }
    /* Clear all the cells */
    for (CellList::iterator c = cells.begin(); c != cells.end(); ++c)
      {
        delete *c;
      }
    /* Return the ref to the current object */
    return *this;
  }
  /** +addRow */
  GridRow&
  GridModel::addRow(int tag)
  {
    /* Create the row */
    GridRow* r = new GridRow(*this, tag);
    if (r != 0)
      {
        /* Set Id */
        r->_id = rows.size();
        /* "Map" the row and inform view */
        rows.push_back(r);
        privAddRow(r);
        /* Fired event if event mode */
        if (eventMode)
          onGridRowCreated(*r);
        /* Return the row */
        return *r;
      }
    else
      {
        return nrow;
      }
  }
  /** +removeRow */
  GridModel&
  GridModel::removeRow(GridRow& row)
  {
    bool bDeleteDone = false;
    /* Pointer of the row */
    GridRow* ptrRow = &row;
    /* Search and remove the row */
    for (RowList::iterator it = rows.begin(); it != rows.end();
    /*NOP*/)
      {
        if (!bDeleteDone)
          {
            if ((*it) == ptrRow)
              {
                /* Fired event if event mode */
                if (eventMode)
                  onGridRowDelete(row);
                /* Informn the view & delete the linked cell */
                privRemoveRow(*it);
                privRemoveRowTagMap(*it);
                /* Delete from memory */
                delete (*it);
                /* Remove the row */
                it = rows.erase(it);
                bDeleteDone = true;
              }
            else
              {
                ++it;
              }
          }
        else
          {
            /* Reduce the id of each next row */
            ((*it)->_id)--;
            ++it;
          }

      }
    /* Return the ref to the current object */
    return *this;
  }
  /** +removeRow */
  GridModel&
  GridModel::removeRow(unsigned int id)
  {
    /* Remove the Row if the row exist */
    if (id >= rows.size())
      {
        return *this;
      }
    else
      {
        return removeRow(*(rows[id]));
      }
  }

  /** +getRowCount */
  unsigned int
  GridModel::getRowCount()
  {
    return rows.size();
  }
  /** +getRowId */
  unsigned int
  GridModel::getRowId(const GridRow& row)
  {

    /* Check if the current id is ok */
    if ((row._id >= rows.size()) && (rows[row._id] != &row))
      {
        /* Search the id */
        unsigned int i = 0;
        for (RowList::iterator it = rows.begin(); it != rows.end(); ++it, i++)
          {
            if ((*it) == &row)
              {
                /* Id find */
                (*it)->_id = i;
                return i;
              }
          }
        /* Id don't find */
        return npos;
      }
    else
      {
        /* Return the id */
        return row._id;
      }
  }
  /** +getRow */
  GridRow&
  GridModel::getRow(unsigned int id)
  {
    /* Return the Row */
    if (id >= rows.size())
      {
        return nrow;
      }
    else
      {
        return *(rows[id]);
      }
  }
  /** +isValid */
  bool
  GridModel::isValid(const GridRow& row)
  {
    return ((&row) != (&nrow));
  }

  /* -------------------------------------------------------------------------- */
  /* Cell section  */

  /** +getCell*/
  const GridCell&
  GridModel::getCell(const GridRow& row, const GridColumn& col)
  {
    return row.getCell(col);
  }
  /** +getCell */
  const GridCell&
  GridModel::getCell(const GridRow& row, unsigned int colId)
  {
    if (colId >= columns.size())
      {
        return ncell;
      }
    else
      {
        return row.getCell(*(columns[colId]));
      }
  }
  /** +getCell */
  const GridCell&
  GridModel::getCell(unsigned int rowId, const GridColumn& col)
  {
    if (rowId >= rows.size())
      {
        return ncell;
      }
    else
      {
        return rows[rowId]->getCell(col);
      }
  }
  /** +getCell */
  const GridCell&
  GridModel::getCell(unsigned int rowId, unsigned int colId)
  {
    if ((rowId >= rows.size()) || (colId >= columns.size()))
      {
        return ncell;
      }
    else
      {
        return rows[rowId]->getCell(*(columns[colId]));
      }
  }
  /** +isValid */
  bool
  GridModel::isValid(const GridCell& cell)
  {
    return ((&cell) != (&ncell));
  }

  /** +getValue */
  const Value&
  GridModel::getValue(const GridRow& row, const GridColumn& col)
  {
    return row.getValue(col);
  }
  /** +getValue */
  const Value&
  GridModel::getValue(const GridRow& row, unsigned int colId)
  {
    if (colId >= columns.size())
      {
        return nvalue;
      }
    else
      {
        return row.getValue(*(columns[colId]));
      }
  }
  /** +getValue */
  const Value&
  GridModel::getValue(unsigned int rowId, const GridColumn& col)
  {
    if (rowId >= rows.size())
      {
        return nvalue;
      }
    else
      {
        return rows[rowId]->getCell(col).get();
      }
  }
  /** +getValue */
  const Value&
  GridModel::getValue(unsigned int rowId, unsigned int colId)
  {
    if ((rowId >= rows.size()) || (colId >= columns.size()))
      {
        return nvalue;
      }
    else
      {
        return rows[rowId]->getCell(*(columns[colId])).get();
      }
  }
  /** +isValid */
  bool
  GridModel::isValid(const Value& v)
  {
    return ((&v) != (&nvalue));
  }

  /** +setValue */
  GridModel&
  GridModel::setValue(GridRow& row, GridColumn& col, const Value& v)
  {
    row.setValue(col, v);
    return *this;
  }
  /** +setValue */
  GridModel&
  GridModel::setValue(GridRow& row, unsigned int colId, const Value& v)
  {
    if (colId < columns.size())
      {
        row.setValue(*(columns[colId]), v);
      }
    return *this;
  }
  /** +setValue */
  GridModel&
  GridModel::setValue(unsigned int rowId, GridColumn& col, const Value& v)
  {
    if (rowId < rows.size())
      {
        rows[rowId]->setValue(col, v);
      }
    return *this;
  }
  /** +setValue */
  GridModel&
  GridModel::setValue(unsigned int rowId, unsigned int colId, const Value& v)
  {
    if ((rowId < rows.size()) && (colId < columns.size()))
      {
        rows[rowId]->setValue(*(columns[colId]), v);
      }
    return *this;
  }

  /* -------------------------------------------------------------------------- */
  /* Filter Section */

  /** Set the filter */
  void
  GridModel::setFilter(const std::string& str)
  {
    _filter.set(str);
    _filter.refresh(*this);
  }

  /** @return the filter */
  std::string
  GridModel::getFilter() const
  {
    return _filter.get();
  }

  /* -------------------------------------------------------------------------- */
  /* View management  */

  /** +clearView */
  GridModel&
  GridModel::clearView()
  {
    /* Clear all the views */
    for (ViewList::iterator it = views.begin(); it != views.end(); ++it)
      {
        /* Call view to erase all */
        (*it)->clearRow();
        (*it)->clearColumn();
      }
    views.clear();
    /* Return the reference of the current object */
    return *this;
  }

  /** +connectView */
  GridModel&
  GridModel::connectView(GridView& view)
  {
    /* Check if the view already present */
    for (ViewList::iterator it = views.begin(); it != views.end(); ++it)
      {
        if ((*it) == &view)
          {
            return *this;
          }
      }
    /* If not found add the view */
    views.push_back(&view);
    /* Copy all the existing column */
    for (ColumnList::iterator c = columns.begin(); c != columns.end(); ++c)
      {
        view.addColumn(*c);
      }
    /* Copy all the existing row */
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        view.addRow(*r);
      }
    /* Return the reference of the current object */
    return *this;
  }
  /** +removeView */
  GridModel&
  GridModel::removeView(GridView& view)
  {
    /* Search the view and destroy it */
    for (ViewList::iterator it = views.begin(); it != views.end(); ++it)
      {
        if ((*it) == &view)
          {
            /* Call view to erase all */
            (*it)->clearRow();
            (*it)->clearColumn();
            /* Remove view from list */
            views.erase(it);
            break;
          }
      }
    /* Return the reference of the current object */
    return *this;
  }

  /** #isRowVisible */
  bool
  GridModel::isRowVisible(const GridRow& r)
  {
    /* Check in all view if it's a "visible" row */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((*v)->isRowRendered(r))
          {
            return true;
          }
      }
    /* If this row is not rendered then return false */
    return false;
  }
  /** #isColumnVisible */
  bool
  GridModel::isColumnVisible(const GridColumn& c)
  {
    /* Check in all view if it's a "visible" column */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((*v)->isColumnRendered(c))
          {
            return true;
          }
      }
    /* If this column is not rendered then return false */
    return false;
  }

  /** #isCellVisible */
  bool
  GridModel::isCellVisible(const GridRow& r, const GridColumn& c)
  {
    return ((isRowVisible(r)) && (isColumnVisible(c)));
  }
  /** #isViewSortByColumn */
  bool
  GridModel::isViewSortByColumn(const GridColumn& c)
  {
    /* Search if column sorted in view */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((&((*v)->getSortRefColumn())) == &c)
          {
            return true;
          }
      }
    /* If this column is not sorted in view return false */
    return false;
  }
  /** #isViewSumEnable */
  bool
  GridModel::isViewColumnSumEnabled(const GridColumn& c)
  {
    /* Search if column have sum activate in view */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        if ((*v)->isColumnSumEnabled(c))
          {
            return true;
          }
      }
    /* If this column have no sum in any view return false */
    return false;
  }
  /* #isViewNeedData */
  bool
  GridModel::isViewNeedData(const GridColumn& c)
  {
    return ((isViewSortByColumn(c)) || (isViewColumnSumEnabled(c))
        || (isColumnVisible(c)));
  }
  /* #isViewNeedData */
  bool
  GridModel::isViewNeedData(const GridRow& r, const GridColumn& c)
  {
    return ((isViewSortByColumn(c)) || (isViewColumnSumEnabled(c))
        || ((isRowVisible(r)) && (isColumnVisible(c))));
  }

  /* -------------------------------------------------------------------------- */
  /* Other function  */

  /** +clear */
  GridModel&
  GridModel::clear()
  {
    /* Clear Column/Row */
    clearRow();
    clearColumn();
    /* Return the reference of the current object */
    return *this;
  }
  /** +isValid */
  bool
  GridModel::isValid(unsigned int id)
  {
    return (id != npos);
  }

  /** +getUpdateCount */
  unsigned int
  GridModel::getUpdateCount() const
  {
    return _updateCounter;
  }
  /** +incUpdateCounter */
  void
  GridModel::incUpdateCounter()
  {
    _updateCounter++;
    if (_updateCounter > 1234567890)
      {
        _updateCounter = 0;
      }
  }

  /* -------------------------------------------------------------------------- */
  /* Protected function */

  /** +createCell */
  GridCell*
  GridModel::createCell(GridRow& forRow, GridColumn& forColumn)
  {
    /* Create the Cell */
    GridCell* cell = new GridCell(*this, forRow, forColumn);
    if (cell == 0)
      return 0;
    /* Add into list of cells */
    cells.insert(cell);
    /* Add into GridRow cells map */
    forRow.cells.insert(GridRow::CellMap::value_type(&forColumn, cell));
    /* Add into GridColumn cells map */
    forColumn.cells.insert(GridColumn::CellMap::value_type(&forRow, cell));
    /* Return the cell */
    return cell;
  }

  /** #setGridColumnName */
  void
  GridModel::setGridColumnName(GridColumn& col, const std::string& name)
  {
    col._name.assign(name);
  }

  /** #getGridColumnByName */
  GridColumn&
  GridModel::getGridColumnByName(const std::string& name, bool ignoreCase)
  {
    /* Search the column */
    for (ColumnList::iterator c = columns.begin(); c != columns.end(); ++c)
      {
        if (ignoreCase)
          {
            if (Tools::identicalString(name, (*c)->_name))
              {
                return *(*c);
              }
          }
        else
          {
            if ((*c)->_name.compare(name) == 0)
              {
                return *(*c);
              }
          }
      }
    /* If not found return ncolumn */
    return ncolumn;
  }

  /** #setGridItemData */
  void
  GridModel::setGridItemData(GridItem& it, void* data)
  {
    it.setData(data);
  }

  /* -------------------------------------------------------------------------- */
  /* Protected "Event" function */

  /** #onGridRowCreated */
  void
  GridModel::onGridRowCreated(GridRow& r)
  {
    /* DO NOTHING - by default */;
  }
  /** #onGridRowDelete */
  void
  GridModel::onGridRowDelete(GridRow& r)
  {
    /* DO NOTHING - by default */;
  }
  /** #onGridRowClear */
  void
  GridModel::onGridRowClear()
  {
    /* DO NOTHING - by default */;
  }

  /** #onGridColumnCreated */
  void
  GridModel::onGridColumnCreated(GridColumn& c)
  {
    /* DO NOTHING - by default */;
  }
  /** #onGridColumnDelete */
  void
  GridModel::onGridColumnDelete(GridColumn& c)
  {
    /* DO NOTHING - by default */;
  }
  /** #onGridColumnClear */
  void
  GridModel::onGridColumnClear()
  {
    /* DO NOTHING - by default */;
  }

  /* -------------------------------------------------------------------------- */
  /* Private function */

  /** -privAddColumn */
  void
  GridModel::privAddColumn(GridColumn* col)
  {
    /* "Map" with column tag */
    if (col->tag != -1)
      {
        columnTags.insert(ColumnTagMap::value_type(col->tag, col));
      }
    /* Add to the views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->addColumn(col);
      }
    /* Refresh Filter */
    _filter.refresh(*this);
  }
  /** -privRemoveColumn */
  void
  GridModel::privRemoveColumn(GridColumn* col)
  {
    /* Remove the linked cell */
    for (GridColumn::CellMap::iterator c = col->cells.begin();
        c != col->cells.end(); ++c)
      {
        /* Remove column from row */
        c->second->row.cells.erase(col);
        /* Delete the cell */
        delete c->second;
        /* Remove cells from cell list */
        cells.erase(c->second);

      }
    /* Remove from the views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->removeColumn(col);
      }
    /* Refresh Filter */
    _filter.refresh(*this);
  }
  /** -privRemoveColumnTagMap */
  void
  GridModel::privRemoveColumnTagMap(GridColumn* col)
  {
    /* Remove from TagMap */
    if (col->tag != -1)
      {
        std::pair<ColumnTagMap::iterator, ColumnTagMap::iterator> ret =
            columnTags.equal_range(col->tag);
        for (ColumnTagMap::iterator it = ret.first; it != ret.second; ++it)
          {
            if (it->second == col)
              {
                columnTags.erase(it);
                break;
              }
          }
      }
  }

  /** -privAddRow */
  void
  GridModel::privAddRow(GridRow* row)
  {
    /* "Map" with row tag */
    if (row->tag != -1)
      {
        rowTags.insert(RowTagMap::value_type(row->tag, row));
      }
    /* Add to the views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->addRow(row);
      }
  }
  /** -privRemoveRow */
  void
  GridModel::privRemoveRow(GridRow* row)
  {
    /* Remove from the views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->removeRow(row);
      }
    /* Remove the linked cell */
    for (GridRow::CellMap::iterator c = row->cells.begin();
        c != row->cells.end(); ++c)
      {
        /* Remove row from column */
        c->second->column.cells.erase(row);
        /* Delete the cell */
        delete c->second;
        /* Remove cells from cell list */
        cells.erase(c->second);
      }
  }
  /** -privRemoveRowTagMap */
  void
  GridModel::privRemoveRowTagMap(GridRow* row)
  {
    /* Remove from TagMap */
    if (row->tag != -1)
      {
        std::pair<RowTagMap::iterator, RowTagMap::iterator> ret =
            rowTags.equal_range(row->tag);
        for (RowTagMap::iterator it = ret.first; it != ret.second; ++it)
          {
            if (it->second == row)
              {
                rowTags.erase(it);
                break;
              }
          }
      }
  }

  /** -viewRefreshCell */
  void
  GridModel::viewRefreshCell(GridCell* cell, const Value& oldValue)
  {
    /* Get Property */
    GridRow* r = &(cell->row);
    GridColumn* c = &(cell->column);
    const Value& val = cell->get();
    /* Refresh all views */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->refreshCell(r, c, oldValue, val);
      }
  }

  /** -viewRefreshColumn */
  void
  GridModel::viewRefreshColumn(GridColumn* col)
  {
    /* Send to all view to refresh column */
    for (ViewList::iterator v = views.begin(); v != views.end(); ++v)
      {
        (*v)->refreshColumn(col);
      }
  }

}
