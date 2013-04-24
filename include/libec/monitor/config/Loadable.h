///////////////////////////////////////////////////////////////////////////////
/// @file		Loadable.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Loader class
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MONITORABLE_H__
#define LIBEC_MONITORABLE_H__

namespace cea
{

  /// @ingroup monitor
  /// @brief "Interface" of a loader of element
  class BaseLoadable
  {
  public:
    virtual
    ~BaseLoadable()
    {
    }

    /// @brief Get generic pointer
    /// @return Generic pointer
    virtual void*
    getPtr() = 0;

    /// @brief Create object and keep generic pointer
    /// @return This class
    virtual BaseLoadable*
    create() = 0;
  };

  /// @ingroup monitor
  /// @brief Generic loader element
  template<typename T>
    class Loadable : public BaseLoadable
    {
    public:

      /// @brief Create a loader of T object
      Loadable() :
          ptr(0)
      {
      }
      /// @brief Store a T object
      /// @param ptr Pointer of the object to surround
      Loadable(T* ptr) :
          ptr(ptr)
      {
      }

      /// @brief Destroys the containing object with the right type
      /// @return Generic pointer
      virtual
      ~Loadable()
      {
        if (ptr != 0)
          {
            delete ptr;
          }
      }

      /// @brief Get generic pointer of loaded object
      /// @return Generic pointer or 0 if no object loaded
      virtual void*
      getPtr()
      {
        return ptr;
      }

      /// @brief Create an object and keep pointer
      /// @return Container object of created element
      virtual BaseLoadable*
      create()
      {
        return new Loadable<T>(new T);
      }

    private:

      /* Member */
      T* ptr; ///< Loaded element surrounded

    };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Loadable
///
/// This class is used by MonitorLoader to automaticly create Monitorable 
/// object and destroy it when no more used.
///
///////////////////////////////////////////////////////////////////////////////
