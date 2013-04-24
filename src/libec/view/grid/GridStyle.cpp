#include <libec/grid/view/GridStyle.h>

namespace cea
{
  /** + Constructor */
  GridStyle::GridStyle() :
      align(LEFT), color(-1), backColor(-1), bold(false), italic(false), underline(
          false), strikethrough(false), size(0), border(true)
  {
    ;
  }
  GridStyle::GridStyle(GridStyle::Align align) :
      align(align), color(-1), backColor(-1), bold(false), italic(false), underline(
          false), strikethrough(false), size(0), border(true)
  {
    ;
  }
  GridStyle::GridStyle(GridStyle::Align align, color_t color, color_t backcolor,
      bool bold, bool italic, bool underline, bool strikethrough,
      const std::string& prefix, const std::string& suffix, int size,
      bool border) :
      align(align), color(color), backColor(backcolor), bold(bold), italic(
          italic), underline(underline), strikethrough(strikethrough), prefix(
          prefix), suffix(suffix), size(size), border(border)
  {
    ;
  }

  /** +setAlign */
  GridStyle&
  GridStyle::setAlign(GridStyle::Align a)
  {
    align = a;
    return *this;
  }
  /** +setTextColor */
  GridStyle&
  GridStyle::setTextColor(color_t c)
  {
    color = c;
    return *this;
  }
  /** +setBackColor */
  GridStyle&
  GridStyle::setBackColor(color_t bc)
  {
    backColor = bc;
    return *this;
  }
  /** +setColor */
  GridStyle&
  GridStyle::setColor(color_t c, color_t bc)
  {
    color = c;
    backColor = bc;
    return *this;
  }
  /** +setBold */
  GridStyle&
  GridStyle::setBold(bool bOn)
  {
    bold = bOn;
    return *this;
  }
  /** +setItalic */
  GridStyle&
  GridStyle::setItalic(bool bOn)
  {
    italic = bOn;
    return *this;
  }
  /** +setUnderline */
  GridStyle&
  GridStyle::setUnderline(bool bOn)
  {
    underline = bOn;
    return *this;
  }
  /** +setStrikethrough */
  GridStyle&
  GridStyle::setStrikethrough(bool bOn)
  {
    strikethrough = bOn;
    return *this;
  }
  /** +setFont */
  GridStyle&
  GridStyle::setFont(bool b, bool i, bool u, bool s)
  {
    bold = b;
    italic = i;
    underline = u;
    strikethrough = s;
    return *this;
  }
  /** +setPrefix */
  GridStyle&
  GridStyle::setPrefix(const std::string& p)
  {
    prefix.assign(p);
    return *this;
  }
  /** +setSuffix */
  GridStyle&
  GridStyle::setSuffix(const std::string& s)
  {
    suffix.assign(s);
    return *this;
  }
  /** +setAffix */
  GridStyle&
  GridStyle::setAffix(const std::string& p, const std::string& s)
  {
    prefix.assign(p);
    suffix.assign(s);
    return *this;
  }
  /** +setBorder */
  GridStyle&
  GridStyle::setBorder(bool b)
  {
    border = b;
    return *this;
  }

  /** +loadXML */
  void
  GridStyle::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    /* Load align */
    std::string strAlign = balise.getAttr("align", "left");
    if (Tools::identicalString(strAlign, "right"))
      {
        align = RIGHT;
      }
    else if ((Tools::identicalString(strAlign, "center"))
        || (Tools::identicalString(strAlign, "middle")))
      {
        align = CENTER;
      }
    else
      {
        align = LEFT;
      }
    /* Load colors */
    color = Tools::htmlColorToRGB(balise.getAttr("color", "-1"));
    backColor = Tools::htmlColorToRGB(balise.getAttr("backcolor", "-1"));
    /* Load Affix */
    prefix = balise.getAttr("prefix");
    suffix = balise.getAttr("suffix");
    /* Load Bold/Italic/Underline/Strikethrough */
    std::string tmp = balise.getAttr("bold", "false");
    bold = ((tmp.compare("1") == 0) || (Tools::identicalString(tmp, "true")));
    tmp = balise.getAttr("italic", "false");
    italic = ((tmp.compare("1") == 0) || (Tools::identicalString(tmp, "true")));
    tmp = balise.getAttr("underline", "false");
    underline = ((tmp.compare("1") == 0)
        || (Tools::identicalString(tmp, "true")));
    tmp = balise.getAttr("strikethrough", "false");
    strikethrough = ((tmp.compare("1") == 0)
        || (Tools::identicalString(tmp, "true")));
  }

}
