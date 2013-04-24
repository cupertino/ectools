///////////////////////////////////////////////////////////////////////////////
/// @file		FormatType.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Contains all format type supported by this library
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_FORMATTYPE_H__
#define LIBEC_FORMATTYPE_H__

#include <string>

namespace cea
{

  /// @ingroup format
  /// @brief Type of the formatter supported
  ///
  /// Each time a new format is create for the library the author
  /// need to add the format as an enum item in this enumeration
  enum FormatType
  {
    UNKNOWN_FORMAT = 0, ///< Unknown format
    /* NOTE: Add Formatter Type here */
    CSV, ///< Comma-separated values format
    XML, ///< Extensible Markup Language format
    Gnuplot,
    Json,
  };

  /// @brief Type of block formatted (mainly used for log system)
  enum FormatBlockType
  {
    ITEM_DEFAULT = 0, ///< No diff direct add (default block type)
    ITEM_ADDED, ///< New item
    ITEM_ERASED, ///< Deleted item
    ITEM_UPDATED, ///< Updated item

    /* Count of block type */
    count_block_type ///< Reserved for library
  };

  /* Prototype of BaseFormat class */
  class BaseFormat;

  /// @brief Create a formatter from a string
  class FormatCreator
  {
  public:

    /// @brief Create a formatter from a string
    /// @param type Type of formatter to create
    ///	@return Pointer to the created format.
    ///			Or 0 if format not recognized.
    static BaseFormat*
    create(const std::string& type);

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
/// @ingroup format
/// @enum FormatBlockType
///
/// Allow to all formatter to select what block name use for 
///	an add,update or remove entry.
///
/// <h3>Example</h3>
/// <!--=======-->
/// For a list of entries 2 added item, 1 removed, 3 updated:
///
/// In CSV default block name are (+ for add, - for remove, . for update).
///	CSV formatted Result will look like:
///	@code
///		+,Item1
///		-,Item2
///		+,Item3
///		.,Item4
///		.,Item5
///		.,Item6
/// @endcode
///
/// In XML default block name are ("add", "delete", "update")
/// XML formatted result will look like:
/// @code
///		<add data="Item1" />
///		<delete data="Item2" />
///		<add data="Item3" />
///		<update data="Item4" />
///		<update data="Item5" />
///		<update data="Item6" />
/// @endcode
///
/// @see cea::CSVFormatter, cea::XMLFormatter
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @ingroup format
/// @class FormatCreator
///
/// This class is mainly used by the XML loader system.
/// Each new format need to have an entry in the create function
///
/// <h3>Example</h3>
/// <!--=======-->
/// Add format DumbFormat:
///
/// The function cea::FormatCreator::create will be:
///	@code
///		BaseFormat* FormatCreator::create(const std::string& type)
///		{
///			if (Tools::identicalString(type, "XML")) {
///				return new XMLFormat();
///			} else if ( (Tools::identicalString(type, "CSV")) ||
///						(Tools::identicalString(type, "CVS")) ) 
///			{
///				return new CSVFormat();
///
///			// ----------------------------------------------------
///			// FORMAT DUMB ADD
///			// ----------------------------------------------------
///			} else if (Tools::identicalString(type,"DUMBFormat")) {
///				return new DumbFormat();
///			// ----------------------------------------------------
///
///			/** NOTE: Add Format Type here */
///			
///			}
///			return 0;
///		}
/// @endcode
///
/// @see cea::GridViewLoader::loadXML, cea::MonitorLoader::loadXML
///////////////////////////////////////////////////////////////////////////////
