#include <libec/formatter/json/JsonFormatter.h>

namespace cea
{

  /** +Constructor */
  JsonFormatter::JsonFormatter() :
      BaseFormatter(Json)
  {
    /* Set the default diff block name */
    _blkType[ITEM_ADDED].assign("+");
    _blkType[ITEM_ERASED].assign("-");
    _blkType[ITEM_UPDATED].assign(".");
  }
  JsonFormatter::JsonFormatter(const JsonFormat& fmtOpt) :
      BaseFormatter(Json)
  {
    /* Set the default diff block name */
    _blkType[ITEM_ADDED].assign("+");
    _blkType[ITEM_ERASED].assign("-");
    _blkType[ITEM_UPDATED].assign(".");
  }

  /** +getOption */
  const JsonFormat&
  JsonFormatter::getOption() const
  {
    return _fmtOpt;
  }
  /** +setOption */
  void
  JsonFormatter::setOption(const BaseFormat& fmtOpt)
  {
    if (fmtOpt.getType() == Json)
      {
        _fmtOpt = static_cast<const JsonFormat&>(fmtOpt);
      }
  }

  /** +openBlock */
  void
  JsonFormatter::openBlock(const std::string& name)
  {
    /* Check if last char on formatted data is separator */
    if ((_formattedData.length() > 0)
        && (_formattedData[_formattedData.length() - 1] != '\n'))
      {
        _formattedData.append("\n");
      }
    /* Format current name */

    if (name.length() > 0)
      {
        std::string tmp = "";
        if ((name[0] == '[') || (name[0] == '{'))
          {
            if (name.length() > 1)
              {
                tmp = name.substr(1, name.length() - 1);
                strToJson(tmp);
                tmp = " " + tmp + " : ";
                tmp = name[0] + tmp;
              }
            else
              {
                tmp = name[0];
              }
          }
        else
          {
            if ((name == ",["))
              {
                tmp = name;
              }
            else
              {
                tmp = name;
                strToJson(tmp);
                tmp.insert(0, "[");
                tmp.append(": ");
              }
          }
        _formattedData.append(tmp);
      }

    else
      {
        _formattedData.append("{");
      }
  }

  /** +format */
  void
  JsonFormatter::format(const Value& data)
  {
    /* Check if last char on formatted data is separator */
    if (_formattedData.length() > 0)
      {
        unsigned char c = _formattedData[_formattedData.length() - 1];
        if ((c != '{') && (c != '[') && (c != ',') && (c != '\n'))
          {
            _formattedData.append(_fmtOpt.separator);
          }
      }
    /* Format current data */
    std::string tmp = data.toString();
    strToJson(tmp);
    _formattedData.append(tmp);
  }

  /** +format */
  void
  JsonFormatter::format(const std::string& propertyName, const Value& data)
  {
    format(data);
  }

  /** +endBlock */
  void
  JsonFormatter::closeBlock(const std::string& name)
  {
    if ((name == "}") || (name == "]"))
      {
        _formattedData.append(name + " ");
      }
    else
      { /* Check if last char on formatted data is separator */
        if ((_formattedData.length() > 0)
            && (_formattedData[_formattedData.length() - 1] != '\n'))
          {
            _formattedData.append("], ");
          }
      }
  }

  /** +formatComment */
  void
  JsonFormatter::formatComment(const std::string& comment)
  {
    /* Check if last char on formatted data is separator */
    if ((_formattedData.length() > 0)
        && (_formattedData[_formattedData.length() - 1] != '\n'))
      {
        _formattedData.append("\n");
      }
    /* Append comment */
    std::string tmp;
    tmp.assign("#");
    tmp.append(comment);
    strToJson(tmp);
    if (tmp[0] != '\"')
      {
        tmp.insert(0, "\"");
        tmp.append("\"");
      }
    /* New Line */
    _formattedData.append(tmp);
//    _formattedData.append("\n");
  }

  /** +clear */
  void
  JsonFormatter::clear()
  {
    _formattedData.clear();
  }

  /** +peek */
  void
  JsonFormatter::peek(std::string& resultFormattedString)
  {
    resultFormattedString.assign(_formattedData);
  }

  /** +strToJson */
  void
  JsonFormatter::strToJson(std::string& data)
  {
    bool bNeedProtectedChar = false;
    char forbidChar = (
        _fmtOpt.separator.length() > 0 ? _fmtOpt.separator[0] : ',');

    /* Replace '"' by '""' */
    for (unsigned int i = 0, max = data.length(); i < max; i++)
      {
        switch (data[i])
          {
        case ',':
        case '\n':
        case ';':
          bNeedProtectedChar = true;
          break;
        case '"':
          bNeedProtectedChar = true;
          data.replace(i, 1, "\"");
          i++;
          break;
        default:
          if (data[i] == forbidChar)
            bNeedProtectedChar = true;
          if ((data[i] >= 'A') && (data[i] <= 'z'))
            bNeedProtectedChar = true;
          break;
          }

      }
    /* Protected if needed */
    if (bNeedProtectedChar)
      {
        data.insert(0, "\"");
        data.append("\"");
      }
  }

}
