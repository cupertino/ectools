///////////////////////////////////////////////////////////////////////////////
/// @file		XMLReader.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		XML DOM parser able to read standard XML format
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_XML_H__
#define LIBEC_XML_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>

#include <libec/value/Value.h>

namespace cea
{

  /// @brief XML DOM parser able to read standard XML format
  class XMLReader
  {
  public:

    /// @brief Representing xml attribute
    struct Attr
    {
      /// @brief Name of the attribute
      std::string name;
      /// @brief Value of the attribute
      std::string value;
    };

    /// @brief Type for list of attribute
    typedef std::list<Attr*> AttrList;
    /// @brief Type for map <Attr.Name, Attr*>
    typedef std::map<std::string, Attr*> AttrMap;

    /* Prototype of balise */
    struct Balise;

    /// @brief Type of a document
    typedef std::list<Balise*> BaliseList;
    /// @brief Type of Map <Balise.Name, Balise*>
    typedef std::map<std::string, Balise*> BaliseMap;

    /// @brief Representing xml balise
    struct Balise
    {
      /// @brief Name of the balise
      std::string name;

      /// @brief Lists of attributes
      AttrList attrs;
      /// @brief Map of attributes
      AttrMap attrsMap;

      /// @brief Pointer to the parent balise
      ///
      /// If (root) Then parent=0
      ///
      Balise* parent;
      /// @brief List of childs
      BaliseList childs;
      /// @brief Map of childs
      BaliseMap childsMap;

      /// @brief Content of balise without child balise
      std::string text;

      /// @brief Debug infos - start position
      unsigned int startPos;
      /// @brief Debug infos - end position
      unsigned int endPos;
      /// @brief Debug infos - Inner start position
      unsigned int innerStartPos;
      /// @brief Debug infos - Inner end position
      unsigned int innerEndPos;

      /// @brief Default constructor
      Balise();

      /// @brief Destruct from memory all attr
      ~Balise();

      /// @brief Get the count of balise
      /// @return balise count
      unsigned int
      getChildCount() const;
      /// @brief Get a balise by id
      /// @param id Position of the balise in the document
      /// @return const Pointer to the corresponding balise or 0 if not found
      const Balise*
      getChild(unsigned int id) const;
      /// @brief Get a balise by path
      /// @param path near of XPath (@ref cea_XMLReader_balisePath)
      /// @return const Pointer to the corresponding balise or 0 if not found
      const Balise*
      getChild(const std::string& path) const;

      /// @brief Clear all the attributes
      void
      clearAttr();

      /// @brief Get an attribute value
      /// @param name Name of the Attribute (@ref cea_XMLReader_attrName)
      /// @param defaultValue Value returned if attr not found
      /// @return value of the attr
      std::string
      getAttr(const std::string& name,
          const std::string defaultValue = "") const;
      /// @brief Get an attribute value
      /// @param name Name of the Attribute (@ref cea_XMLReader_attrName)
      /// @param defaultValue Value returned if attr not found
      /// @return value of the attr
      Value
      getAttrValue(const std::string& name, const Value& defaultValue =
          Value::defaultValue) const;

      /// @brief Get an attribute value from child balise
      /// @param path near of XPath (@ref cea_XMLReader_balisePath)
      /// @param name Name of the Attribute (@ref cea_XMLReader_attrName)
      /// @param defaultValue Value returned if attr not found
      /// @return value of the attr
      std::string
      getBaliseAttr(const std::string& path, const std::string& name,
          const std::string defaultValue = "") const;
      /// @brief Get an attribute value from child balise
      /// @param path near of XPath (@ref cea_XMLReader_balisePath)
      /// @param name Name of the Attribute (@ref cea_XMLReader_attrName)
      /// @param defaultValue Value returned if attr not found
      /// @return value of the attr
      Value
      getBaliseAttrValue(const std::string& path, const std::string& name,
          const Value& defaultValue = Value::defaultValue) const;

      /// @brief Add an attribute
      /// @param name Name of the attribute
      /// @param value Value of the attribute
      void
      addAttr(const std::string& name, const std::string& value);
    };

    /// @brief Deleted from memory all created elements
    ~XMLReader();

    /* General - Section */
    /// @brief Clear from memory all balise and attr
    void
    clear();

    /* Load - Section */
    /// @brief Parse a file into "XML" DOM
    /// @param file File to open and parse
    /// @return number of balise loaded
    unsigned int
    loadFile(const std::string& file);

    /// @brief Parse a string into "XML" DOM
    /// @param str str to parse
    /// @return number of balise loaded
    unsigned int
    loadString(const std::string& str);
    /// @brief Parse a string into "XML" DOM
    /// @param str str to parse
    /// @param start Position where start parsing
    /// @param len Length of str to parse if 0 parse the full string
    /// @return number of balise loaded
    unsigned int
    loadString(const std::string& str, unsigned int start,
        unsigned int len = 0);

    /* Balise - Section */
    /// @brief Get the count of balise
    /// @return balise count
    unsigned int
    getBaliseCount() const;
    /// @brief Get a balise by id
    /// @param id Position of the balise in the document
    /// @return const Pointer to the corresponding balise or 0 if not found
    const Balise*
    getBalise(unsigned int id) const;
    /// @brief Get a balise by path
    /// @param path near of XPath (@ref cea_XMLReader_balisePath)
    /// @return const Pointer to the corresponding balise or 0 if not found
    const Balise*
    getBalise(const std::string& path) const;

    /// @brief Get innerCode string of balise
    /// @param path near of XPath (@ref cea_XMLReader_balisePath)
    /// @param defaultValue Value returned if attr not found
    /// @return innerCode in string
    std::string
    getText(const std::string& path, const std::string& defaultValue = "");

    /// @brief Get inner value of balise
    /// @param path near of XPath (@ref cea_XMLReader_balisePath)
    /// @param defaultValue Value returned if no balise found
    /// @return value of inner code
    Value
    getTextValue(const std::string& path, const Value& defaultValue =
        Value::defaultValue);

    /// @brief Get an attribute value
    /// @param balisePath near of XPath (@ref cea_XMLReader_balisePath)
    /// @param attrName Name of the Attribute (@ref cea_XMLReader_attrName)
    /// @param defaultValue Value returned if attr not found
    /// @return string of the attr or defaultValue if attr not found
    std::string
    getAttr(const std::string& balisePath, const std::string& attrName,
        const std::string& defaultValue = "");

    /// @brief Get an attribute value
    /// @param balisePath near of XPath (@ref cea_XMLReader_balisePath)
    /// @param attrName Name of the Attribute (@ref cea_XMLReader_attrName)
    /// @param defaultValue Value returned if attr not found
    /// @return value of the attr or defaultValue if attr not found
    Value
    getAttrValue(const std::string& balisePath, const std::string& attrName,
        const Value& defaultValue = Value::defaultValue);

    /// \brief Get a single valued tag in the format
    /// \verbatim<tagId value="">\endverbatim from a tag input
    /// \param tag    Input tag to be evaluated
    /// \param tagId  Tag's identifier (may be one subtag from the input tag)
    /// \param to     Output value will be stored in this variable
    template<typename T>
      static void
      readSingleValuedTag(const char* tag, const char* tagId, T& to);

    /// \brief Get an attribute's value from a tag in the format
    /// \verbatim<tagId attributeId="">\endverbatim
    /// \param tag          Input tag to be evaluated
    /// \param tagId        Tag's identifier (may be one subtag from the input tag)
    /// \param attributeId  Attribute's identifier
    /// \param to           Output value will be stored in this variable
    template<typename T>
      static void
      readAttributeFromTag(const char* tag, const char* tagId,
          const char* attributeId, T& to);

  protected:

    /* Members */

    /// @brief XML document
    BaliseList doc;
    /// @brief Virtual Balise representing the root of the document
    Balise root;

  private:

    /* Private functions */

    /// @brief Load all balise from string
    /// @param str String to parse
    /// @param start Parsing start position
    /// @param max Parsing end position
    void
    loadBalise(const std::string& str, unsigned int start, unsigned int max);

    /// @brief Load all attribute from string
    /// @param data Balise container
    /// @param str String to parse
    /// @param start Parsing start position
    /// @param max Parsing end position
    /// @return Position after loaded attributes
    unsigned int
    loadAttrs(Balise& data, const std::string& str, unsigned int start,
        unsigned int max);

  };

  template<typename T>
    void
    XMLReader::readSingleValuedTag(const char* tag, const char* tagId, T& to)
    {
      readAttributeFromTag(tag, tagId, "value", to);
    }

  template<typename T>
    void
    XMLReader::readAttributeFromTag(const char* tag, const char* tagId,
        const char* attributeId, T& to)
    {
      std::string tagStr, tmpStr;
      unsigned begin, end, pos;
      std::stringstream ss;

      tagStr = tag;

      // Find the tag id and resize the tagStr to have only this tag
      ss << "<" << tagId;
      begin = tagStr.find(ss.str());
      if (begin == std::string::npos)
        return; //error: there is no tag ID in the submitted tag

      end = tagStr.substr(begin).find(">");
      if (end == std::string::npos)
        return; //error: no end tag available

      tagStr = tagStr.substr(begin, end);

      // Look for the attribute id inside the filtered tag
      ss.str("");
      ss << attributeId << "=\"";
      begin = tagStr.find(ss.str());

      if (begin == std::string::npos)
        return; //the tag do not have such attribute

      tmpStr = attributeId;
      pos = begin + tmpStr.length() + 2;
      end = tagStr.substr(pos).find("\"");
      if (end == std::string::npos)
        return; //the tag do not have such attribute

      tagStr = tagStr.substr(pos, end);

      // Convert attribute value to output type
      ss.str("");
      ss.clear();
      ss << tagStr;
      ss >> to;
    }

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::XMLReader
///	@ingroup tools
///
/// @section cea_XMLReader_balisePath Balise path format
///	The balise path is a little XPath format, in fact only separator can be
/// used to search a balise, no array or XPath function can be used. \n
/// Basically the balise path consist of Balise::name separated by a char
/// which can be '/', '\', '|' or ';'. \n
/// Exemple of balise path to get title of an well-formed html page : 
/// "html/head/title".
///
///
///	@section cea_XMLReader_attrName Attribute format
///	The attribute take care of case and respect the XML attribute format.
///
///////////////////////////////////////////////////////////////////////////////
