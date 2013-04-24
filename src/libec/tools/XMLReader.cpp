#include <libec/tools/XMLReader.h>

namespace cea
{

  /* ------------------------------------------------------------------------- */
  /* Balise struct Function */

  /** +Constructor */
  XMLReader::Balise::Balise() :
      parent(0), startPos(0), endPos(0), innerStartPos(0), innerEndPos(0)
  {
    ;
  }

  /** +Destructor */
  XMLReader::Balise::~Balise()
  {
    /* Delete from memory all attrs */
    for (AttrList::iterator it = attrs.begin(); it != attrs.end(); ++it)
      {
        delete (*it);
      }
  }

  /** +getChildCount */
  unsigned int
  XMLReader::Balise::getChildCount() const
  {
    return childs.size();
  }

  /** +getChild */
  const XMLReader::Balise*
  XMLReader::Balise::getChild(unsigned int id) const
  {
    /* Search the balise */
    for (BaliseList::const_iterator it = childs.begin(); it != childs.end();
        ++it)
      {
        if (id == 0)
          {
            return *it;
          }
        else
          {
            id--;
          }
      }
    /* If not found return a null pointer */
    return 0;
  }

  /** +getChild */
  const XMLReader::Balise*
  XMLReader::Balise::getChild(const std::string& path) const
  {
    const Balise* b = this;
    unsigned int oldPos = 0;
    /* Search the balise */
    for (unsigned int i = 0, max = path.length(); i <= max; i++)
      {
        /* Get char */
        unsigned char c = ((i == max) ? '/' : path[i]);
        /* Interpret the char */
        switch (c)
          {
        case '/':
        case '\\':
        case '|':
        case ';':
          /* Search the balise */
          BaliseMap::const_iterator it = b->childsMap.find(
              path.substr(oldPos, i - oldPos));
          if (it == b->childsMap.end())
            {
              /* not found return null pointer */
              return 0;
            }
          /* Continue the search */
          b = it->second;
          oldPos = i + 1;
          break;
          }
      }
    /* If balise found return the balise */
    if (b == this)
      {
        return 0;
      }
    else
      {
        return b;
      }
  }

  /** +clearAttr */
  void
  XMLReader::Balise::clearAttr()
  {
    /* Delete from memory all attrs */
    for (AttrList::iterator it = attrs.begin(); it != attrs.end(); ++it)
      {
        delete (*it);
      }
    /* Clear all attrs list */
    attrs.clear();
    attrsMap.clear();
  }

  /** +getAttr */
  std::string
  XMLReader::Balise::getAttr(const std::string& name,
      const std::string defaultValue) const
  {
    /* Search the attr with no index */
    AttrMap::const_iterator it = attrsMap.find(name);
    if (it != attrsMap.end())
      {
        return it->second->value;
      }
    /* If no attr return DefaultValue */
    return defaultValue;
  }

  /** +getAttrValue */
  Value
  XMLReader::Balise::getAttrValue(const std::string& name,
      const Value& defaultValue) const
  {
    /* Search the attr with no index */
    AttrMap::const_iterator it = attrsMap.find(name);
    if (it != attrsMap.end())
      {
        return it->second->value;
      }
    /* If no attr return DefaultValue */
    return defaultValue;
  }

  /** +getBaliseAttr */
  std::string
  XMLReader::Balise::getBaliseAttr(const std::string& path,
      const std::string& name, const std::string defaultValue) const
  {
    /* Search the balise */
    const Balise* b = getChild(path);
    if (b == 0)
      {
        return defaultValue;
      }
    else
      {
        return b->getAttr(name, defaultValue);
      }
  }

  /** +getBaliseAttrValue */
  Value
  XMLReader::Balise::getBaliseAttrValue(const std::string& path,
      const std::string& name, const Value& defaultValue) const
  {
    /* Search the balise */
    const Balise* b = getChild(path);
    if (b == 0)
      {
        return defaultValue;
      }
    else
      {
        return b->getAttrValue(name, defaultValue);
      }
  }

  /** +addAttr */
  void
  XMLReader::Balise::addAttr(const std::string& name, const std::string& value)
  {
    /* Create the attr */
    Attr* attr = new Attr;
    /* Add into the list of attr */
    if (attr != 0)
      {
        attr->name = name;
        attr->value = value;
        attrs.push_back(attr);
        attrsMap.insert(AttrMap::value_type(name, attr));
      }
  }

  /* ------------------------------------------------------------------------- */
  /* Destructor */

  /** +destructor */
  XMLReader::~XMLReader()
  {
    for (BaliseList::iterator it = doc.begin(); it != doc.end(); ++it)
      {
        delete (*it);
      }
  }

  /* ------------------------------------------------------------------------- */
  /* General section */

  /** +clear */
  void
  XMLReader::clear()
  {
    for (BaliseList::iterator it = doc.begin(); it != doc.end(); ++it)
      {
        delete (*it);
      }
    doc.clear();
    root.childs.clear();
    root.childsMap.clear();
    root.clearAttr();
    root.name.clear();
    root.text.clear();
    root.startPos = 0;
    root.endPos = 0;
    root.innerStartPos = 0;
    root.innerEndPos = 0;
  }

  /* ------------------------------------------------------------------------- */
  /* Load section */

  /** +loadFile */
  unsigned int
  XMLReader::loadFile(const std::string& file)
  {
    clear();
    std::ifstream f(file.c_str());
    if (f.is_open())
      {
        /* Get the content of file */
        std::stringstream buf;
        buf << f.rdbuf();
        unsigned int cnt = loadString(buf.str(), 0, buf.str().length());
        f.close();
        return cnt;
      }
    else
      {
        return 0;
      }
  }

  /** +loadString */
  unsigned int
  XMLReader::loadString(const std::string& str)
  {
    clear();
    return loadString(str, 0, str.length());
  }

  /** +loadString */
  unsigned int
  XMLReader::loadString(const std::string& str, unsigned int start,
      unsigned int len)
  {
    /* Clear all data */
    clear();
    /* Load All balise */
    if ((len == 0) || (start + len > str.length()))
      {
        len = str.length() - start;
      }
    loadBalise(str, start, start + len);
    /* Return the balise count */
    return doc.size();
  }

  /* ------------------------------------------------------------------------- */
  /* Balise Function */

  /** +getBaliseCount */
  unsigned int
  XMLReader::getBaliseCount() const
  {
    return doc.size();
  }

  /** +getBalise */
  const XMLReader::Balise*
  XMLReader::getBalise(unsigned int id) const
  {
    /* Search the balise */
    for (BaliseList::const_iterator it = doc.begin(); it != doc.end(); ++it)
      {
        if (id == 0)
          {
            return *it;
          }
        else
          {
            id--;
          }
      }
    /* If not found return a null pointer */
    return 0;
  }

  /** +getBalise */
  const XMLReader::Balise*
  XMLReader::getBalise(const std::string& path) const
  {
    const Balise* b = root.getChild(path);
    if (b == 0)
      {
        return &root;
      }
    else
      {
        return b;
      }
  }

  /** +getText */
  std::string
  XMLReader::getText(const std::string& path, const std::string& defaultValue)
  {
    /* Search the balise */
    const Balise* b = getBalise(path);
    if (b != 0)
      {
        return b->text;
      }
    else
      {
        return defaultValue;
      }
  }

  /** +getTextValue */
  Value
  XMLReader::getTextValue(const std::string& path, const Value& defaultValue)
  {
    /* Search the balise */
    const Balise* b = getBalise(path);
    if (b != 0)
      {
        return b->text;
      }
    else
      {
        return defaultValue;
      }
  }

  /** +getAttr */
  std::string
  XMLReader::getAttr(const std::string& balisePath, const std::string& attrName,
      const std::string& defaultValue)
  {
    /* Search the balise */
    const Balise* b = getBalise(balisePath);
    if (b != 0)
      {
        return b->getAttr(attrName, defaultValue);
      }
    else
      {
        return defaultValue;
      }
  }

  /** +getAttrValue */
  Value
  XMLReader::getAttrValue(const std::string& balisePath,
      const std::string& attrName, const Value& defaultValue)
  {
    /* Search the balise */
    const Balise* b = getBalise(balisePath);
    if (b != 0)
      {
        return b->getAttrValue(attrName, defaultValue);
      }
    else
      {
        return defaultValue;
      }
  }

//  template<typename T>
//    void
//    XMLReader::readSingleValuedTag(const char* tag, T& to)
//    {
//      std::string tagStr(tag);
//      unsigned begin, end;
//
//      begin = tagStr.find("<" + tagStr + " value=\"");
//      begin += tagStr.length() + 9;
//      end = tagStr.substr(begin).find("\"");
//
//      if ((begin < end) && (begin != std::string::npos)
//          && (end != std::string::npos))
//        {
//          std::stringstream oss;
//          oss << tagStr.substr(begin, end);
//          oss >> to;
//        }
//      else
//        to = NULL;
//    }

  /* ------------------------------------------------------------------------- */
  /* Private Function section */

  /** -loadBalise */
  void
  XMLReader::loadBalise(const std::string& str, unsigned int start,
      unsigned int max)
  {
    unsigned char valueProtectedChar = '\'';
    unsigned int oldPos = start, startPos = start;
    std::list<Balise*> parents;
    Balise* current;
    /* 0=Search Balise Start <, 1=Search Balise Name, */
    /* 2=Search '>', 3=Search protected char, 4=Mode comment, */
    /* 5=Mode CDATA, 6=Mode find '>' for end balise */
    int mode = 0;

    /* Add the root balise */
    parents.push_back(&root);

    /* Read the balise */
    for (unsigned int i = start; i <= max; i++)
      {
        /* Get Char */
        unsigned char c = ((i == max) ? '\n' : str[i]);
        /* Interpret the char */
        if (mode == 0)
          {
            /* Search balise start */
            switch (c)
              {
            case '<':
              /* Add to the parent the value */
              if (parents.size() > 0)
                {
                  parents.back()->text.append(str, oldPos, i - oldPos);
                }
              mode = 1;
              oldPos = i + 1;
              startPos = i;
              break;
              }
          }
        else if (mode == 1)
          {
            /* Search balise name */
            switch (c)
              {
            case '-':
              if ((i == oldPos + 2) && (str[i - 1] == '-')
                  && (str[i - 2] == '!') && (str[i - 3] == '<'))
                {
                  /* Get a comment */
                  mode = 4;
                  oldPos = i + 1;
                }
              break;
            case '[':
              if ((i == oldPos + 7) && (str[i - 1] == 'A')
                  && (str[i - 2] == 'T') && (str[i - 3] == 'A')
                  && (str[i - 4] == 'D') && (str[i - 5] == 'C')
                  && (str[i - 6] == '[') && (str[i - 7] == '!')
                  && (str[i - 8] == '<'))
                {
                  /* Get a <![CDATA[ */
                  mode = 5;
                  oldPos = i + 1;
                }
              break;
            case '/':
              /* Check if end balise */
              if (oldPos == i)
                {
                  /* Set current innercode infos */
                  if (parents.size() > 0)
                    {
                      parents.back()->innerEndPos = i;
                    }
                  /* Goto mode: Wait '>' */
                  mode = 6;
                }
              break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
            case '>':
              /* Create the balise */
              current = new Balise();
              if (current == 0)
                {
                  /* Mode no balise wait '>' */
                  mode = 2;
                }
              else
                {
                  /* Add to BaliseList */
                  doc.push_back(current);
                  /* Set default value */
                  current->startPos = startPos;
                  /* Set Name */
                  current->name.assign(str, oldPos, i - oldPos);
                  /* Add to parent */
                  if (parents.size() > 0)
                    {
                      current->parent = parents.back();
                      parents.back()->childs.push_back(current);
                      parents.back()->childsMap.insert(
                          BaliseMap::value_type(current->name, current));
                    }
                  /* Load attribute */
                  if (c != '>')
                    {
                      i = loadAttrs(*current, str, i, max);
                    }
                  else
                    {
                      i++;
                    }
                  /* Add to parent list */
                  if ((i >= 2) && (str[i - 1] == '>') && (str[i - 2] != '/'))
                    {
                      current->innerStartPos = i;
                      parents.push_back(current);
                      oldPos = i;
                    }
                  i--;
                  mode = 0;
                }
              break;
              }
          }
        else if (mode == 2)
          {
            /* Wait '>' */
            switch (c)
              {
            case '\'':
            case '\"':
              valueProtectedChar = c;
              mode = 3;
              break;
            case '>':
              mode = 0;
              break;
              }
          }
        else if (mode == 3)
          {
            /* Wait ' or " */
            if (c == valueProtectedChar)
              {
                mode = 2;
              }
          }
        else if (mode == 4)
          {
            /* Comment: Wait --> */
            if ((i >= oldPos + 3) && (c == '>') && (str[i - 1] == '-')
                && (str[i - 2] == '-'))
              {
                mode = 0;
                oldPos = i + 1;
              }
          }
        else if (mode == 5)
          {
            /* CDATA: Wait ]]> */
            if ((i >= 3) && (c == '>') && (str[i - 1] == ']')
                && (str[i - 2] == ']'))
              {
                mode = 0;
                /* Add data to parent inner code */
                if (parents.size() > 0)
                  {
                    parents.back()->text.append(str, oldPos, i - oldPos - 2);
                  }
                oldPos = i + 1;
              }
          }
        else if (mode == 6)
          {
            /* Wait '>' */
            if (c == '>')
              {
                if (parents.size() > 0)
                  {
                    parents.back()->endPos = i;
                    /* Pop the parent */
                    parents.pop_back();
                  }
                oldPos = i + 1;
                mode = 0;
              }
          }
      }
  }

  /** -loadAttrs */
  unsigned int
  XMLReader::loadAttrs(XMLReader::Balise& data, const std::string& str,
      unsigned int start, unsigned int max)
  {
    unsigned int oldPos = start;
    std::string name, value;
    unsigned char valueProtectedChar = '\'';
    /* 0=Name, 1=wait '=', 2=Value with ' or ",  3=Value with nothing, */
    /* 4=Wait type of value; 5=Wiat value type ' or " */
    int mode = 0;
    /* Search the attr */
    for (unsigned int i = start; i <= max; i++)
      {
        /* Get Char */
        unsigned char c = ((i == max) ? '>' : str[i]);
        /* Interpret the char */
        if (mode == 0)
          {
            /* Wait Attr */
            switch (c)
              {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
              /* Wait '=' */
              if (oldPos < i)
                {
                  if (!(name.empty()))
                    {
                      data.addAttr(name, value);
                      name.clear();
                      value.clear();
                    }
                  name.assign(str, oldPos, i - oldPos);
                  mode = 1;
                }
              oldPos = i + 1;
              break;
            case '=':
              /* Wait value */
              if (!(name.empty()))
                {
                  data.addAttr(name, value);
                  name.clear();
                  value.clear();
                }
              name.assign(str, oldPos, i - oldPos);
              oldPos = i + 1;
              mode = 4;
              break;
            case '>':
              /* Add Empty attr */
              if (oldPos < i)
                {
                  name.assign(str, oldPos, i - oldPos);
                  data.addAttr(name, value);
                  value.clear();
                }
              return i + 1;
            default:
              break;
              }
          }
        else if (mode == 1)
          {
            /* Wait '=' */
            switch (c)
              {
            case '=':
              /* Wait value type */
              mode = 4;
              oldPos = i + 1;
              break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
              /* Wait other char */
              oldPos = i + 1;
              break;
            case '>':
              /* Add Empty attr */
              if (oldPos < i)
                {
                  data.addAttr(name, value);
                  name.clear();
                  value.clear();
                }
              return i + 1;
            default:
              /* New Attribute */
              if (oldPos < i)
                {
                  name.assign(str, oldPos, i - oldPos);
                  data.addAttr(name, value);
                  value.clear();
                }
              oldPos = i;
              mode = 0;
              break;
              }
          }
        else if (mode == 4)
          {
            /* Wait value type */
            switch (c)
              {
            case '\'':
              mode = 2;
              valueProtectedChar = c;
              oldPos = i + 1;
              break;
            case '\"':
              mode = 2;
              valueProtectedChar = c;
              oldPos = i + 1;
              break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
              mode = 5;
              break;
            case '>':
              value.assign(str, oldPos, i - oldPos);
              data.addAttr(name, value);
              name.clear();
              value.clear();
              return i + 1;
              break;
            default:
              /* Value end */
              mode = 3;
              oldPos = i;
              break;
              }
          }
        else if (mode == 5)
          {
            /* Wait value type ' or " */
            switch (c)
              {
            case '\'':
              mode = 2;
              valueProtectedChar = c;
              oldPos = i + 1;
              break;
            case '\"':
              mode = 2;
              valueProtectedChar = c;
              oldPos = i + 1;
              break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
              /* NOP */
              break;
            case '>':
              data.addAttr(name, value);
              return i + 1;
            default:
              oldPos = i;
              mode = 0;
              break;
              }
          }
        else if (mode == 2)
          {
            /* Value with ' or " */
            if (c == valueProtectedChar)
              {
                value.assign(str, oldPos, i - oldPos);
                data.addAttr(name, value);
                name.clear();
                value.clear();
                mode = 0;
                oldPos = i + 1;
              }
          }
        else if (mode == 3)
          {
            /* Value with nothing */
            switch (c)
              {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
              /* Value end */
              value.assign(str, oldPos, i - oldPos);
              data.addAttr(name, value);
              name.clear();
              value.clear();
              mode = 0;
              oldPos = i + 1;
              break;
            case '>':
              /* Add Empty attr */
              if (oldPos < i)
                {
                  value.assign(str, oldPos, i - oldPos);
                  data.addAttr(name, value);
                  name.clear();
                  value.clear();
                }
              return i + 1;
              }
          }
      }
    return max;
  }

}
