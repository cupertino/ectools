#include <libec/grid/filter/GridFilter.h>

namespace cea
{

  /* ----------------------------------------------------------------------- */
  /* Destructor */

  /** +destructor */
  GridFilter::~GridFilter()
  {
    /* Delete all Element from memory */
    for (FilterList::iterator f = filters.begin(); f != filters.end(); ++f)
      {
        for (FilterElementList::iterator e = (*f)->begin(); e != (*f)->end();
            ++e)
          {
            for (ValueList::iterator v = (*e)->params.begin();
                v != (*e)->params.end(); ++v)
              {
                delete (*v);
              }
            delete (*e);
          }
        delete (*f);
      }
  }

  /* ----------------------------------------------------------------------- */
  /* Apply Filters */

  /** +applyFilter */
  bool
  GridFilter::applyFilter(GridRow& row)
  {
    bool bElementOk = false;
    bool bValueOk = false;
    // Check for all Elements
    for (FilterList::iterator f = filters.begin(); f != filters.end(); ++f)
      {
        /* OR Combinaison */
        bElementOk = true;
        for (FilterElementList::iterator e = (*f)->begin(); e != (*f)->end();
            ++e)
          {
            /* AND Combinaison */
            /* Get Column */
            if ((*e)->col == 0)
              {
                bElementOk = false;
                continue;
              }
            GridColumn& c = *((*e)->col);
            OPERATOR op = (*e)->op;
            /* Or Combinaison one of the value */
            bValueOk = false;
            for (ValueList::iterator v = (*e)->params.begin();
                v != (*e)->params.end(); ++v)
              {
                /* Or combinaison */
                // Get Compare Value
                int cmp = c.getCell(row).get().compare((*v)->value);
                // Intrepret compare value
                if (((op == EGAL) && (cmp == 0)) || ((op == DIFF) && (cmp != 0))
                    || ((op == SUP) && (cmp > 0)) || ((op == INF) && (cmp < 0))
                    || ((op == SUPEGAL) && (cmp >= 0))
                    || ((op == INFEGAL) && (cmp <= 0)))
                  {
                    // Condition passed
                    bValueOk = true;
                    break;
                  }
              }
            /* Interpret the value (AND Combinaison) */
            /* so if no value corresponding go next to other filter */
            if (!bValueOk)
              {
                bElementOk = false;
                break;
              }
          }
        /* If one Element passed then return true */
        if (bElementOk)
          {
            return true;
          }
      }
    // If no filter have passed return false
    return (filters.size() == 0);
  }

  /** +applyFilter */
  bool
  GridFilter::applyFilter(GridView::RowView& row)
  {
    if (row.ref == 0)
      {
        return true;
      }
    else
      {
        return applyFilter(*(row.ref));
      }
  }

  /* ----------------------------------------------------------------------- */
  /* Refresh Filters */

  /** +refresh */
  void
  GridFilter::refresh(GridModel& model)
  {
    // Refresh all Filter values
    for (FilterList::iterator f = filters.begin(); f != filters.end(); ++f)
      {
        for (FilterElementList::iterator e = (*f)->begin(); e != (*f)->end();
            ++e)
          {
            /* Retrieve the column by name or id */
            Value::Type type = Value::UNKNOWN;
            if ((*e)->colId == -1)
              {
                GridColumn& c = model.getGridColumnByName((*e)->colName);
                if (model.isValid(c))
                  {
                    (*e)->col = &c;
                    type = c.getType();
                  }
                else
                  {
                    (*e)->col = 0;
                  }
              }
            else
              {
                GridColumn& c = model.getColumn((*e)->colId);
                if (model.isValid(c))
                  {
                    (*e)->col = &c;
                    type = c.getType();
                  }
                else
                  {
                    (*e)->col = 0;
                  }
              }
            /* Init all values with good type and strValue(loaded value) */
            if (type != Value::UNKNOWN)
              {
                for (ValueList::iterator v = (*e)->params.begin();
                    v != (*e)->params.end(); ++v)
                  {
                    /* Update all values */
                    (*v)->value.setType(type);
                    (*v)->value.set((*v)->strValue);
                  }
              }
          }
      }
  }

  /** +clearAllFilter */
  void
  GridFilter::clearAllFilter()
  {
    /* Delete all Element from memory */
    for (FilterList::iterator f = filters.begin(); f != filters.end(); ++f)
      {
        for (FilterElementList::iterator e = (*f)->begin(); e != (*f)->end();
            ++e)
          {
            for (ValueList::iterator v = (*e)->params.begin();
                v != (*e)->params.end(); ++v)
              {
                delete (*v);
              }
            (*e)->params.clear();
            delete (*e);
          }
        (*f)->clear();
        delete (*f);
      }
    /* Clear all lists */
    filters.clear();
  }

  /* ----------------------------------------------------------------------- */
  /* Get/Set Filters */

  /** +get */
  std::string
  GridFilter::get() const
  {
    return _filterCode;
  }

  /** +set */
  bool
  GridFilter::set(std::string code)
  {
    FilterElement* fe = NULL;
    FilterValue* v;
    FilterElementList* f = 0;
    code.append(" ");
    unsigned char strChar = '\'';
    unsigned int oldPos = 0;
    int mode = 0; /* 0 = Search Start Name, 1 = Search End Name, */
    /* 2 = Search Number after '$', */
    /* 3 = Search Op Start, 4 = Search Op End */
    /* 5 = Search Start value, 6 = Search End value, */
    /* 7 = Search Value String */
    /* Clear All Operation */
    clearAllFilter();
    /* Set the Filter string code member */
    _filterCode = code;
    /* Find the filters */
    for (unsigned int i = 0, max = code.length(); i <= max; i++)
      {
        /* Get char */
        unsigned char c = ((i == max) ? ' ' : code[i]);
        /* Interpret char */
        if (mode < 2)
          {
            /// Search Name
            if (c == '$')
              {
                // Search Number after '$'
                mode = 2;
                oldPos = i + 1;
                // Create FilterElement
                if (f == 0)
                  {
                    f = new FilterElementList;
                    if (f == 0)
                      return false;
                  }
                fe = new FilterElement;
                if (fe == 0)
                  {
                    delete f;
                    return false;
                  }
                f->push_back(fe);
                fe->col = 0;
                fe->colId = -1;
              }
            else if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))
                || ((c >= '0') && (c <= '9')) || (c == '_') || (c == ':'))
              {
                if (mode == 0)
                  {
                    // Start name
                    mode = 1;
                    oldPos = i;
                    // Create FilterElement
                    if (f == 0)
                      {
                        f = new FilterElementList;
                        if (f == 0)
                          return false;
                      }
                    fe = new FilterElement;
                    if (fe == 0)
                      {
                        delete f;
                        return false;
                      }
                    f->push_back(fe);
                    fe->col = 0;
                    fe->colId = -1;
                  }

              }
            else
              {

                if (mode == 1)
                  {
                    // Name Found search operator
                    mode = 3;
                    fe->colName.assign(code, oldPos, i - oldPos);
                    fe->colId = -1;
                  }

              }
          }
        else if (mode == 2)
          {
            /// Search number after '$'
            if ((c >= '0') && (c <= '9'))
              {
                // wait other char
              }
            else
              {
                /// Search opeartor start
                mode = 3;
                Tools::CStrTo(code.substr(oldPos, i - oldPos), fe->colId);
              }
          }
        else if (mode < 5)
          {
            /// Search operator
            switch (c)
              {
            case '=':
            case '<':
            case '>':
            case '!':
              /// Search Operator Start
              if (mode == 3)
                {
                  // Search operator End
                  mode = 4;
                  oldPos = i;
                }
              break;
            default:
              /// Search Operator End
              if (mode == 4)
                {
                  // Convert string to OPERATOR
                  std::string tmp(code, oldPos, i - oldPos);
                  if (tmp.compare("<") == 0)
                    {
                      fe->op = INF;
                    }
                  else if (tmp.compare(">") == 0)
                    {
                      fe->op = SUP;
                    }
                  else if ((tmp.compare("<=") == 0) || (tmp.compare("=<") == 0))
                    {
                      fe->op = INFEGAL;
                    }
                  else if ((tmp.compare(">=") == 0) || (tmp.compare("=>") == 0))
                    {
                      fe->op = SUPEGAL;
                    }
                  else if ((tmp.compare("!=") == 0) || (tmp.compare("<>") == 0))
                    {
                      fe->op = DIFF;
                    }
                  else
                    {
                      fe->op = EGAL;
                    }
                  // Search Start Value
                  mode = 5;
                  oldPos = i;
                }
              break;
              }
          }
        else if (mode < 7)
          {
            /// Search Value
            if ((mode == 5) && ((c == '\'') || (c == '\"')))
              {
                // Search End Value with string
                mode = 7;
                oldPos = i + 1;
                strChar = c;

              }
            else if ((c == '&') || ((mode == 5) && (c == '.')))
              {
                // Add Value
                if (c == '&')
                  {
                    v = new FilterValue;
                    if (v != 0)
                      {
                        v->strValue.assign(code, oldPos, i - oldPos);
                        fe->params.push_back(v);
                      }
                  }
                // Change mode to 0
                mode = 0;
                oldPos = i + 1;

              }
            else if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))
                || ((c >= '0') && (c <= '9')) || (c == '_') || (c == ':')
                || (c == '.') || (c == '/') || (c == '\\'))
              {
                if (mode == 5)
                  {
                    // Start Value
                    mode = 6;
                    oldPos = i;
                  }

              }
            else if ((i == max) || (c == '|') || (c == ';'))
              {
                // New Filter Elements
                mode = 0;
                filters.push_back(f);
                f = 0;
              }
            else
              {
                if (mode == 6)
                  {
                    // Value Found search new value
                    mode = 5;
                    // Add Value
                    v = new FilterValue;
                    if (v != 0)
                      {
                        v->strValue.assign(code, oldPos, i - oldPos);
                        fe->params.push_back(v);
                      }
                  }
              }

          }
        else if (mode == 7)
          {
            /// Search Value String
            if ((c == strChar) || (i == max))
              {
                // Add Value
                v = new FilterValue;
                if (v != 0)
                  {
                    v->strValue.assign(code, oldPos, i - oldPos);
                    fe->params.push_back(v);
                  }
                // Goback to mode search value
                mode = 5;
                oldPos = i + 1;
              }
          }

      }

    return true;
  }

}
