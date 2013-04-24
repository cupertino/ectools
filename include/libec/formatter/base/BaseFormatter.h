///////////////////////////////////////////////////////////////////////////////
/// @file		BaseFormatter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class for Formatter
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_BASEFORMATTER_H__
#define LIBEC_BASEFORMATTER_H__

#include <iostream>
#include <string>

#include "../../value/Value.h"
#include "../FormatType.h"
#include "BaseFormat.h"

namespace cea
{

  /// @brief Abstract class for Formatter
  class BaseFormatter
  {
  public:

    /* Constructor */
    /// @brief Create the base formatter and set the type of formatter
    /// @param type Type of format
    /// @see cea::FormatType
    BaseFormatter(FormatType type);

    /* Property */
    /// @brief Change generic name of a block type
    /// @param blockType Block type to change
    /// @param name New name of the block type
    void
    setBlockType(FormatBlockType blockType, const std::string& name);

    /// @brief Get generic name for a block type
    /// @param blockType The Block Type
    /// @return Name of the block type
    std::string
    getBlockType(FormatBlockType blockType);

    /// @brief Get the formatter type
    /// @return the format type
    FormatType
    getType() const;

    /// @brief Set option of the formatter
    ///
    /// Formatter have setting manage by Format class. \n
    /// For each formatter a format class need to be create. \n
    /// This function need to be overridden.
    ///
    /// @param fmtOpt Corresponding Format class of the Formatter. \n
    virtual void
    setOption(const BaseFormat& fmtOpt) = 0;

    /// @brief Get the option of the formatter
    ///
    /// This function need to be overridden.
    ///
    /// @return Corresponding Format class of the Formatter
    /// @see setOption
    virtual const BaseFormat&
    getOption() const = 0;

    /* Block open/close */
    /// @brief Open a block using generic name from a block type
    ///
    /// When a block is open you need to close it by calling
    /// closeBlockType.
    ///
    /// @param blockType Type of block to open
    /// @see FormatBlockType
    void
    openBlockType(FormatBlockType blockType);

    /// @brief Close a block using generic name from a block type
    /// @param blockType Type of block to close
    /// @see FormatBlockType, openBlockType
    void
    closeBlockType(FormatBlockType blockType);

    /// @brief Open a named block
    ///
    /// This function need to be overridden. \n
    /// When a block is open you need to close it by calling
    /// closeBlock.
    ///
    /// @param name Name of the block to open
    virtual void
    openBlock(const std::string& name = "") = 0;

    /// @brief Close a named block
    ///
    /// This function need to be overridden.
    ///
    /// @param name Name of the block to close
    virtual void
    closeBlock(const std::string& name = "") = 0;

    /* Format data */
    /// @brief Format a data with name
    ///
    /// This function need to be overridden. \n
    /// Generally a data is associated to a name.
    ///
    /// @param propertyName Name of the data to format
    /// @param data Data to format
    virtual void
    format(const std::string& propertyName, const Value& data) = 0;

    /// @brief Format a data with no name.
    ///
    /// This function need to be overridden. \n
    /// Generally a data is associated to a name.
    ///
    /// @param data Data to format
    virtual void
    format(const Value& data) = 0;

    /* Format a Comment */
    /// @brief Format a comment
    ///
    /// This function need to be overridden.
    ///
    /// @param comment Comment to format
    virtual void
    formatComment(const std::string& comment) = 0;

    /* Format entries manipulation */
    /// @brief Clear the formatted data
    ///
    /// This function need to be overridden.
    ///
    virtual void
    clear() = 0;

    /// @brief Peek all formatted values
    ///
    /// This function need to be overridden.
    ///
    /// @param resultFormattedString Out string for formmatted data
    virtual void
    peek(std::string& resultFormattedString) = 0;

  protected:

    /* Members */
    FormatType _type; ///< Type of the format
    std::string _blkType[count_block_type]; ///< Block Type map name

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::BaseFormatter
///	@ingroup format
///
/// To create a new format you need to public extend this class.
/// This class represent the formatter itself and use the corresponding extend
/// of BaseFormat as option.
///
///////////////////////////////////////////////////////////////////////////////
