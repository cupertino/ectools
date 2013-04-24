#include <libec/grid/model/GridItem.h>
#include <libec/grid/model/GridModel.h>

namespace cea
{

  /** #Constructor */
  GridItem::GridItem(GridModel& grid, int tag) :
      _id(0), data(0), grid(grid), _updateCounter(0), _fixed(false), _fixedOffset(
          0), _fixedAtEnd(false), tag(tag), args(0)
  {
    ;
  }

  /** #Destructor */
  GridItem::~GridItem()
  {
    if (args != 0)
      {
        for (ArgMap::iterator it = args->begin(); it != args->end(); ++it)
          {
            delete (it->second);
          }
        args->clear();
        delete args;
      }
  }

  /** #setData */
  void
  GridItem::setData(void* d)
  {
    data = d;
  }
  /** #getData */
  void*
  GridItem::getData() const
  {
    return data;
  }

  /** +setArg */
  GridItem&
  GridItem::setArg(unsigned int id, const Value& v)
  {
    /* Create the List if not instanciate */
    if (args == 0)
      {
        args = new ArgMap;
        /* Return the reference of the current object */
        if (args == 0)
          return *this;
      }
    /* Check if the args exist and calculate the right place to insert if not */
    ArgMap::iterator it = args->lower_bound(id);
    /* Check if args exist */
    if ((it != args->end()) && (!(args->key_comp()(id, it->first))))
      {
        /* Item Already exist - change value */
        (*(it->second)) = v;
      }
    else
      {
        /* Create the arg */
        Value* vTmp = new Value(v);
        /* Insert to the place pre-calculated */
        if (vTmp != 0)
          {
            args->insert(it, ArgMap::value_type(id, vTmp));
          }
      }
    /* Return the reference of the current object */
    return *this;
  }

  /** +getArg */
  const Value&
  GridItem::getArg(unsigned int id) const
  {
    /* Get the item */
    if (args != 0)
      {
        ArgMap::const_iterator it = args->find(id);
        if (it != args->end())
          {
            return *(it->second);
          }
      }
    /* If not found Return static value no arg */
    return GridModel::nvalue;
  }

  /** +getGrid */
  GridModel&
  GridItem::getGrid() const
  {
    return grid;
  }

  /** +getUpdateCount */
  unsigned int
  GridItem::getUpdateCount() const
  {
    return _updateCounter;
  }
  /** +incUpdateCounter */
  void
  GridItem::incUpdateCounter()
  {
    /* Update the counter (cyclic) */
    _updateCounter++;
    if (_updateCounter > 1234567890)
      {
        _updateCounter = 0;
      }
  }

  /** +setFixed */
  GridItem&
  GridItem::setFixed(int fixedOffset, bool fixedAtEnd)
  {
    _fixed = true;
    _fixedOffset = fixedOffset;
    _fixedAtEnd = fixedAtEnd;
    return *this;
  }

  /** +unfix */
  GridItem&
  GridItem::unfix()
  {
    _fixed = false;
    return *this;
  }

  /** +isFixed */
  bool
  GridItem::isFixed() const
  {
    return _fixed;
  }
  /** +getFixedOffset */
  int
  GridItem::getFixedOffset() const
  {
    return _fixedOffset;
  }
  /** +isFixedAtEnd */
  bool
  GridItem::isFixedAtEnd() const
  {
    return _fixedAtEnd;
  }

}
