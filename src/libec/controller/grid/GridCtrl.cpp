#include <libec/grid/controller/GridCtrl.h>

namespace cea
{

///////////////////////////////////////////////////////////////////////////////
/// Event Section
///////////////////////////////////////////////////////////////////////////////

  /** +constructor */
  GridCtrl::Event::Event(Type type) :
      type(type), param(0), paramExt(0)
  {
    ;
  }
  /** +constructor */
  GridCtrl::Event::Event(Type type, int param) :
      type(type), param(param), paramExt(0)
  {
    ;
  }
  /** +constructor */
  GridCtrl::Event::Event(Type type, void* paramExt) :
      type(type), param(0), paramExt(paramExt)
  {
    ;
  }
  /** +constructor */
  GridCtrl::Event::Event(Type type, int param, void* paramExt) :
      type(type), param(param), paramExt(paramExt)
  {
    ;
  }

///////////////////////////////////////////////////////////////////////////////
/// GridCtrl Section
///////////////////////////////////////////////////////////////////////////////

  /** +Constructor */
  GridCtrl::GridCtrl() :
      view(0)
  {
    ;
  }

  /** +Constructor */
  GridCtrl::GridCtrl(GridView& grdView) :
      view(&grdView)
  {
    ;
  }

  /** +connectToView */
  void
  GridCtrl::connectToView(GridView& grdView)
  {
    view = &grdView;
  }

  /** +handleEvent */
  void
  GridCtrl::handleEvent(const GridCtrl::Event& e)
  {
    /* DO NOTHING - By default */;
  }

}
