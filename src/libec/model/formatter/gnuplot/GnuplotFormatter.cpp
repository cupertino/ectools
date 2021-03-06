#include <libec/formatter/gnuplot/GnuplotFormatter.h>

namespace cea
{

  /** +Constructor */
  GnuplotFormatter::GnuplotFormatter() :
      BaseFormatter(Gnuplot)
  {
    /* Set the default diff block name */
    _blkType[ITEM_ADDED].assign("+");
    _blkType[ITEM_ERASED].assign("-");
    _blkType[ITEM_UPDATED].assign(".");
  }
  GnuplotFormatter::GnuplotFormatter(const GnuplotFormat& fmtOpt) :
      BaseFormatter(Gnuplot)
  {
    /* Set the default diff block name */
    _blkType[ITEM_ADDED].assign("+");
    _blkType[ITEM_ERASED].assign("-");
    _blkType[ITEM_UPDATED].assign(".");
  }

  /** +getOption */
  const GnuplotFormat&
  GnuplotFormatter::getOption() const
  {
    return _fmtOpt;
  }
  /** +setOption */
  void
  GnuplotFormatter::setOption(const BaseFormat& fmtOpt)
  {
    if (fmtOpt.getType() == Gnuplot)
      {
        _fmtOpt = static_cast<const GnuplotFormat&>(fmtOpt);
      }
  }

  /** +openBlock */
  void
  GnuplotFormatter::openBlock(const std::string& name)
  {
    /* Check if last char on formatted data is separator */
    if ((_formattedData.length() > 0)
        && (_formattedData[_formattedData.length() - 1] != '\n'))
      {
        _formattedData.append("\n");
      }
    /* Format current name */
    std::string tmp = name;
    strToGnuplot(tmp);
    _formattedData.append(tmp);
  }

  /** +format */
  void
  GnuplotFormatter::format(const Value& data)
  {
    /* Check if last char on formatted data is separator */
    if (_formattedData.length() > 0)
      {
        unsigned char c = _formattedData[_formattedData.length() - 1];
        if ((c != ',') && (c != '\n'))
          {
            _formattedData.append(_fmtOpt.separator);
          }
      }
    /* Format current data */
    std::string tmp = data;
    strToGnuplot(tmp);
    _formattedData.append(tmp);
  }

  /** +format */
  void
  GnuplotFormatter::format(const std::string& propertyName, const Value& data)
  {
    format(data);
  }

  /** +endBlock */
  void
  GnuplotFormatter::closeBlock(const std::string& name)
  {
    /* Check if last char on formatted data is separator */
    if ((_formattedData.length() > 0)
        && (_formattedData[_formattedData.length() - 1] != '\n'))
      {
        _formattedData.append("\n");
      }
  }

  /** +formatComment */
  void
  GnuplotFormatter::formatComment(const std::string& comment)
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
    //strToGnuplot(tmp);
    /* New Line */
    _formattedData.append(tmp);
    _formattedData.append("\n");
  }

  /** +clear */
  void
  GnuplotFormatter::clear()
  {
    _formattedData.clear();
  }

  /** +peek */
  void
  GnuplotFormatter::peek(std::string& resultFormattedString)
  {
    resultFormattedString.assign(_formattedData);
  }

  /** +strToCVS */
  void
  GnuplotFormatter::strToGnuplot(std::string& data)
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
            {
              bNeedProtectedChar = true;
            }
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
