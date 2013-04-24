#include <libec/formatter/base/BaseFormatter.h>

namespace cea
{

  /** +Constructor */
  BaseFormatter::BaseFormatter(FormatType fmtType) :
      _type(fmtType)
  {
    ;
  }

  /** +getType */
  FormatType
  BaseFormatter::getType() const
  {
    return _type;
  }

  /** +setBlockType */
  void
  BaseFormatter::setBlockType(FormatBlockType blockType,
      const std::string& name)
  {
    unsigned int blkId = (unsigned int) (blockType);
    if (blkId < (unsigned int) (count_block_type))
      {
        _blkType[blkId].assign(name);
      }
  }

  /** +getBlockType */
  std::string
  BaseFormatter::getBlockType(FormatBlockType blockType)
  {
    unsigned int blkId = (unsigned int) (blockType);
    if (blkId < (unsigned int) (count_block_type))
      {
        return _blkType[blkId];
      }
    else
      {
        return "";
      }
  }

  /** +openBlock */
  void
  BaseFormatter::openBlockType(FormatBlockType blockType)
  {
    /* Open with the name of the corresponding block */
    if (blockType < count_block_type)
      {
        openBlock(_blkType[blockType]);
      }
  }
  /** +closeBlock */
  void
  BaseFormatter::closeBlockType(FormatBlockType blockType)
  {
    /* Close with the name of the corresponding block */
    if (blockType < count_block_type)
      {
        closeBlock(_blkType[blockType]);
      }
  }

}
