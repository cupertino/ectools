///////////////////////////////////////////////////////////////////////////////
/// @file		TermGridCtrl.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Terminal grid controller
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_TERMGRIDCONTROLLER_H__
#define LIBEC_TERMGRIDCONTROLLER_H__

#include "../GridCtrl.h"
#include "../../../tools/Console.h"

namespace cea
{

  /// @brief Terminal grid controller
  class TermGridCtrl : public GridCtrl
  {
  public:

    /* Constructor */
    /// @brief Default constructor
    TermGridCtrl();
    /// @brief Construct a terminal controller and link to a view
    /// @param grdView View to link to
    TermGridCtrl(GridView& grdView);

    /// @brief Interpret the terminal input
    ///
    /// <h3>Action possible</h3>
    ///	<ul>
    ///		<li><b>Sort</b> -- Default action.
    ///			Repeat of this action on a column will invert order of Sort
    ///			(ASCENDING or DESCENDING).</li>
    ///		<li><b>Sum</b> -- Activate or Desactivate the sum computation.
    ///			\n Invert status each time action raised.</li>
    ///		<li><b>Fix</b> -- Fix or unfix a column. \n
    ///			Invert status each time action raised. </li>
    ///	</ul>
    ///
    ///	<h3>Key interpreted</h3>
    ///	<ul>
    ///		<li><b>0</b> -- Disable sort</li>
    ///		<li><b>1</b> to <b>9</b> -- Action on column 1 to 9.
    ///			\n With 1 egal to the first column visible in screen.
    ///			\n Action change into Sort.</li>
    ///		<li><b>s</b> -- Change Action into Sum. </li>
    ///		<li><b>f</b> -- Change Action into Fix. </li>
    ///	</ul>
    ///
    /// @param key Key to interpret
    void
    handleTerminalEvent(int key);

    /// @brief Handle an event
    /// @param e Event to handle
    virtual void
    handleEvent(const GridCtrl::Event& e);

  protected:

    /* Members */
    /// Action id: 0 = Order, 1 = Sum, 2 = Fix
    /// @see handleTerminalEvent
    int mode;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::TermGridCtrl
///	@ingroup grid_ctrl
///
///	@copydetails cea::TermGridCtrl::handleTerminalEvent
///
///////////////////////////////////////////////////////////////////////////////
