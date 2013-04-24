///////////////////////////////////////////////////////////////////////////////
/// @file		Manager.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Generic manager of element
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MANAGER_H__
#define LIBEC_MANAGER_H__

#include <iostream>
#include <string>
#include <map>

namespace cea
{

  /// @ingroup tools
  /// @brief Generic manager of element
  template<typename T>
    class BaseManager
    {
    public:

      /// @brief Add an element
      /// @param key Unique key of the element
      /// @param item Element to add
      void
      add(const std::string& key, T* item);

      /// @brief Remove an element by key
      /// @param key Key of the element to remove
      void
      remove(const std::string& key);

      /// @brief Remove all the occurrence of element
      /// @param element Element to remove
      void
      remove(T* element);

      /// @brief Get an element by key
      /// @param key Key of the element to retrieve
      /// @return Pointer to the corresponding element. \n
      ///         Or 0 if element not found.
      T*
      get(const std::string& key);

      /// @brief Get the unique key of an element container
      /// @param item Element container considered
      /// @return The element key. \n
      ///         Or "" if item not found.
      std::string
      getKey(T* item);

      /// @brief Replace an element container
      /// @param key Unique key of the element to replace
      /// @param item New element container to set
      /// @return true if element container replaced
      bool
      set(const std::string& key, T* item);

      /// @brief Replace an unique key of an element
      /// @param key Key of the element considered
      /// @param newKey New unique key to set
      /// @return true if the new unique key is set
      bool
      setKey(const std::string& key, const std::string& newKey);
      /// @brief Replace an unique key of an element
      /// @param element Element considered
      /// @param newKey New unique key to set
      /// @return true if new unique key is set
      bool
      setKey(T* element, const std::string& newKey);

    protected:

      /* Member */
      std::map<std::string, T*> _items; ///< Elements map

    };

  /// @brief Generic manager which can manage any memory "element"
  class Manager : public BaseManager<void*>
  {
  };

  /* +add */
  template<typename T>
    void
    BaseManager<T>::add(const std::string &key, T *item)
    {
      /* Check if name not already present before add */
      if (get(key) == 0)
        {
          _items.insert(std::map<std::string, T*>::value_type(key, item));
        }
    }

  /* +remove */
  template<typename T>
    void
    BaseManager<T>::remove(const std::string &key)
    {
      typename std::map<std::string, T*>::iterator it = _items.find(key);
      if (it != _items.end())
        {
          _items.erase(it);
        }
    }

  /* +remove */
  template<typename T>
    void
    BaseManager<T>::remove(T *element)
    {
      for (typename std::map<std::string, T*>::iterator it = _items.begin();
          it != _items.end();)
        {
          if (it->second == element)
            {
              _items.erase(it++);
              return;
            }
          else
            {
              ++it;
            }
        }
    }

  /* +get */
  template<typename T>
    T*
    BaseManager<T>::get(const std::string &key)
    {
      typename std::map<std::string, T*>::iterator it = _items.find(key);
      if (it != _items.end())
        {
          return it->second;
        }
      else
        {
          return 0;
        }
    }

  /* +getKey */
  template<typename T>
    std::string
    BaseManager<T>::getKey(T* item)
    {
      /* Search the element */
      for (typename std::map<std::string, T*>::iterator it = _items.begin();
          it != _items.end(); ++it)
        {
          if (it->second == item)
            {
              return it->first;
            }
        }
      /* If not found return "" */
      return "";
    }

  /* +set */
  template<typename T>
    bool
    BaseManager<T>::set(const std::string& key, T* item)
    {
      typename std::map<std::string, T*>::iterator it = _items.find(key);
      if (it != _items.end())
        {
          it->second = item;
          return true;
        }
      else
        {
          return false;
        }
    }

  /* +setKey */
  template<typename T>
    bool
    BaseManager<T>::setKey(const std::string& key, const std::string& newKey)
    {
      /* Check if new key not already exist */
      if (get(newKey) != 0)
        return false;
      /* Change the key of the given element if found it */
      typename std::map<std::string, T*>::iterator it = _items.find(key);
      if (it != _items.end())
        {
          /* Change the key */
          it->first = newKey;
          return true;
        }
      else
        {
          return false;
        }
    }

  /* +setKey */
  template<typename T>
    bool
    BaseManager<T>::setKey(T* element, const std::string& newKey)
    {
      /* Check if new key not already exist */
      if (get(newKey) != 0)
        return false;
      /* Search the element */
      for (typename std::map<std::string, T*>::iterator it = _items.begin();
          it != _items.end(); ++it)
        {
          if (it->second == element)
            {
              it->first = newKey;
              return true;
            }
        }
      /* If not found return true */
      return false;
    }

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Manager
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////
