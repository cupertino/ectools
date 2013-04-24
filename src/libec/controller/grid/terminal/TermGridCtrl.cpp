#include <libec/grid/controller/terminal/TermGridCtrl.h>

namespace cea
{

  /** +Constructor */
  TermGridCtrl::TermGridCtrl() :
      GridCtrl(), mode(0)
  {
    ;
  }
  TermGridCtrl::TermGridCtrl(GridView& grdView) :
      GridCtrl(grdView), mode(0)
  {
    ;
  }

  /* -------------------------------------------------------------------------- */
  /* Event */

  /** Interpret the terminal input */
  void
  TermGridCtrl::handleTerminalEvent(int key)
  {
    if (view == 0)
      return;
    switch (key)
      {
    case '0':
      view->disableSort();
      break;
    case Console::K_UP:
      view->scrollView(-1);
      break;
    case Console::K_DOWN:
      view->scrollView(1);
      break;
    case Console::K_LEFT:
      view->scrollView(0, -1);
      Console::clear(view->getLeft(), view->getTop(), view->getWidth(),
          view->getHeight());
      break;
    case Console::K_RIGHT:
      view->scrollView(0, 1);
      Console::clear(view->getLeft(), view->getTop(), view->getWidth(),
          view->getHeight());
      break;
    case '+':
      view->scrollView(view->getHeight());
      break;
    case '-':
      view->scrollView(-view->getHeight());
      break;
    case '=':
    case 's':
    case 'S': /* For sum activate/desactivate */
      mode = 1;
      break;
    case 'f':
    case 'F': /* For fix/unfix column */
      mode = 2;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      /* Get the column - advance from _firstColumnView */
      GridView::ColumnView* c = view->getColumnView(key - '1', true);
      if (c != 0)
        {
          switch (mode)
            {
          case 1:
            /* Then activate/desactivate sum */
            if (c->sumEnable)
              {
                view->setColumnSum(*(c->ref), false);
              }
            else
              {
                view->setColumnSum(*(c->ref), true);
              }
            mode = 0;
            break;
          case 2:
            /* Then activate/desactivate fixed */
            if (c->ref != 0)
              {
                if (c->fixed)
                  {
                    view->unfixColumn(*(c->ref));
                  }
                else
                  {
                    view->fixColumn(*(c->ref), false);
                  }
              }
            mode = 0;
          default:
            /* Sort column option */
            if ((view->isSortEnabled())
                && (c->ref == &(view->getSortRefColumn())))
              {
                view->sort(*(c->ref),
                    ((view->getSortOrder() == GridView::ASCENDING) ?
                        GridView::DESCENDING : GridView::ASCENDING));
              }
            else
              {
                view->sort(*(c->ref), view->getSortOrder());
              }
            }
        }
      break;
      }
  }

  /** +handleEvent */
  void
  TermGridCtrl::handleEvent(const GridCtrl::Event& e)
  {
    /* Handle only Terminal Key Hit event */
    if (e.type == GridCtrl::Event::TERM_KEYHIT)
      {
        handleTerminalEvent(e.param);
      }
  }

}
