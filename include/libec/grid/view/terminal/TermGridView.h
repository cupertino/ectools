///////////////////////////////////////////////////////////////////////////////
/// @file		TermGridView.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Terminal GridModel view
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_TERMGRIDVIEW_H__
#define LIBEC_TERMGRIDVIEW_H__

#include "../GridView.h"

#include "../../../tools/Console.h"

namespace cea
{

  /// @brief Terminal GridModel view
  class TermGridView : public GridView
  {
  public:

    /* Constructor */
    /// @brief Default constructor
    TermGridView();
    /// @brief Create a terminal view of a GridModel
    /// @param model GridModel to connect to
    TermGridView(GridModel& model);

    /* Size */
    /// @brief Refresh size with Console Size
    /// @see GridView::refreshAutoSize
    virtual void
    refreshAutoSize();

    /** Render Method */
    /// @copydoc GridView::renderColumnHeader
    virtual void
    renderColumnHeader(const ColumnView& col, int colId,
        const std::string& text, int x, int y, int colSize,
        const GridStyle& style);

    /// @copydoc GridView::renderRow
    virtual void
    renderRow(const RowView& row, int rowId, int x, int y, int viewWidth);
    /// @copydoc GridView::renderCell
    virtual void
    renderCell(const GridCell& c, int rowId, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @copydoc GridView::renderSumBar
    virtual void
    renderSumBar(int x, int y, int viewWidth);
    /// @copydoc GridView::renderSumBarItem
    virtual void
    renderSumBarItem(const ColumnView& col, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    /// @brief Function called after function render
    ///
    /// Clear empty space
    ///
    /// @param viewWidth Width of the view
    /// @param viewHeight Height of the view
    virtual void
    afterRender(int viewWidth, int viewHeight);

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::TermGridView
///	@ingroup grid_view
///
///////////////////////////////////////////////////////////////////////////////
