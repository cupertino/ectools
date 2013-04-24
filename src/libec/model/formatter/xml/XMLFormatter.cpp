#include <libec/formatter/xml/XMLFormatter.h>

namespace cea
{

  /** +Constructor */
  XMLFormatter::XMLFormatter() :
      BaseFormatter(XML)
  {
    /* Set the default diff block name */
    _blkType[ITEM_DEFAULT].assign("data");
    _blkType[ITEM_ADDED].assign("add");
    _blkType[ITEM_ERASED].assign("delete");
    _blkType[ITEM_UPDATED].assign("update");
  }
  XMLFormatter::XMLFormatter(const XMLFormat& fmtOpt) :
      BaseFormatter(XML), _fmtOpt(fmtOpt)
  {
    /* Set the default diff block name */
    _blkType[ITEM_DEFAULT].assign("data");
    _blkType[ITEM_ADDED].assign("add");
    _blkType[ITEM_ERASED].assign("delete");
    _blkType[ITEM_UPDATED].assign("update");
  }

  /** +getOption */
  const XMLFormat&
  XMLFormatter::getOption() const
  {
    return _fmtOpt;
  }
  /** +setOption */
  void
  XMLFormatter::setOption(const BaseFormat& fmtOpt)
  {
    if (fmtOpt.getType() == XML)
      {
        _fmtOpt = static_cast<const XMLFormat&>(fmtOpt);
      }
  }

  /** +openBlock */
  void
  XMLFormatter::openBlock(const std::string& name)
  {
    /* Create a new block */
    std::string tmp(name);
    if (tmp.empty())
      tmp.assign("data");
    _formattedData.append(_fmtOpt._offsetIndent);
    _formattedData.append(_addedTab);
    _formattedData.append("<");
    htmlentities(tmp, true, _fmtOpt.attrMode);
    _formattedData.append(tmp);
    /* Add seprator */
    if (_fmtOpt.attrMode)
      {
        _formattedData.append(" ");
      }
    else
      {
        _formattedData.append(">\n");
      }
    /* Add an indent */
    _addedTab.append("\t");
  }

  /** +format */
  void
  XMLFormatter::format(const Value& data)
  {
    format("data", data);
  }

  /** +format */
  void
  XMLFormatter::format(const std::string& propertyName, const Value& data)
  {
    /* Format */
    std::string tmpName(propertyName);
    htmlentities(tmpName, true, _fmtOpt.attrMode);
    std::string tmpValue = data.toString();
    htmlentities(tmpValue, false, _fmtOpt.attrMode);
    /* Add separator */
    if (_fmtOpt.attrMode)
      {
        _formattedData.append(tmpName);
        _formattedData.append("=\"");
        _formattedData.append(tmpValue);
        _formattedData.append("\" ");
      }
    else
      {
        _formattedData.append(_fmtOpt._offsetIndent);
        _formattedData.append(_addedTab);
        _formattedData.append("<");
        _formattedData.append(tmpName);
        _formattedData.append(">");
        _formattedData.append(tmpValue);
        _formattedData.append("</");
        _formattedData.append(tmpName);
        _formattedData.append(">\n");
      }
  }

  /** +endBlock */
  void
  XMLFormatter::closeBlock(const std::string& name)
  {
    /* Remove an ident */
    if (_addedTab.length() > 0)
      {
        _addedTab.erase(_addedTab.length() - 1);
      }
    /* Write close balise */
    if (_fmtOpt.attrMode)
      {
        _formattedData.append("/>\n");
      }
    else
      {
        std::string tmpName = name;
        htmlentities(tmpName, true, _fmtOpt.attrMode);
        if (tmpName.empty())
          tmpName.assign("data");
        _formattedData.append(_fmtOpt._offsetIndent);
        _formattedData.append(_addedTab);
        _formattedData.append("</");
        _formattedData.append(tmpName);
        _formattedData.append(">\n");
      }
  }

  /** +formatComment */
  void
  XMLFormatter::formatComment(const std::string& comment)
  {
    /* Check if last data is a closed balise
     * To avoid to add a comment inner a balise attribute */
    unsigned int l = _formattedData.length();
    if (l > 1)
      {
        if (!((_formattedData[l - 1] == '>')
            || ((_formattedData[l - 2] == '>')
                && (_formattedData[l - 1] == '\n'))))
          {
            return;
          }
      }
    /* Format comment */
    std::string tmp = comment;
    htmlentities(tmp, false, false);
    _formattedData.append(_fmtOpt._offsetIndent);
    _formattedData.append(_addedTab);
    _formattedData.append("<!--");
    _formattedData.append(tmp);
    _formattedData.append("-->\n");
  }

  /** +clear */
  void
  XMLFormatter::clear()
  {
    _formattedData.clear();
  }

  /** +peek */
  void
  XMLFormatter::peek(std::string& resultFormattedString)
  {
    resultFormattedString.assign(_formattedData);
  }

  /** +htmlentities */
  void
  XMLFormatter::htmlentities(std::string& data, bool forXMLName,
      bool isAttrValue)
  {
    /* Replace '\"', '\'', '<', '>', '&' */
    for (unsigned int i = 0; i < data.length(); i++)
      {
        switch (data[i])
          {
        case '<':
          data.replace(i, 1, "&lt;");
          i += 3;
          break;
        case '>':
          data.replace(i, 1, "&gt;");
          i += 3;
          break;
        case '&':
          data.replace(i, 1, "&amp;");
          i += 4;
          break;
        case '\"':
          if (forXMLName)
            {
              data[i] = '_';
            }
          else if (isAttrValue)
            {
              data.replace(i, 1, "&quot;");
              i += 5;
            }
          break;
        case '\'':
          if (forXMLName)
            {
              data[i] = '_';
            }
          else if (isAttrValue)
            {
              data.replace(i, 1, "&#039;");
              i += 5;
            }
          break;
        case ' ':
        case '\t':
          if (forXMLName)
            {
              data[i] = '_';
            }
          break;
          }
      }
  }

}
