///////////////////////////////////////////////////////////////////////////////
/// @file		GridCtrl.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base class for grid controller
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GRIDCTRL_H__
#define LIBEC_GRIDCTRL_H__

#include "../view/GridView.h"

namespace cea
{

  /// @brief Base class for grid controller
  class GridCtrl
  {
  public:

    /// @ingroup grid_ctrl
    /// @brief Structure representing an event
    struct Event
    {
      /* Enumeration */
      /// @brief Type of the event
      enum Type
      {
        /// Terminal KeyHit \n
        /// Value of key = param
        TERM_KEYHIT,

        /// Graphical User Interface event - Key down \n
        /// Value of key = param \n
        /// <i>Reserved for future usage</i>
        GUI_KEYDOWN,

        /// Graphical User Interface event - Key up \n
        /// Value of key = param \n
        /// <i>Reserved for future usage</i>
        GUI_KEYUP,

        /// Graphical User Interface event - Mouse Button Down \n
        /// <i>Reserved for future usage</i>
        GUI_MOUSEDOWN,

        /// Graphical User Interface event - Mouse Button Up \n
        /// <i>Reserved for future usage</i>
        GUI_MOUSEUP,

        /// Graphical User Interface event - Mouse Move \n
        /// <i>Reserved for future usage</i>
        GUI_MOUSEMOVE
      };

      /* Public - Members */
      Type type; ///< @brief Type of event
      int param; ///< @brief Param1 of event
      /// @brief Extended parameters.
      /// Allow to extend easily event follow needs
      void* paramExt;

      /* Constructor */
      /// @brief Create a specific event
      /// @param type Type of the event
      Event(Type type);
      /// @brief Create a specific event with param
      /// @param type Type of the event
      /// @param param Main int param initial value of the event
      Event(Type type, int param);
      /// @brief Create a specific event with extend param
      /// @param type Type of the event
      /// @param paramExt Initial extended param value of the event
      Event(Type type, void* paramExt);
      /// @brief Create a specific with param and extend param
      /// @param type Type of the event
      /// @param param Main int param initial value of the event
      /// @param paramExt Initial extended param value of the event
      Event(Type type, int param, void* paramExt);

    };

    /* Constructor */
    /// @brief Default Constructor
    GridCtrl();
    /// @brief Construct a controller and link it with a View
    /// @param grdView View to link to
    GridCtrl(GridView& grdView);

    /// @brief Connect the controller with a view
    /// @param grdView View to link to
    void
    connectToView(GridView& grdView);

    /// @brief Handle an event
    ///
    ///	This function handle any event Terminal, GUI or other. \n
    /// By default this function do nothing. \n
    /// When a new controller is created this function need to be overridden
    /// in order to interpret event. \n \n
    ///
    /// For example TermGridCtrl override this function to interpret
    /// terminal event.
    ///
    /// @param e Event to handle
    virtual void
    handleEvent(const Event& e);

  protected:

    /* Members */
    GridView* view; ///< Reference to the grid view

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::GridCtrl
///	@ingroup grid_ctrl
///
///////////////////////////////////////////////////////////////////////////////
