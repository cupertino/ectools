///////////////////////////////////////////////////////////////////////////////
/// @file		GridView.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base class for grid view
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDVIEW_H__
#define LIBEC_GRIDVIEW_H__

#include "../model/GridModel.h"
#include "../model/GridCell.h"
#include "../model/GridColumn.h"
#include "../model/GridRow.h"
#include "GridStyle.h"
#include "../../tools/containers/DoubleLinkedList.h"

#include "GridViewLog.h"
#include "GridViewLoader.h"

namespace cea
{
  /* Prototype */
  class GridFilter;

  ///@brief Base class for grid view
  class GridView : public GridViewLog, public GridViewLoader
  {
  public:

    /* Public - Declaration */

    /// @brief Autosize Settings
    struct AutoSize
    {
      // Position
      bool autoWidth; ///< If true then Width will be auto calculated
      bool autoHeight; ///< If true then Height will be auto calculated
      // Offset
      int offsetW; ///< Auto width = (view)width - offsetW
      int offsetH; ///< Auto height = (view)height - offsetH

      // Constructor
      /// @brief Default Constructor set auto mode for width and height
      AutoSize();
      /// @brief Auto size for width and height with offset set
      /// @param offsetW value substract to auto calculated width
      /// @param offsetH value substract to auto calculated height
      AutoSize(int offsetW, int offsetH);
      /// @brief Constructor
      AutoSize(bool autoWidth, bool autoHeight);
      /// @brief Set autosize for width/height with offset
      /// @param autoWidth If true then Width will be auto calculated
      /// @param offsetW Auto width = (view)width - offsetW
      /// @param autoHeight If true then Height will be auto calculated
      /// @param offsetH Auto height = (view)height - offsetH
      AutoSize(bool autoWidth, int offsetW, bool autoHeight, int offsetH);
    };

    /// @brief Sort order
    enum SortOrder
    {
      ASCENDING, ///< Ascending: A to Z
      DESCENDING ///< Descending: Z to A
    };

    /// @brief View of a GridColumn
    struct ColumnView
    {
      unsigned int uid; ///< Unique id - usefull for event mode
      GridColumn* ref; ///< GridModel column reference
      GridStyle* style; ///< Style of cells of this columns
      GridStyle* headerStyle; ///< Style of header
      GridStyle* sumStyle; ///< Style of sum cell
      unsigned int w; ///< Size of the column
      bool visible; ///< Visibility status of the column
      bool fixed; ///< Indicate if the column is fixed
      bool fixedAtEnd; ///< If true fixed from right of the view
      int fixedOffset; ///< Indicate column fix position

      /// @brief Pointer to the container case in a double linked list
      ///
      /// This members is just for optimization and should be never
      /// manipulated.
      ///
      DoubleLinkedList<ColumnView*>::item* it;

      bool sumEnable; ///< Indicate if sum calcul of the column is enable
      Value sum; ///< Current sum cells column value
      bool needSumRecalc; ///< Indicate if sum need to be recalculated
    };
    /// @brief View of a GridRow
    struct RowView
    {
      unsigned int uid; ///< Unique id - usefull for event mode
      GridRow* ref; ///< GridModel row reference
      GridStyle* style; ///< Style of cells of this rows
      GridFilter* styleFromFilter; ///< Last style filter applied
      bool visible; ///< Visibility status of the row
      bool fixed; ///< Indicate if the row is fixed
      bool fixedAtEnd; ///< If true fixed from bottom of the view
      int fixedOffset; ///< Indicate row fix position

      /// @brief Pointer to the container case in a double linked list
      ///
      /// This members is just for optimization and should be never
      /// manipulated.
      ///
      DoubleLinkedList<RowView*>::item* it;
    };
    /// @brief Render option
    struct RenderViewItem
    {
      ColumnView* col; ///< View of column to render
      RowView* row; ///< View of row to render
      int pos; ///< id of element to render
      int x; ///< X-coordinate of item to render
      int y; ///< Y-coordinate of item to render
      int w; ///< Width of item to render
      int h; ///< Height of item to render
    };

    /* Type of column mapping */
    /// @brief Map model column with his view
    typedef std::map<GridColumn*, ColumnView*> ColumnMap;
    /// @brief List of column view loaded
    typedef DoubleLinkedList<ColumnView*> ColumnTable;
    /// @brief List containing all the fixed column
    typedef std::set<ColumnView*> FixedColumnTable;

    /* Type of row mapping */
    /// @brief Map model column with his view
    typedef std::map<GridRow*, RowView*> RowMap;
    /// @brief List of row view loaded
    typedef DoubleLinkedList<RowView*> RowTable;
    /// @brief List containing all the fixed row
    typedef std::set<RowView*> FixedRowTable;

    /// @brief Render list - item to render
    typedef std::list<RenderViewItem> RenderList;

    /* Type of Style */
    /// @brief Map Style with int
    typedef std::map<int, GridStyle*> StyleMap;
    /// @brief Map style number with Filter
    typedef std::map<GridFilter*, int> StyleFilterMap;

    /* Static style */
    /// @brief Default column cell style
    static GridStyle defaultColumnStyle;
    /// @brief Default column header style
    static GridStyle defaultColumnHeaderStyle;
    /// @brief Default column sum cell style
    static GridStyle defaultColumnSumStyle;
    /// @brief Default row style
    static GridStyle defaultRowStyle;

  private:

    /* GridViewLog & GridViewLoader integration */
    /// @brief Log integration
    /// @return Pointer of this current GridView
    GridView*
    getGridViewLogRef();

    /// @brief Loader integration
    /// @return Pointer of this current GridView
    GridView*
    getGridViewLoaderRef();

  private:

    /* Function called by GridModel */

    /// @brief Add a column model
    /// @param c Pointer of the column model to add
    void
    addColumn(GridColumn* c);
    /// @brief Remove a column model
    /// @param c Pointer of the column model to remove
    void
    removeColumn(GridColumn* c);
    /// @brief Clear all column model
    void
    clearColumn();

    /// @brief Add a row model
    /// @param c Pointer of the row model to add
    void
    addRow(GridRow* r);
    /// @brief Remove a row model
    /// @param c Pointer of the row model to remove
    void
    removeRow(GridRow* r);
    /// @brief Clear all row model
    void
    clearRow();

    /// @brief Refresh a cell - called when cell change value
    /// @param row Pointer of the cell row
    /// @param col Pointer of the cell column
    /// @param oldValue Value before change value of cell
    /// @param newValue New value of the cell
    virtual void
    refreshCell(GridRow* row, GridColumn* col, const Value& oldValue,
        const Value& newValue);

    /// @brief Refresh a entire column
    /// @param col Pointer of the column model to refresh
    virtual void
    refreshColumn(GridColumn* col);

    /* Sort method */
    /// @brief Sort a specific row
    /// @param r Pointer of the row to place
    void
    sortARow(RowView* r);

    /// @brief Get row sort position
    ///
    /// @return 0 if the sort position is the start of the list.
    ///           Or the pointer of the previous RowView
    RowView*
    getRowViewOrderedAfter(RowView* r);

    /* Sum Column */
    /// @brief Calcul all the total sum of each cell of a column
    /// @param col Pointer of the column to calcul
    void
    calculSumColumn(ColumnView* col);
    /// @brief Clear all sum result of all column
    ///
    /// This function is called when all row are deleted
    ///
    void
    clearAllSumColumn();

    /// @brief Increment a column sum by v
    /// @param col Pointer of the column
    /// @param v Value to add to sum
    void
    incSumColumn(ColumnView* col, const Value& v);
    /// @brief Increment all column sum of cells associated to a row
    /// @param r Pointer of the row
    void
    incSumColumn(RowView* r);
    /// @brief Decrement a column sum by v
    /// @param col Pointer of the column
    /// @param v Value to substract to sum
    void
    decSumColumn(ColumnView* col, const Value& v);
    /// @brief Decrement all column sum of cells associated to a row
    /// @param r Pointer of the row
    void
    decSumColumn(RowView* r);

  public:

    /* Static variables */
    /// @brief Constants representing an invalid position
    static const unsigned int npos = (unsigned int) (-1);
    /// @brief Represent an invalid style
    static GridStyle nstyle;

    /* Constructor/Destructor */
    /// @brief Default Constructor
    GridView();
    /// @brief Construct a view for model
    /// @param model GridModel to connect to
    GridView(GridModel& model);

    /// @brief Delete all created memory element
    ~GridView();

    /* Sort section */
    /// @brief Enable sort by a column
    /// @param byColumnViewId Column by position in the view
    /// @param sortOrder Order of sorting
    void
    sort(unsigned int byColumnViewId, SortOrder sortOrder = DESCENDING);
    /// @brief Enable sort by a column
    /// @param byColumn Column by GridColumn
    /// @param sortOrder Order of sorting
    void
    sort(const GridColumn& byColumn, SortOrder sortOrder = DESCENDING);
    /// @brief Disable sort
    ///
    /// All row keep current position.
    ///
    void
    disableSort();
    /// @brief Sort all the rows of the view
    void
    sortRows();

    /// @brief Check is sort is enabled
    /// @return True if sort enabled
    bool
    isSortEnabled() const;
    /// @brief Get sort reference column position in the view
    /// @return Position in the view of the sort reference. \n
    ///         Or GridView::npos if sort disabled
    unsigned int
    getSortRefColumnViewId() const;
    /// @brief Get sort reference column
    ///
    /// To test if the return column is valid use GridModel::isValid.
    ///
    /// @return Sort reference GridColumn. \n
    ///         Or GridModel::ncolumn if sort disabled
    const GridColumn&
    getSortRefColumn() const;
    /// @brief Get sort Order
    /// @return Sort order value. \n
    ///         Or last sort order value if sort disabled
    SortOrder
    getSortOrder() const;

    /* Show/Hide/Move Column */
    /// @brief Set column visibility
    /// @param col Column by GridColumn
    /// @param visible Visible status (if True then visible else hidden)
    /// @return Reference to the current object
    GridView&
    setColumnVisibility(const GridColumn& col, bool visible);
    /// @brief Set column visibility
    /// @param colViewId Column by position in the view
    /// @param visible Visible status (if True then visible else hidden)
    /// @return Reference to the current object
    GridView&
    setColumnVisibility(unsigned int colViewId, bool visible);
    /// @brief Show a column
    /// @param col Column by GridColumn
    /// @return Reference to the current object
    GridView&
    showColumn(const GridColumn& col);
    /// @brief Show a column
    /// @param colViewId Column by position in the view
    /// @return Reference to the current object
    GridView&
    showColumn(unsigned int colViewId);
    /// @brief Hide a column
    /// @param col Column by GridColumn
    /// @return Reference to the current object
    GridView&
    hideColumn(const GridColumn& col);
    /// @brief Hide a column
    /// @param colViewId Column by position in the view
    /// @return Reference to the current object
    GridView&
    hideColumn(unsigned int colViewId);
    /// @brief Hide all column of the view
    /// @return Reference to the current object
    GridView&
    hideAllColumn();
    /// @brief Show all column of the view
    /// @return Reference to the current object
    GridView&
    showAllColumn();
    /// @brief Check if a column is visible
    /// @param col Column by GridColumn
    /// @return True if column visible
    bool
    isColumnVisible(const GridColumn& col) const;
    /// @brief Check if a column is visible
    /// @param colViewId Column by position in the view
    /// @return True if column visible
    bool
    isColumnVisible(unsigned int colViewId) const;
    /// @brief Get GridColumn position in the view
    /// @param col GridColumn considered
    /// @return Position of the GridColumn in the view. \n
    ///         Or GridView::npos if the GridColumn is not present
    ///         in this view
    unsigned int
    getColumnViewId(const GridColumn& col) const;
    /// @brief Get First grid GridView::ColumnView in the view
    /// @return First GridView::ColumnView. \n
    ///         Or O if no column in this view.
    ColumnView*
    getFirstColumnView();
    /// @brief Move a column after a position
    /// @param col Column by GridColumn
    /// @param afterViewId If < 0 then Column is moved at the
    ///                    first position. \n
    ///                    Else Column is moved after the ColumnView
    ///                    represented by afterViewId.
    /// @return Reference to the current object
    GridView&
    moveColumn(const GridColumn& col, int afterViewId);
    /// @brief Move a column after a position
    /// @param colViewId Column by position in the view
    /// @param afterViewId If < 0 then Column is moved at the
    ///                    first position. \n
    ///                    Else Column is moved after the ColumnView
    ///                    represented by afterViewId.
    /// @return Reference to the current object
    GridView&
    moveColumn(unsigned int colViewId, int afterViewId);
    /// @brief Fix a column
    /// @param col Column by GridColumn
    /// @param fixAtTheEnd If true Column will be positionned follow the
    ///                    right edge. \n
    ///                    Else Column will be possitionned follow the left
    ///                    edge.
    /// @param offset Offset of the fix follow fixAtTheEnd parameter
    /// @return Reference to the current object
    GridView&
    fixColumn(const GridColumn& col, bool fixAtTheEnd = false, int offset = 0);
    /// @brief Fix a column
    /// @param colViewId Column by position in the view
    /// @param fixAtTheEnd If true Column will be positionned follow the
    ///                    right edge. \n
    ///                    Else Column will be possitionned follow the left
    ///                    edge.
    /// @param offset Offset of the fix follow fixAtTheEnd parameter
    /// @return Reference to the current object
    GridView&
    fixColumn(unsigned int colViewId, bool fixAtTheEnd = false, int offset = 0);
    /// @brief Unfix a column
    /// @param col Column by GridColumn
    /// @return Reference to the current object
    GridView&
    unfixColumn(const GridColumn& col);
    /// @brief Unfix a column
    /// @param colViewId Column by position in the view
    /// @return Reference to the current object
    GridView&
    unfixColumn(unsigned int colViewId);

    /* Show/Hide/Move Row */
    /// @brief Set row visibility
    /// @param row Row by GridRow
    /// @param visible Visible status (if True then visible else hidden)
    /// @return Reference to the current object
    GridView&
    setRowVisibility(const GridRow& row, bool visible);
    /// @brief Set row visibility
    /// @param rowViewId Row by position in the view
    /// @param visible Visible status (if True then visible else hidden)
    /// @return Reference to the current object
    GridView&
    setRowVisibility(unsigned int rowViewId, bool visible);
    /// @brief Show a row
    /// @param row Row by GridRow
    /// @return Reference to the current object
    GridView&
    showRow(const GridRow& row);
    /// @brief Show a row
    /// @param rowViewId Row by position in the view
    /// @return Reference to the current object
    GridView&
    showRow(unsigned int rowViewId);
    /// @brief Hide a row
    /// @param row Row by GridRow
    /// @return Reference to the current object
    GridView&
    hideRow(const GridRow& row);
    /// @brief Hide a row
    /// @param rowViewId Row by position in the view
    /// @return Reference to the current object
    GridView&
    hideRow(unsigned int rowViewId);
    /// @brief Hide all row of the view
    /// @return Reference to the current object
    GridView&
    hideAllRow();
    /// @brief Show all row of the view
    /// @return Reference to the current object
    GridView&
    showAllRow();
    /// @brief Check if a row is visible
    /// @param row Row by GridRow
    /// @return True if row visible
    bool
    isRowVisible(const GridRow& row) const;
    /// @brief Check if a row is visible
    /// @param rowViewId Row by position in the view
    /// @return True if row visible
    bool
    isRowVisible(unsigned int rowViewId) const;
    /// @brief Get GridRow position in the view
    /// @param row GridRow considered
    /// @return Position of the GridRow in the view. \n
    ///         Or GridView::npos if the GridRow is not present
    ///         in this view
    unsigned int
    getRowViewId(const GridRow& row) const;
    /// @brief Get First grid GridView::RowView in the view
    /// @return First GridView::RowView. \n
    ///         Or O if no row in this view.
    RowView*
    getFirstRowView();
    /// @brief Move a row after a position
    /// @param row Row by GridRow
    /// @param afterViewId If < 0 then Row is moved at the
    ///                    first position. \n
    ///                    Else Row is moved after the RowView
    ///                    represented by afterViewId.
    /// @return Reference to the current object
    GridView&
    moveRow(const GridRow& row, int afterViewId);
    /// @brief Move a row after a position
    /// @param rowViewId Row by position in the view
    /// @param afterViewId If < 0 then Row is moved at the
    ///                    first position. \n
    ///                    Else Row is moved after the RowView
    ///                    represented by afterViewId.
    /// @return Reference to the current object
    GridView&
    moveRow(unsigned int rowViewId, int afterViewId);
    /// @brief Fix a row
    /// @param row Row by GridRow
    /// @param fixAtTheEnd If true Row will be positionned follow the
    ///                    right edge. \n
    ///                    Else Row will be possitionned follow the left
    ///                    edge.
    /// @param offset Offset of the fix follow fixAtTheEnd parameter
    /// @return Reference to the current object
    GridView&
    fixRow(const GridRow& row, bool fixAtTheEnd = false, int offset = 0);
    /// @brief Fix a row
    /// @param rowViewId Row by position in the view
    /// @param fixAtTheEnd If true Row will be positionned follow the
    ///                    right edge. \n
    ///                    Else Row will be possitionned follow the left
    ///                    edge.
    /// @param offset Offset of the fix follow fixAtTheEnd parameter
    /// @return Reference to the current object
    GridView&
    fixRow(unsigned int rowViewId, bool fixAtTheEnd = false, int offset = 0);
    /// @brief Unfix a row
    /// @param row Row by GridRow
    /// @return Reference to the current object
    GridView&
    unfixRow(const GridRow& row);
    /// @brief Unfix a row
    /// @param rowViewId Row by position in the view
    /// @return Reference to the current object
    GridView&
    unfixRow(unsigned int rowViewId);

    /* Sum-Bar section */
    /// @brief Enable/Disable sum calcul of a column
    /// @param col Column by GridColumn
    /// @param enable If true Enabled sum calcul otherwise disable
    /// @return Reference to the current object
    GridView&
    setColumnSum(const GridColumn& col, bool enable);
    /// @brief Enable/Disable sum calcul of a column
    /// @param colViewId Column by position in the view
    /// @param enable If true Enabled sum calcul otherwise disable
    /// @return Reference to the current object
    GridView&
    setColumnSum(unsigned int colViewId, bool enable);
    /// @brief Check if sum calcul of a column is enabled
    /// @param col Column by GridColumn
    /// @return True if sum enabled on the column
    bool
    isColumnSumEnabled(const GridColumn& col) const;
    /// @brief Check if sum calcul of a column is enabled
    /// @param colViewId Column by position in the view
    /// @return True if sum enabled on the column
    bool
    isColumnSumEnabled(unsigned int colViewId) const;

    /* List helpers Section */
    /// @brief Get the count of Row represented in the view
    /// @return RowView count
    unsigned int
    getRowViewCount() const;
    /// @brief Get a GridRow by position in the view
    /// @param rowViewId Row position in the view
    /// @return Corresponding GridRow if exist. \n
    ///         Or GridModel::nrow
    GridRow&
    getRow(unsigned int rowViewId);
    /// @brief Get the count of Column represented in the view
    /// @return ColumnView count
    unsigned int
    getColumnViewCount() const;
    /// @brief Get a GridColumn by position in the view
    /// @param colViewId Column position in the view
    /// @return Corresponding GridColumn if exist. \n
    ///         Or GridModel::ncolumn
    GridColumn&
    getColumn(unsigned int colViewId);
    /// @brief Get a ColumnView from an offset in the current "screen"
    ///        of view
    /// @param offsetViewId View position in the current "screen" of view
    /// @param considerFixedColumn If true take in account fixed column
    /// @return Corresponding ColumnView. \n
    ///         Or 0 if not found.
    ColumnView*
    getColumnView(int offsetViewId = 0, bool considerFixedColumn = false);

    /* Style Section */
    /// @brief Create or set style number id
    /// @param id Number(=key) of the style
    /// @param style GridStyle associated to id number
    /// @return Reference to the current object
    GridView&
    setStyle(unsigned int id, const GridStyle& style);
    /// @brief Get Style at number id
    ///
    /// To know if the style is valid use GridView::isValid.
    ///
    /// @param id Number(=key) of the style
    /// @return Const reference of the GridStyle if id exist. \n
    ///         Or return GridView::nstyle
    const GridStyle&
    getStyle(unsigned int id);
    /// @brief Get Read/Write access to GridStyle at number id
    ///
    /// To know if the style is valid use GridView::isValid.
    ///
    /// @param id Number(=key) of the style
    /// @return Reference to the GridStyle if exist. \n
    ///         Or reference to GridView::nstyle.
    GridStyle&
    style(unsigned int id);
    /// @brief Check if style is valid
    /// @param style Style to test
    /// @return True If (&style != &style).
    bool
    isValid(const GridStyle& style) const;

    /* Column Style */
    /// @brief Set style of a column
    /// @param col Column by GridColumn
    /// @param styleId Style to set for column cells. \n
    ///                If ==-1 Then set style
    ///                GridView::defaultColumnStyle
    /// @param headerStyleId Style to set for column header cell. \n
    ///                      If ==-1 Then set style
    ///                      GridView::defaultColumnHeaderStyle
    /// @param sumStyleId Style to set for column sum cell. \n
    ///                   If ==-1 Then set style
    ///                   GridView::defaultColumnSumStyle
    /// @return Reference to the current object
    GridView&
    setColumnStyle(const GridColumn& col, int styleId = -1, int headerStyleId =
        -1, int sumStyleId = -1);
    /// @brief Set style of a column
    /// @param colViewId Column by position in the view
    /// @param styleId Style to set for column cells. \n
    ///                If ==-1 set Then set style
    ///                GridView::defaultColumnStyle
    /// @param headerStyleId Style to set for column header cell. \n
    ///                      If ==-1 Then set style
    ///                      GridView::defaultColumnHeaderStyle
    /// @param sumStyleId Style to set for column sum cell. \n
    ///                   If ==-1 Then set style
    ///                   GridView::defaultColumnSumStyle
    /// @return Reference to the current object
    GridView&
    setColumnStyle(unsigned int colViewId, int styleId = -1, int headerStyleId =
        -1, int sumStyleId = -1);

    /* Row Style */
    /// @brief Set style of a row
    /// @param row Row by GridRow
    /// @param styleId Style to set for row cells. \n
    ///				   If ==-1 Then set style GridView::defaultRowStyle
    /// @return Reference to the current object
    GridView&
    setRowStyle(const GridRow& row, int styleId = -1);
    /// @brief Set style of a row
    /// @param rowViewId Row by position in the view
    /// @param styleId Style to set for row cells. \n
    ///				   If ==-1 Then set style GridView::defaultRowStyle
    /// @return Reference to the current object
    GridView&
    setRowStyle(unsigned int rowViewId, int styleId = -1);

    /* Filter Section */
    /// @brief Refresh all Filter (Log, Style Filter, Main Filter)
    /// @see GridFilter::refresh
    void
    refreshFilters();

    /// @brief Set filter value
    /// @param str Value of the filter
    void
    setFilter(const std::string& str);

    /// @brief Get filter string value
    /// @return Filter string value
    std::string
    getFilter() const;

    /// @brief Map a filter with a style
    ///
    /// Style-filter allow to change style of some row depend on their
    /// cells value.
    ///
    /// <h3>Example</h3>
    /// Set all pid > 100 to red
    /// @code
    ///		// A grid view
    ///		GridView view;
    ///
    ///		// Create style and set the style
    ///		view.setStyle(10, GridStyle().setColor(Color::RED));
    ///
    ///		// Add Style filter
    ///		view.addStyleFilter("PID > 100", 10);
    ///
    /// @endcode
    ///
    /// @param filter Filter to map
    /// @param styleId Style Id to associate with filter
    /// @return Reference to the current object
    GridView&
    addStyleFilter(const std::string& filter, int styleId);

    /// @brief Remove a style-filter
    /// @param id Id of style-filter to delete (start from 0)
    /// @return Reference to the current object
    /// @see GridView::addStyleFilter
    GridView&
    removeStyleFilter(unsigned int id);
    /// @brief Get style-filter count
    /// @return count of style filter
    unsigned int
    getStyleFilterCount() const;
    /// @brief Clear all style-filter
    /// @return Reference to the current object
    GridView&
    clearStyleFilter();

    /* Model - section */
    /// @brief Get a pointer to the GridModel of the view
    /// @return pointer to the connected model. \n
    ///			Or 0 if not connected to a model
    GridModel*
    getModel();

    /* Scroll View - Section */
    /// @brief Scroll the view to a row
    /// @param row Row by GridRow
    void
    scrollRowTo(const GridRow& row);
    /// @brief Scroll the view to a row
    /// @param rowViewId Row by position in the view
    void
    scrollRowTo(unsigned int rowViewId);
    /// @brief Scroll the view to a column
    /// @param col Column by GridColumn
    void
    scrollColumnTo(const GridColumn& col);
    /// @brief Scroll the view to a column
    /// @param colViewId Column by position in the view
    void
    scrollColumnTo(unsigned int colViewId);

    /// @brief Move the current view
    /// @param rowViewOffset Number of row to offset in the view
    /// @param columnViewOffset Number of column to offset in the view
    void
    scrollView(int rowViewOffset = 0, int columnViewOffset = 0);

    /// @brief Get the first visible row in the view
    /// @return The first visible row in the view
    GridRow&
    getFirstVisibleRow() const;
    /// @brief Get the first visible column in the view
    /// @return The first visible column in the view
    GridColumn&
    getFirstVisibleColumn() const;

    /// @brief Check if the row will be render on the view
    /// @param row Row by GridRow
    /// @return true if the row is render
    virtual bool
    isRowRendered(const GridRow& row);
    /// @brief Check if the column will be render on the view
    /// @param col Column by GridColumn
    /// @return true if the column is render
    virtual bool
    isColumnRendered(const GridColumn& col);

    /* Position/Size */
    /// @brief Set the render location
    /// @param x Left position
    /// @param y Top position
    /// @param w Render width. \n
    ///			 If 0 Then width not changed
    /// @param h Render height. \n
    ///			 If 0 Then height not changed
    /// @return Reference to the current object
    virtual GridView&
    move(int x, int y, int w = 0, int h = 0);
    /// @brief Set the render size
    /// @param w Render width. \n
    ///			 If 0 Then width not changed
    /// @param h Render height. \n
    ///			 If 0 Then height not changed
    /// @return Reference to the current object
    virtual GridView&
    size(int w = 0, int h = 0);
    /// @brief Get the X render location
    /// @return Left position of the view
    int
    getLeft() const;
    /// @brief Get the Y render location
    /// @return Top position of the view
    int
    getTop() const;
    /// @brief Get the height of the render
    /// @return Height size of the view
    int
    getHeight() const;
    /// @brief Get the width of the render
    /// @return Width size of the view
    int
    getWidth() const;
    /// @brief Set AutoSize for width/height calculated by the view
    ///		   container.
    /// @param autosize Structure representing the option of autoSize
    /// @see GridView::AutoSize
    void
    setAutoSize(const AutoSize& autosize);
    /// @brief Get AutoSize option
    /// @return const Reference to the AutoSize structure
    const AutoSize&
    getAutoSize() const;
    /// @brief Recalcul the size width/height of the view follow
    ///		   container and autoSize settings.
    virtual void
    refreshAutoSize();

    /* Render section */
    /// @brief Change render mode
    ///
    ///	If renderAllItem is set to true Then
    ///	<ul>
    ///		<li>All GridRow and GridColumn will be rendered.
    ///			Even if the view is not enough large.
    ///		</li>
    ///	</ul>
    ///	Else
    ///	<ul>
    ///		<li>Only GridRow and GridColumn who fit in the view
    ///			will be render (see GridView::scrollView)
    ///		</li>
    ///	</ul>
    ///
    /// @param renderAllItem Indicate Render Mode
    ///
    /// @return Reference to the current object
    GridView&
    setRenderMode(bool renderAllItem);
    /// @brief Check if render mode is render all item
    /// @return True if render mode is render all item
    bool
    isRenderAllItem() const;

    /* Render - method */
    /// @brief Update log and sort rows
    void
    update();
    /// @brief Change need Render flag to true
    /// @see GridView::needRender
    void
    forceRender();
    /// @brief Check if render needed
    ///
    /// If there is no change on the view or on the cells \n
    /// Then this function will return false.
    ///
    /// <h3>Example</h3>
    /// @code
    ///		// A Grid Model
    ///		GridModel model;
    ///
    ///		// A Grid View
    ///		GridView view;
    ///
    ///		// Infinite loop
    ///		while (1) {
    ///
    ///			// GridModel change ...
    ///
    ///			// Render only if it's needed
    ///			if (view.needRender()) {
    ///				view.render();
    ///			}
    ///
    ///		}
    ///
    /// @endcode
    ///
    /// @return True if render needed
    bool
    needRender() const;
    /// @brief Render the view.
    virtual void
    render();

    /// @brief Render a column header
    /// @param col ColumnView to render
    /// @param colId Count of column already rendered
    /// @param text Title of the column
    /// @param x Left position of the header to render
    /// @param y Top position of the header to render
    /// @param colSize Size of the header
    /// @param style Style of the header
    virtual void
    renderColumnHeader(const ColumnView& col, int colId,
        const std::string& text, int x, int y, int colSize,
        const GridStyle& style);

    /// @brief Render a row background
    /// @param row RowView to render
    /// @param rowId Count of row already rendered
    /// @param x Left position of the row to render
    /// @param y Top position of the row to render
    /// @param viewWidth Size of the view
    virtual void
    renderRow(const RowView& row, int rowId, int x, int y, int viewWidth);
    /// @brief Render a cell
    /// @param c GridCell to render
    /// @param rowId Count of row already rendered
    /// @param colId Count of column of this row already rendered
    /// @param text Text of the cell already formatted
    /// @param x Left position of the cell to render
    /// @param y Top position of the cell to render
    /// @param colSize Width of the column
    /// @param style Style of the cell
    virtual void
    renderCell(const GridCell& c, int rowId, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);
    /// @brief Render the sum bar
    /// @param x Left position of the sum bar to render
    /// @param y Top position of the sum bar to render
    /// @param viewWidth Size of the view
    virtual void
    renderSumBar(int x, int y, int viewWidth);
    /// @brief Render a sum cell
    /// @param col ColumnView to render
    /// @param colId Count of sum column already rendered
    /// @param text Text of the column sum cell
    /// @param x Left position of the sum cell to render
    /// @param y Top position of the sum cell to render
    /// @param colSize Width of the column
    /// @param style Style of the sum cell
    virtual void
    renderSumBarItem(const ColumnView& col, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /* Properties - Get */
    /// @brief Get additional header line
    /// @return true if an additional header line exist
    bool
    getAdditionalHeaderLine() const;
    /// @brief Get additional sum line
    /// @return true if an additional sum line exist
    bool
    getAdditionalSumLine(bool additionalLine);

    /* Properties - Set */
    /// @brief Set additional header line
    /// @param additionalLine Status of the additional line
    void
    setAdditionalHeaderLine(bool additionalLine);
    /// @brief Set additional sum line
    /// @param additionalLine Status of the additional line
    void
    setAdditionalSumLine(bool additionalLine);

    /* Friend class */
    friend class GridModel;
    friend class GridViewLoader;
    friend class GridViewLog;

  protected:

    /* Filter Protected - section */
    /// @brief Apply filter (Style-Filter, main-filter) on a RowView
    /// @param r Grid row view to test with filters
    /// @return true if main filter passed
    bool
    applyFilter(RowView* r);
    /// @brief Refresh a filter with the model of current GridView
    /// @param filter Filter to refresh
    void
    refreshAFilter(GridFilter& filter);

    /* Render method */
    /// @brief Function called before function render
    ///
    /// This function by default do nothing. \n
    /// To execute specific code before render override it.
    ///
    virtual void
    beforeRender();
    /// @brief Function called after function render
    ///
    /// This function by default do nothing. \n
    /// To execute specific code after render override it.
    ///
    /// @param viewWidth Width of the view
    /// @param viewHeight Height of the view
    virtual void
    afterRender(int viewWidth, int viewHeight);

    /// @brief Prepare the list of row/column to render
    void
    refreshRenderList();

    /// @brief Resize columns follow column size infos and current
    ///        view size
    ///
    /// This function can be overridden
    ///
    virtual void
    smartResizeColumns();

    /// @brief Format a text follow a style
    ///
    /// This function can be overridden.
    ///
    /// @param txt Text to format
    /// @param colSize Size of the column
    /// @param style Style to use
    /// @param retText Out string for formatted text
    virtual void
    getShowText(const std::string& txt, unsigned int colSize,
        const GridStyle& style, std::string& retText);

    /* Event function */
    /// @brief Event raised after the creation of a row (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param r RowView created
    /// @param afterRow Position of the new RowView. \n
    ///                 If (afterRow == 0) Then
    ///                      RowView added at the start \n
    ///                 Else RowView added after afterRow.
    virtual void
    onRowAdded(RowView& r, RowView* afterRow);
    /// @brief Event raised before the deletion of a row (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param r RowView deleted
    virtual void
    onRowDelete(RowView& r);
    /// @brief Event raised after the change of a row.
    ///		   Change can be visibility status or style. (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param r RowView changed
    virtual void
    onRowChange(RowView& r);
    /// @brief Event raised after the movement of a row (Event mode only)
    ///
    /// @copydetails GridView::onRowAdded
    ///
    /// @param r RowView moved
    /// @param afterRow New Position of the RowView. \n
    ///                 If (afterRow == 0) Then
    ///                      RowView moved at the start \n
    ///                 Else RowView moved after afterRow.
    virtual void
    onRowMoved(RowView& r, RowView* afterRow);
    /// @brief Event raised before the deletion of all the row
    ///        (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    virtual void
    onRowClear();

    /// @brief Event raised after the creation of a row (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param c ColumnView created
    /// @param afterColumn Position of the new ColumnView. \n
    ///                    If (afterColumn == 0) Then
    ///                        ColumnView added at the start \n
    ///                    Else ColumnView added after afterColumn.
    virtual void
    onColumnAdded(ColumnView& c, ColumnView* afterColumn);
    /// @brief Event raised before the deletion of a column
    ///        (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param c ColumnView deleted
    virtual void
    onColumnDelete(ColumnView& c);
    /// @brief Event raised after the change of a column.
    ///		   Change can be visibility status or style. (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param c ColumnView changed
    virtual void
    onColumnChange(ColumnView& c);
    /// @brief Event raised after the movement of a column
    ///        (Event mode only)
    ///
    /// @copydetails GridView::onColumnAdded
    ///
    /// @param c ColumnView moved
    /// @param afterColumn New Position of the ColumnView. \n
    ///                    If (afterColumn == 0) Then
    ///                        ColumnView moved at the start \n
    ///                    Else ColumnView moved after afterColumn.
    virtual void
    onColumnMoved(ColumnView& c, ColumnView* afterColumn);
    /// @brief Event raised before the deletion of all the column
    ///        (Event mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    virtual void
    onColumnClear();

    /// @brief Event raised after after the change of a cell value
    ///        (Event Mode only)
    ///
    /// This function do nothing by default. \n
    /// If you use event mode, example for a GUI GridView, you need
    /// to override it.
    ///
    /// @param r RowView associated to the cell
    /// @param c ColumnView associated to the cell
    /// @param newValue New Value set to the cell
    virtual void
    onCellChange(RowView& r, ColumnView& c, const Value& newValue);

    /* Protected - RowView/ColumnView method */
    /// @brief Get min/max row follow the cells values of a ref column
    /// @param start Begin search from start row
    /// @param col Ref GridColumn
    /// @param cmpOrder If < 0 Then search min row \n
    ///					Else search max row
    /// @return Iterator to the min/max row
    RowTable::iterator
    privSearchMinMaxRow(RowTable::iterator start, const GridColumn* col,
        int cmpOrder);

    /// @brief Change visibility of ColumnView
    /// @param c The column view
    /// @param visible Visibility status
    /// @return Reference to the current object
    GridView&
    privSetColumnVisibility(ColumnView* c, bool visible);
    /// @brief Move a ColumnView
    /// @param c The column view
    /// @param afterViewId If < 0 Then Move at begin of list \n
    ///	                   Else move after afterViewId position
    /// @return Reference to the current object
    GridView&
    privMoveColumn(ColumnView* c, int afterViewId);

    /// @brief Change visibility of RowView
    /// @param r The row view
    /// @param visible Visibility status
    /// @return Reference to the current object
    GridView&
    privSetRowVisibility(RowView* r, bool visible);
    /// @brief Move a RowView
    /// @param r The row view
    /// @param afterViewId If < 0 Then Move at begin of list \n
    ///	                   Else move after afterViewId position
    /// @return Reference to the current object
    GridView&
    privMoveRow(RowView* r, int afterViewId);

    /// @brief Enable/Disable Column sum of a ColumnView
    /// @param col The column view
    /// @param enable If == true Then enable sum of the column \n
    ///               Else disable sum of the column
    /// @return Reference to the current object
    GridView&
    privSetColumnSum(ColumnView* col, bool enable);

    /// @brief Set style of a ColumnView
    /// @param c The column view
    /// @param styleId Style to set for column cells. \n
    ///                If ==-1 Then set style
    ///                GridView::defaultColumnStyle
    /// @param headerStyleId Style to set for column header cell. \n
    ///                      If ==-1 Then set style
    ///                      GridView::defaultColumnHeaderStyle
    /// @param sumStyleId Style to set for column sum cell. \n
    ///                   If ==-1 Then set style
    ///                   GridView::defaultColumnSumStyle
    /// @return Reference to the current object
    GridView&
    privSetColumnStyle(ColumnView* c, int styleId = -1, int headerStyleId = -1,
        int sumStyleId = -1);
    /// @brief Set style of a RowView
    /// @param r The row view
    /// @param styleId Style to set for row cells. \n
    ///				   If ==-1 Then set style GridView::defaultRowStyle
    /// @return Reference to the current object
    GridView&
    privSetRowStyle(RowView* r, int styleId = -1);

  protected:

    /* Protected - Members */

    int left; ///< Left position of the view
    int top; ///< Top position of the view
    int _width; ///< Width of the view
    int _height; ///< Height of the view

    bool _sortEnabled; ///< Sort status
    const GridColumn* _sortRefColumn; ///< Sort ref GridColumn
    SortOrder _sortOrder; ///< Sort order Ascending or Descending

    bool _sortNeeded; ///< Indicate if sort need to be done before render

    ColumnMap columnsMap; ///< Columns map between view and model
    ColumnTable columns; ///< Ordered list of ColumnView
    FixedColumnTable fixedColumns; ///< List of Fixed ColumnView
    RenderList _renderColumns; ///< List of column to render

    RowMap rowsMap; ///< Rows map between view and model
    RowTable rows; ///< Ordered list of RowView
    FixedRowTable fixedRows; ///< Lost of Fixed RowView
    RenderList _renderRows; ///< List of row to render

    StyleMap stylesMap; ///< Map of styles
    StyleFilterMap filtersStyle; ///< Map of style-filter

    /// @brief Mode of the GridView
    ///
    /// <ul>
    ///		<li><b>(default) Render mode</b> -- Render function is used to
    ///         render item. </li>
    ///     <li><b>Event mode</b> -- Each time a change is done on view a
    ///         specific event is raised. \n
    ///         (This mode is provide for GUI where almost time no render
    ///           is needed just "object" state change)</li>
    ///	</ul>
    ///
    bool _eventMode;

    /// @brief Last Unique id set to a RowView or ColumnView
    unsigned int _lastUID;

    unsigned int _updateCounter; ///<  Update counter used to know change

    /// @brief Update counter value of the last render call
    unsigned int _lastRenderUpdateCounter;

    /// @brief Store the count of column sum to calcul
    unsigned int _sumColumnEnableTotal;

    RowView* _firstRowView; ///< Current scroll RowView position
    ColumnView* _firstColumnView; ///< Current scroll ColumnView position

    bool _renderAllItem; ///< Indicate if all item need to be render in one go

    GridFilter& _filter; ///< Filter of the view

    AutoSize _autoSize; ///< Autosize - Property

    bool _headerAdditionalLine; ///< Header additional line
    bool _sumAdditionalLine; ///< Sum additional line

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridView
///	@ingroup grid_view
///
/// By default this class is a fully functional view which simply output
/// all visible rows and columns in the std::cout ostream. \n \n
///
/// There are 2 mode of view:
/// <ul>
///		<li><b>(default) Render mode</b> -- Render function is used to 
///         render item. </li>
///     <li><b>Event mode</b> -- Each time a change is done on view a 
///         specific event is raised. \n 
///         (This mode is provide for GUI where almost time no render
///           is needed just "object" state change)</li>
///	</ul>
///
///////////////////////////////////////////////////////////////////////////////
