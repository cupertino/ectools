#include <libec/monitor/config/MonitorLoader.h>
#include <libec/monitor/Monitor.h>

#include <libec/grid/view/GridView.h>
#include <libec/grid/view/terminal/TermGridView.h>
#include <libec/grid/view/html/HTMLGridView.h>
#include <libec/grid/view/html/HTMLChartGridView.h>

#include <libec/grid/controller/GridCtrl.h>
#include <libec/grid/controller/terminal/TermGridCtrl.h>

namespace cea
{

///////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
///////////////////////////////////////////////////////////////////////////////

  /** +constructor */
  MonitorLoader::MonitorLoader()
  {
    /* Map view "default","terminal","html","html-chart" */
    loadMapView<GridView>("");
    loadMapView<GridView>("default");
    loadMapView<TermGridView>("term");
    loadMapView<TermGridView>("terminal");
    loadMapView<HTMLGridView>("html");
    loadMapView<HTMLChartGridView>("chart-html");
    loadMapView<HTMLChartGridView>("html-chart");
    /* Map controller "default", "terminal" */
    loadMapController<GridCtrl>("");
    loadMapController<GridCtrl>("default");
    loadMapController<TermGridCtrl>("term");
    loadMapController<TermGridCtrl>("terminal");
  }

  /** +destructor */
  MonitorLoader::~MonitorLoader()
  {
#if 1
    /* Delete all loaded elements */
    for (LoadedList::iterator it = loadedElements.begin();
        it != loadedElements.end(); ++it)
      {
        delete (*it);
      }
    /* Delete all Loaders */
    for (LoaderMap::iterator it = monitorableLoaders.begin();
        it != monitorableLoaders.end(); ++it)
      {
        delete it->second;
      }
    for (LoaderMap::iterator it = viewLoaders.begin(); it != viewLoaders.end();
        ++it)
      {
        delete it->second;
      }
    for (LoaderMap::iterator it = ctrlLoaders.begin(); it != ctrlLoaders.end();
        ++it)
      {
        delete it->second;
      }
#endif
  }

///////////////////////////////////////////////////////////////////////////////
// Loadable function
///////////////////////////////////////////////////////////////////////////////

  /** #deleteAllCreatedElements */
  void
  MonitorLoader::deleteAllCreatedElements()
  {
#if 1
    Monitor& self = getMonitorRef();
    /* Deconnect all loaded Views */
    for (Monitor::ViewList::iterator v = self.loadedViews.begin();
        v != self.loadedViews.end(); ++v)
      {
        self.removeView(*(*v));
      }
    /* Delete all loaded elements */
    for (LoadedList::iterator it = loadedElements.begin();
        it != loadedElements.end(); ++it)
      {
        delete (*it);
      }
    loadedElements.clear();
    /* Views/Controllers */
    self.loadedControllers.clear();
    self.loadedViews.clear();
    /* Remove all loaded GridColumn */
    for (LoadedGridItemList::iterator it = loadedColumnLst.begin();
        it != loadedColumnLst.end(); ++it)
      {
        for (GridModel::ColumnList::iterator c = self.columns.begin();
            c != self.columns.end(); ++c)
          {
            if ((*c) == (*it))
              {
                self.removeColumn(*(*c));
                break;
              }
          }
      }
    loadedColumnLst.clear();
    /* Remove all loaded GridRow */
    for (LoadedGridItemList::iterator it = loadedRowLst.begin();
        it != loadedRowLst.end(); ++it)
      {
        for (GridModel::RowList::iterator r = self.rows.begin();
            r != self.rows.end(); ++r)
          {
            if ((*r) == (*it))
              {
                self.removeRow(*(*r));
                break;
              }
          }
      }
    loadedRowLst.clear();
#endif
  }

  /** #loadMonitorable */
  void*
  MonitorLoader::loadMonitorable(const std::string& className,
      const std::string& type)
  {
#if 1
    /* Search the loader */
    LoaderMap::iterator it = monitorableLoaders.find(
        Tools::toLower(className + "::" + type));
    /* Create the monitorable */
    if (it != monitorableLoaders.end())
      {
        BaseLoadable* elt = it->second->create();
        if (elt != 0)
          {
            /* Monitorable created */
            loadedElements.push_back(elt);
            return elt->getPtr();
          }
      }
    return 0;
#endif
  }

  /** #loadView */
  GridView*
  MonitorLoader::loadView(const std::string& type)
  {
#if 1
    /* Search the loader */
    LoaderMap::iterator it = viewLoaders.find(Tools::toLower(type));
    /* Create the view */
    if (it != viewLoaders.end())
      {
        BaseLoadable* elt = it->second->create();
        if (elt != 0)
          {
            /* View created */
            loadedElements.push_back(elt);
            return (GridView*) (elt->getPtr());
          }
      }
    return 0;
#endif
  }

  /** #loadController */
  GridCtrl*
  MonitorLoader::loadController(const std::string& type)
  {
#if 1
    /* Search the loader */
    LoaderMap::iterator it = ctrlLoaders.find(Tools::toLower(type));
    /* Create the controller */
    if (it != ctrlLoaders.end())
      {
        BaseLoadable* elt = it->second->create();
        if (elt != 0)
          {
            /* Controller created */
            loadedElements.push_back(elt);
            return (GridCtrl*) (elt->getPtr());
          }
      }

    return 0;
#endif
  }

///////////////////////////////////////////////////////////////////////////////
// Loader function
///////////////////////////////////////////////////////////////////////////////

  /** +loadFile */
  void
  MonitorLoader::loadFile(const std::string& file)
  {
#if 1
    /* Load file */
    xml.loadFile(file);
    /* Load xml data */
    loadXML(xml);
#endif
  }

  /** +loadString */
  void
  MonitorLoader::loadString(const std::string& str)
  {
#if 1
    /* Load file */
    xml.loadString(str);
    /* Load xml data */
    loadXML(xml);
#endif
  }

  /** +loadXML */
  void
  MonitorLoader::loadXML(XMLReader& xml)
  {
    // Get a Reference to the Monitor
    Monitor& self = getMonitorRef();
    // Clear all loaded data
    deleteAllCreatedElements();
    // Load XML Monitor
    const XMLReader::Balise* xmlMonitor = xml.getBalise("monitor");
    if (xmlMonitor != 0)
      {
        for (unsigned int i = 0, max = xmlMonitor->getChildCount(); i < max;
            i++)
          {
            // Get the Balise
            const XMLReader::Balise* b = xmlMonitor->getChild(i);
            if (b == 0)
              continue;
            // Interpret the Balise
            if (Tools::identicalString(b->name, "freq"))
              {
                // Frequency
                self.setFrequency(b->getAttrValue("value", 1000));
              }
            else if (Tools::identicalString(b->name, "filter"))
              {
                // Filter
                self.setFilter(b->getAttr("value"));
              }
            else if (Tools::identicalString(b->name, "column"))
              {
                // Column
                GridColumn* c = 0;
                /* Get all the property */
                Value::Type type = stringToValueType(b->getAttr("type", "int"));
                std::string name = b->getAttr("name");
                std::string title = b->getAttr("title", name);
                int fixedPos = b->getAttrValue("fixed", -1);
                std::string fixedMode = b->getAttr("fixed-mode");
                bool fixedAtEnd = Tools::identicalString(fixedMode, "end");
                int tag = b->getAttrValue("tag", -1);
                unsigned int pSize = b->getAttrValue("percent-size", 0);
                unsigned int mSize = b->getAttrValue("size", 0);
                mSize = b->getAttrValue("min-size", mSize);
                std::string cls = b->getAttr("class");
                std::string obj = b->getAttr("object");
                /* Add The Column */
                void* monitorable = 0;
                if (!(cls.empty()))
                  {
                    monitorable = loadMonitorable(cls, obj);
                  }
                if (monitorable != 0)
                  {
                    c = &(self.addMappedColumn(monitorable, title, type, tag,
                        mSize, pSize));
                  }
                else
                  {
                    c = &(self.addColumn(title, type, tag, mSize, pSize));
                  }
                if (c != 0)
                  {
                    loadedColumnLst.push_back(c);
                  }
                /* Set Fixed property */
                if ((c != 0) && (fixedPos != -1))
                  {
                    c->setFixed(fixedPos, fixedAtEnd);
                  }
                /* Set other property */
                if (c != 0)
                  {
                    self.setGridColumnName(*c, name);
                  }
                /* Set args property */
                unsigned int k = 0;
                if (c != 0)
                  {
                    for (unsigned int j = 0, max2 = b->getChildCount();
                        j < max2; j++)
                      {
                        const XMLReader::Balise* b2 = b->getChild(j);
                        if ((b2 != 0)
                            && (Tools::identicalString(b2->name, "arg")))
                          {
                            /* Load arg property */
                            unsigned int argId = b2->getAttrValue("id", k);
                            k++;
                            Value argValue(
                                stringToValueType(b2->getAttr("type", "int")));
                            argValue.set(b2->getAttr("value"));
                            /* Add the Args */
                            c->setArg(argId, argValue);
                          }
                      }
                  }
              }
            else if (Tools::identicalString(b->name, "row"))
              {
                // Row
                GridRow* r = 0;
                /* Get all the property */
                int tag = b->getAttrValue("tag", -1);
                int fixedPos = b->getAttrValue("fixed", -1);
                std::string fixedMode = b->getAttr("fixed-mode");
                bool fixedAtEnd = Tools::identicalString(fixedMode, "end");
                /* Add Row */
                r = &self.addRow(tag);
                if (r != 0)
                  {
                    loadedRowLst.push_back(r);
                  }
                /* Set Fixed property */
                if ((r != 0) && (fixedPos != -1))
                  {
                    r->setFixed(fixedPos, fixedAtEnd);
                  }
              }
          }
      }
    // Force refresh of all initial column value
    for (GridModel::RowList::iterator r = self.rows.begin();
        r != self.rows.end(); ++r)
      {
        self.onRowLoaded(*(*r));
      }
    // Force refresh of filter
    self._filter.refresh(self);
    // Load XML Views
    for (unsigned int i = 0, max = xml.getBaliseCount(); i < max; i++)
      {
        // Get the Balise
        const XMLReader::Balise* b = xml.getBalise(i);
        if (b == 0)
          continue;
        // Interpret the Balise
        if (Tools::identicalString(b->name, "view"))
          {
            /* Get all the Property */
            std::string type = b->getAttr("type");
            std::string ctrl = b->getAttr("controller",
                b->getAttr("ctrl", "term"));
            /* Create the View */
            GridView* v = loadView(type);
            if (v != 0)
              {
                /* Add the view in monitor */
                self.connectLoadedView(v);
                /* Create and connect the controller */
                GridCtrl* c = loadController(ctrl);
                if (c != 0)
                  {
                    self.connectLoadedController(c, *v);
                  }
                /* Load View */
                v->loadXML(self, xml, *b);
              }
          }
      }
  }

  /** +stringToValueType */
  Value::Type
  MonitorLoader::stringToValueType(const std::string& type)
  {
    /* Search Value::Type */
    if (Tools::identicalString(type, "bool"))
      {
        return Value::BOOL;
      }
    else if (Tools::identicalString(type, "int"))
      {
        return Value::INT;
      }
    else if (Tools::identicalString(type, "uint"))
      {
        return Value::UINT;
      }
    else if ((Tools::identicalString(type, "double"))
        || (Tools::identicalString(type, "float")))
      {
        return Value::DOUBLE;
      }
    else if (Tools::identicalString(type, "string"))
      {
        return Value::STRING;
      }
    else
      {
        return Value::INT;
      }
  }

}
