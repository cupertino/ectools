#include <libec/grid/view/GridViewLoader.h>
#include <libec/grid/view/GridViewLog.h>
#include <libec/grid/view/GridView.h>
#include <libec/grid/model/GridModel.h>
#include <libec/grid/log/GridLog.h>
#include <libec/grid/filter/GridFilter.h>

namespace cea
{

  /** +loadXML */
  void
  GridViewLoader::loadXML(GridModel& model, XMLReader &xml,
      const XMLReader::Balise &balise)
  {
    const XMLReader::Balise* b;
    /* Get grid view */
    GridView& self = *(getGridViewLoaderRef());

    /* Load Position */
    b = balise.getChild("position");
    if (b != 0)
      {
        int w = b->getAttrValue("w", -1);
        int h = b->getAttrValue("h", -1);
        self.move(b->getAttrValue("x", 0), b->getAttrValue("y", 0), w, h);
        self.setAutoSize(
            GridView::AutoSize((w < 0), b->getAttrValue("offsetw", 0), (h < 0),
                b->getAttrValue("offseth", 0)));
      }
    else
      {
        self.move(0, 0, -1, -1);
        self.setAutoSize(GridView::AutoSize(true, true));
      }

    /* Load Style & Filter styles */
    for (unsigned int i = 0, max = balise.getChildCount(); i < max; i++)
      {
        b = balise.getChild(i);
        if (b == 0)
          continue;
        if (Tools::identicalString(b->name, "style"))
          {
            /// Style
            /* New Style */
            unsigned int id = b->getAttrValue("id", 0);
            GridStyle style;
            style.loadXML(xml, *b);
            /* Affect the style */
            self.setStyle(id, style);

          }
        else if ((Tools::identicalString(b->name, "style-filter"))
            || (Tools::identicalString(b->name, "filter-style")))
          {
            /// Style Filter
            /* New Filter Style */
            unsigned int id = b->getAttrValue("style", 0);
            std::string strFilter = b->getAttr("filter");
            /* Create the Style-Filter */
            self.addStyleFilter(strFilter, id);

          }
        else if (Tools::identicalString(b->name, "hide"))
          {
            /// Hide column
            /* Show all column */
            self.showAllColumn();
            /* Hide specific column */
            std::string str = b->getAttr("columns");
            unsigned int oldPos = 0;
            for (unsigned int i = 0, max = str.length(); i <= max; i++)
              {
                if ((i == max) || (str[i] == ','))
                  {
                    /* Get Column */
                    GridColumn& col = model.getGridColumnByName(
                        str.substr(oldPos, i - oldPos));
                    /* Hide column */
                    if (model.isValid(col))
                      {
                        self.hideColumn(col);
                      }
                    /* Advance */
                    oldPos = i + 1;
                  }
              }
          }
        else if (Tools::identicalString(b->name, "show"))
          {
            /// Show column & order
            /* Hide all column */
            self.hideAllColumn();
            /* Show specific column & order */
            std::string str = b->getAttr("columns");
            int colCnt = -1;
            unsigned int oldPos = 0;
            for (unsigned int i = 0, max = str.length(); i <= max; i++)
              {
                if ((i == max) || (str[i] == ','))
                  {
                    /* Get Column */
                    GridColumn& col = model.getGridColumnByName(
                        str.substr(oldPos, i - oldPos));
                    /* Show column & Move it */
                    if (model.isValid(col))
                      {
                        self.showColumn(col);
                        self.moveColumn(col, colCnt);
                      }
                    /* Advance */
                    oldPos = i + 1;
                    colCnt++;
                  }
              }
          }
        else if (Tools::identicalString(b->name, "sum"))
          {
            /// Sum column activation
            std::string str = b->getAttr("columns");
            unsigned int oldPos = 0;
            for (unsigned int i = 0, max = str.length(); i <= max; i++)
              {
                if ((i == max) || (str[i] == ','))
                  {
                    /* Get Column */
                    GridColumn& col = model.getGridColumnByName(
                        str.substr(oldPos, i - oldPos));
                    /* Show column & Move it */
                    if (model.isValid(col))
                      {
                        self.setColumnSum(col, true);
                      }
                    /* Advance */
                    oldPos = i + 1;
                  }
              }

          }

      }

    /* Filter */
    self.setFilter(balise.getBaliseAttr("filter", "value"));

    /* Sort */
    b = balise.getChild("sort");
    if (b != 0)
      {
        /* Get Column and order */
        GridColumn& col = model.getGridColumnByName(b->getAttr("by"));
        GridView::SortOrder order;
        std::string strOrder = b->getAttr("order");
        if ((Tools::identicalString(strOrder, "asc"))
            || (Tools::identicalString(strOrder, "ascending")))
          {
            order = GridView::ASCENDING;
          }
        else
          {
            order = GridView::DESCENDING;
          }
        /* Set sort */
        if (model.isValid(col))
          {
            self.sort(col, order);
          }
        else
          {
            self.disableSort();
          }
      }
    else
      {
        self.disableSort();
      }

    /* Option */
    b = balise.getChild("option");
    if (b != 0)
      {
        int tmp = b->getAttrValue("additional-header-line", 0);
        self._headerAdditionalLine = (tmp == 1);
        tmp = b->getAttrValue("additional-sum-line", 0);
        self._sumAdditionalLine = (tmp == 1);
      }

    /* Log */
    b = balise.getChild("log");
    if (b != 0)
      {
        /* Load log Type */
        BaseLog* log = LogCreator::create(b->getAttr("type", "file"));
        if (log != 0)
          {
            /* Get Log-type balise */
            const XMLReader::Balise* cfg = balise.getChild("log-type");
            if (cfg != 0)
              {
                log->loadXML(xml, *cfg);
              }
            /* Set log type */
            self.setLog(*log);
          }
        /* Load Log Format */
        BaseFormat* fmt = FormatCreator::create(b->getAttr("format", "xml"));
        if (fmt != 0)
          {
            /* Get Log-type balise */
            const XMLReader::Balise* cfg = balise.getChild("log-format");
            if (cfg != 0)
              {
                fmt->loadXML(xml, *cfg);
              }
            /* Set log format */
            self.setLog(*fmt);
          }
        /* Load GridLog option */
        self.getLog().setFlushFrequency(b->getAttrValue("flush-freq", "0"));
        self.getLog().setFlushItemCount(b->getAttrValue("flush-max-item", "0"));
        self.setLogFilter(b->getAttr("filter"));

        self.getLog().setBlockType(ITEM_DEFAULT,
            b->getAttr("default-block",
                self.getLog().getBlockType(ITEM_DEFAULT)));
        self.getLog().setBlockType(ITEM_ADDED,
            b->getAttr("added-block", self.getLog().getBlockType(ITEM_ADDED)));
        self.getLog().setBlockType(ITEM_ERASED,
            b->getAttr("erased-block",
                self.getLog().getBlockType(ITEM_ERASED)));
        self.getLog().setBlockType(ITEM_UPDATED,
            b->getAttr("updated-block",
                self.getLog().getBlockType(ITEM_UPDATED)));

        /* Activate log */
        self.activateLog(true);
      }
    else
      {
        self.activateLog(false);
      }

    /* Load specific XML */
    loadSpecificXML(model, xml, balise);

    /* Refresh filters */
    self.refreshFilters();
  }

  /** +loadSpecificXML */
  void
  GridViewLoader::loadSpecificXML(GridModel& model, XMLReader &xml,
      const XMLReader::Balise &balise)
  {
    /* DO NOTHING - By default */;
  }

}
