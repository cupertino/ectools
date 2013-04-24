#include <libec/formatter/xml/XMLFormat.h>
#include <libec/formatter/xml/XMLFormatter.h>

#include <libec/tools/Tools.h>

namespace cea
{

  /** +Constructor */
  XMLFormat::XMLFormat() :
      BaseFormat(XML), attrMode(true)
  {
    ;
  }
  XMLFormat::XMLFormat(bool attributeMode) :
      BaseFormat(XML), attrMode(attributeMode)
  {
    ;
  }
  XMLFormat::XMLFormat(int tabIndent) :
      BaseFormat(XML), attrMode(true)
  {
    _offsetIndent.resize((unsigned int) (tabIndent), '\t');
  }
  XMLFormat::XMLFormat(bool attributeMode, int tabIndent) :
      BaseFormat(XML), attrMode(attributeMode)
  {
    _offsetIndent.resize((unsigned int) (tabIndent), '\t');
  }
  XMLFormat::XMLFormat(const XMLFormat& fmtOpt) :
      BaseFormat(XML), attrMode(fmtOpt.attrMode), _offsetIndent(
          fmtOpt._offsetIndent)
  {
    ;
  }

  /** Assign operator */
  XMLFormat&
  XMLFormat::operator=(const XMLFormat& fmtOpt)
  {
    if (&fmtOpt != this)
      {
        attrMode = fmtOpt.attrMode;
        _offsetIndent.assign(fmtOpt._offsetIndent);
      }
    return *this;
  }

  /** +loadXML */
  void
  XMLFormat::loadXML(XMLReader& xml, const XMLReader::Balise& balise)
  {
    std::string strAttrMode = balise.getAttr("attr-mode", "true");
    attrMode = ((Tools::identicalString(strAttrMode, "true"))
        || (Tools::identicalString(strAttrMode, "1")));

    unsigned int tabIdent = balise.getAttrValue("indent", 0);
    if (tabIdent > 2048)
      tabIdent = 2048;
    _offsetIndent.clear();
    _offsetIndent.resize(tabIdent, '\t');
  }

  /** Create the Formatter */
  BaseFormatter*
  XMLFormat::createFormatter() const
  {
    return new XMLFormatter();
  }

}
