///////////////////////////////////////////////////////////////////////////////
/// @file		DoubleLinkedList.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Generic Double linked list
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_DOUBLELINKEDLIST_H__
#define LIBEC_DOUBLELINKEDLIST_H__

#include <iostream>

namespace cea
{

  /* -------------------------------------------------------------------------- */
  /*	Class definition */

  /// @brief Generic Double linked list
  template<typename T>
    class DoubleLinkedList
    {
    public:

      /* Structure/Type */

      /// @brief Represent the item
      struct item
      {
        /* List id */
        unsigned int fromLstId; ///< DoubleLinkedList parent id

        /* Previous and Next linked item */
        item* prev; ///< Previous linked item or 0 if first item
        item* next; ///< Next linked item or 0 if last item

        /* Current value */
        T value; ///< Value of this item

        /// @brief Return the value of this item
        /// @return Value of this item
        T&
        operator*()
        {
          return value;
        }
        /// @brief Return value of this item
        /// @return Value of this item
        T*
        operator->()
        {
          return &value;
        }

        /* Navigate operator */
        /// @brief Goto next linked item
        /// @return Next linked item or 0 if last item
        item*
        operator++()
        {
          return next;
        }
        /// @brief Goto previous linked item
        /// @return Previous linked item or 0 if first item
        item*
        operator--()
        {
          return prev;
        }

      };

      /// @brief Type of an iterator
      typedef item* iterator;

      /* Static variable */
      /// @brief Represent invalid item
      static item* nitem;
      /// @brief Represent invalid position
      static const unsigned int npos = (unsigned int) (-1);
      /// @brief Represent invalid value
      static T nvalue;

      /* Constructor/Destructor */
      /// @brief Default constructor
      DoubleLinkedList();

      /// @brief Destroy all created memory
      ~DoubleLinkedList();

      /* Insert item function */
      /// @brief Insert an item
      /// @param value Value to insert
      /// @param after If after == 0 Then push_front(value)
      /// @return Pointer to the created item or 0
      item*
      insert(const T& value, item* after = 0);
      /// @brief Insert to back an item
      /// @param value Value to insert
      /// @return Pointer to the created item or 0
      item*
      push_back(const T& value);
      /// @brief Insert to front an item
      /// @param value Value to insert
      /// @return Pointer to the created item or 0
      item*
      push_front(const T& value);

      /* Get item function */
      /// @brief Get first item of the list
      /// @return Pointer to the first item or 0 if no items
      item*
      begin();
      /// @brief Get last item of the list
      /// @return Always 0
      item*
      end();
      /// @brief Get front item of the list
      /// @return Pointer to the front item or 0 if no items
      item*
      front();
      /// @brief Get back item of the list
      /// @return Pointer to the back item or 0 if no items
      item*
      back();

      /* Iterator function */
      /// @brief Get next item of an iterator
      ///
      /// Identical to it = it->next
      ///
      /// @param it Iterator considered
      /// @return Pointer to next item of it
      item*
      next(item* it);
      /// @brief Get previous item of an iterator
      ///
      /// Identical to it = it->prev
      ///
      /// @param it Iterator considered
      /// @return Pointer to next item of it
      item*
      previous(item* it);
      /// @brief Return an offset iterator
      /// @param it Iterator considered
      /// @param offset Number of item to move
      /// @return Pointer to offseted iterator
      item*
      advance(item* it, int offset);
      /// @brief Get item at a specific position
      /// @param id Position of item to get
      /// @return Item at position Or 0 if no item exist at id position
      item*
      getIt(signed int id);
      /// @brief Get item at a specific position
      /// @param id Position of item to get
      /// @return Item at position Or 0 if no item exist at id position
      item*
      getIt(unsigned int id);

      /// @brief Check if an item is valid
      /// @param value Item to test
      /// @return True if item is valid
      bool
      isItemValid(const item* value) const;

      /// @brief Get position of an item
      /// @param it Item considered
      /// @return Position of it Or nvalue if it no longer to the list
      unsigned int
      getPos(item* it) const;

      /* Pop/Erase item */
      /// @brief Pop from back an item
      /// @return Popped value
      T
      pop_back();
      /// @brief Pop from front an item
      /// @return Popped value
      T
      pop_front();

      /// @brief Clear all items
      void
      clear();

      /// @brief Erase item and return the next valid item pointer
      /// @param it Item to erase
      /// @return Next valid item
      item*
      erase(item* it);
      /// @brief Erase range of item and return the next valid item pointer
      /// @param begin Begin item to erase
      /// @param end End item to erase
      /// @return Next valid item
      item*
      erase(item* begin, item* end);
      /// @brief Erase range of item and return the next valid item pointer
      /// @param begin Begin item to erase
      /// @param count Count of item to erase
      /// @return Next valid item
      item*
      erase(item* begin, unsigned int count);

      /// @brief Get count of item
      /// @return Number of item
      unsigned int
      size() const;

      /// @brief Swap items
      /// @param it1 Item to swap
      /// @param it2 Item to swap
      void
      swap(item* it1, item* it2);

      /// @brief Move an item
      /// @param it1 Item to move
      /// @param afterIt If afterIt == 0 Then
      ///                   move it1 to the beginning of the list
      item*
      move(item* it1, item* afterIt);
      /// @brief Get value by a specific item
      /// @param it Item containing value
      /// @return If it not belong to the list return nvalue
      T&
      get(item* it);
      /// @brief Get value at a specific position
      /// @param id Position of the item
      /// @return If position is invalid Then return nvalue
      T&
      operator[](signed int id);
      /// @brief Get value at a specific position
      /// @param id Position of the item
      /// @return If position is invalid Then return nvalue
      T&
      operator[](unsigned int id);

      /// @brief Get value at a specific position
      /// @param id Position of the item
      /// @return If position is invalid Then return nvalue
      T&
      getValue_const(unsigned int id) const;

      /// @brief Check if a value is valid
      /// @param value Value to test
      /// @return true if the value is valid
      bool
      isValid(const T& value) const;

    protected:

      /* Members */
      item* _first; ///< First item
      item* _last; ///< Last item
      unsigned int _size; ///< Item count

      /// @brief List id provided by DoubleLinkedListCount::value
      unsigned int _lstId;

    };

  /* -------------------------------------------------------------------------- */
  /*	Linked List counter */

  /// @ingroup tools
  /// @brief Counter for DoubleLinkedList
  class DoubleLinkedListCount
  {
  private:

    /// @brief DoubleLinkedList count
    ///
    /// This value is used to know if an item parameter is on the right
    /// list.
    ///
    static unsigned int value;

    template<typename T> friend class DoubleLinkedList;
  };

  /* -------------------------------------------------------------------------- */
  /*	Implementation */

  /* ----------------------- */
  /* Static member */

  /* const representing the non item */
  template<typename T>
    typename DoubleLinkedList<T>::item* DoubleLinkedList<T>::nitem = 0;

  /* const representing no value */
  template<typename T>
    T DoubleLinkedList<T>::nvalue;

  /* ----------------------- */
  /* Constructor/Destructor */

  /* +Constructor */
  template<typename T>
    DoubleLinkedList<T>::DoubleLinkedList() :
        _first(0), _last(0), _size(0)
    {
      DoubleLinkedListCount::value++;
      _lstId = DoubleLinkedListCount::value;
    }

  /* +Destructor */
  template<typename T>
    DoubleLinkedList<T>::~DoubleLinkedList()
    {
      clear();
    }

  /* ----------------------- */
  /* Item add */

  /* +insert */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::insert(const T& value,
        typename DoubleLinkedList<T>::item* after)
    {
      /* Create item */
      item* it = new item();
      if (it == 0)
        return 0;
      it->value = value;
      it->fromLstId = _lstId;
      /* Check if first item */
      if (_first == 0)
        {
          /* First item added */
          it->prev = 0;
          it->next = 0;
          _first = it;
          _last = it;
        }
      else if (after == 0)
        {
          /* Link to first item */
          _first->prev = it;
          it->prev = 0;
          it->next = _first;
        }
      else if (after == _last)
        {
          /* Link to last item */
          _last->next = it;
          it->prev = _last;
          it->next = 0;
        }
      else
        {
          /* Link to after item */
          it->next = after->next;
          it->next->prev = it;
          it->prev = after;
          after->next = it;
        }
      /* Increment size */
      _size++;
      /* Return the created item */
      return it;
    }

  /* +push_back */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::push_back(const T& value)
    {
      /* Create item */
      item* it = new item();
      if (it == 0)
        return 0;
      it->value = value;
      it->fromLstId = _lstId;
      /* Add to Last */
      if (_first == 0)
        {
          /* First item added */
          it->prev = 0;
          it->next = 0;
          _first = it;
          _last = it;
        }
      else
        {
          /* Link to last item */
          _last->next = it;
          it->prev = _last;
          it->next = 0;
          _last = it;
        }
      /* Increment size */
      _size++;
      /* Return the created item */
      return it;
    }

  /* +push_front */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::push_front(const T& value)
    {
      /* Create item */
      item* it = new item();
      if (it == 0)
        return 0;
      it->value = value;
      it->fromLstId = _lstId;
      /* Add to First */
      if (_first == 0)
        {
          /* First item added */
          it->prev = 0;
          it->next = 0;
          _first = it;
          _last = it;
        }
      else
        {
          /* Link to first item */
          _first->prev = it;
          it->prev = 0;
          it->next = _first;
          _first = it;
        }
      /* Increment size */
      _size++;
      /* Return the created item */
      return it;
    }

  /* ----------------------- */
  /* Item "navigation" */

  /* +begin */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::begin()
    {
      return _first;
    }
  /* +end */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::end()
    {
      return 0;
    }

  /* +front */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::front()
    {
      return _first;
    }
  /* +back */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::back()
    {
      return _last;
    }

  /* +next */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::next(typename DoubleLinkedList<T>::item* it)
    {
      if (it == 0)
        return 0;
      return it->next;
    }
  /* +previous */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::previous(typename DoubleLinkedList<T>::item* it)
    {
      if (it == 0)
        return 0;
      return it->prev;
    }
  /* +advance */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::advance(typename DoubleLinkedList<T>::item* it,
        int offset)
    {
      if ((it == 0) || (offset == 0))
        return it;
      if (offset < 0)
        {
          /* Decrement item */
          while (offset < 0)
            {
              if (it->prev == 0)
                return it;
              it = it->prev;
              offset++;
            }
        }
      else
        {
          /* Increment Item */
          while (offset > 0)
            {
              if (it->next == 0)
                return it;
              it = it->next;
              offset--;
            }
        }
      /* Return it */
      return it;
    }

  /* +getIt */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::getIt(signed int id)
    {
      return (getIt((unsigned int) (id)));
    }
  /* +getIt */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::getIt(unsigned int id)
    {
      /* Check if id no out of item */
      if (id >= _size)
        return 0;
      /* if id <= size / 2 start from first to last (++) */
      if (id <= _size / 2)
        {
          for (item* it = _first; it != 0; it = it->next)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it;
              id--;
            }
        }
      else
        {
          /* Recalculate id */
          id = (_size - id - 1);
          /* start from last to first (--) */
          for (item* it = _last; it != 0; it = it->prev)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it;
              id--;
            }
        }
      /* If no item found return 0 */
      return 0;
    }
  /* +isItemValid */
  template<typename T>
    bool
    DoubleLinkedList<T>::isItemValid(
        const typename DoubleLinkedList<T>::item* it) const
    {
      return ((it != 0) && (it->fromLstId == _lstId));
    }

  /* +getPos */
  template<typename T>
    unsigned int
    DoubleLinkedList<T>::getPos(typename DoubleLinkedList<T>::item* it) const
    {
      /* Search the position of the item */
      unsigned int pos = 0;
      if ((it != 0) && (it->fromLstId == _lstId))
        {
          for (item* i = _first; i != 0; i = i->next)
            {
              if (i == it)
                {
                  return pos;
                }
              ++pos;
            }
        }
      /* If not found return npos */
      return npos;
    }

  /* ----------------------- */
  /* Item remove */

  /* +pop_front */
  template<typename T>
    T
    DoubleLinkedList<T>::pop_front()
    {
      T retValue;
      /* Pop the value */
      if (_first != 0)
        {
          item* tmp = _first;
          retValue = _first->value;
          /* Change next/prev */
          _first = _first->next;
          if (_first != 0)
            _first->prev = 0;
          /* Delete from memory */
          delete tmp;
          /* Change the size */
          _size--;
        }
      if (_size == 0)
        {
          _first = 0;
          _last = 0;
        }
      return retValue;
    }
  /* +pop_back */
  template<typename T>
    T
    DoubleLinkedList<T>::pop_back()
    {
      T retValue = nvalue;
      /* Pop the value */
      if (_last != 0)
        {
          item* tmp = _last;
          retValue = _last->value;
          /* Change next/prev */
          _last = _last->prev;
          if (_last != 0)
            _last->next = 0;
          /* Delete from memory */
          delete tmp;
          /* Change the size */
          _size--;
        }
      if (_size == 0)
        {
          _first = 0;
          _last = 0;
        }
      /* Return the value */
      return retValue;
    }

  /* +clear */
  template<typename T>
    void
    DoubleLinkedList<T>::clear()
    {
      /* Clear from memory all the item */
      for (item* it = _first; it != 0;)
        {
          item* tmp = it->next;
          delete it;
          it = tmp;
        }
      /* Reset members */
      _first = 0;
      _last = 0;
      _size = 0;
    }

  /* +erase */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::erase(typename DoubleLinkedList<T>::item* it)
    {
      /* Check if item is valid */
      if (_size == 0)
        return it->next;
      if ((it != 0) && (it->fromLstId != _lstId))
        return 0;
      if (it == 0)
        it = _last;
      /* Change the link */
      item* nextIt = it->next;
      if (it->prev != 0)
        {
          it->prev->next = it->next;
        }
      else
        {
          _first = it->next;
          if (_first != 0)
            _first->prev = 0;
        }
      if (it->next != 0)
        {
          it->next->prev = it->prev;
        }
      else
        {
          _last = it->prev;
          if (_last != 0)
            _last->next = 0;
        }
      /* Delete the item */
      delete it;
      /* Reduce the size of the list */
      _size--;
      if (_size == 0)
        {
          _first = 0;
          _last = 0;
        }
      /* Return the next of it */
      return nextIt;
    }
  /* +erase */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::erase(typename DoubleLinkedList<T>::item* begin,
        typename DoubleLinkedList<T>::item* end)
    {
      /* Check if item is valid */
      if (_size == 0)
        return begin->next;
      if ((begin == 0) || (begin->fromLstId != _lstId))
        return begin->next;
      if ((end != 0) && (end->fromLstId != _lstId))
        return begin->next;
      if (end == 0)
        end = _last;
      /* Keep the next of end item */
      item* nextIt = end->next;
      /* Delete all the item */
      item* max = end->next;
      for (item* it = begin->next; (it != 0) && (it != max);)
        {
          item* tmp = it->next;
          delete it;
          it = tmp;
          _size--;
        }
      /* Change the item Link */
      if (begin->prev != 0)
        {
          begin->prev->next = nextIt;
        }
      else
        {
          _first = nextIt;
          if (_first != 0)
            _first->prev = 0;
        }
      if (nextIt != 0)
        {
          nextIt->prev = begin->prev;
        }
      else
        {
          _last = begin->prev;
          if (_last != 0)
            _last->next = 0;
        }
      /* Delete the begin case */
      delete begin;
      _size--;
      if (_size == 0)
        {
          _first = 0;
          _last = 0;
        }
      /* Return the valid next item */
      return nextIt;
    }
  /* +erase */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::erase(typename DoubleLinkedList<T>::item* begin,
        unsigned int count)
    {
      /* Check if item is valid */
      if (_size == 0)
        return begin->next;
      if ((begin == 0) || (begin->fromLstId != _lstId))
        return begin->next;
      if (count == 0)
        count = _size;
      /* Find the end item */
      for (item* end = begin; end != 0; end = end->next)
        {
          if ((count <= 1) || (end->next == 0))
            {
              return erase(begin, end);
            }
          count--;
        }
      /* If not found return begin next */
      return begin->next;
    }

  /* ----------------------- */
  /* Tools method */

  /* +size */
  template<typename T>
    unsigned int
    DoubleLinkedList<T>::size() const
    {
      return _size;
    }

  /* +swap */
  template<typename T>
    void
    DoubleLinkedList<T>::swap(typename DoubleLinkedList<T>::item* it1,
        typename DoubleLinkedList<T>::item* it2)
    {
      if ((it1 != 0) && (it2 != 0) && (it1->fromLstId == _lstId)
          && (it2->fromLstId == _lstId))
        {
          T tmp = it1->value;
          it1->value = it2->value;
          it2->value = tmp;
        }
    }
  /* +move */
  template<typename T>
    typename DoubleLinkedList<T>::item*
    DoubleLinkedList<T>::move(typename DoubleLinkedList<T>::item* it1,
        typename DoubleLinkedList<T>::item* afterIt)
    {
      if ((it1 != 0) && (it1->fromLstId == _lstId)
          && ((afterIt == 0)
              || ((afterIt != 0) && (afterIt->fromLstId == _lstId))))
        {
          /* Return Item */
          item* retIt = it1->prev;
          /* Change Next and Prev case from it1 */
          if (it1 != _first)
            {
              it1->prev->next = it1->next;
            }
          else
            {
              _first = it1->next;
              retIt = _first;
            }
          if (it1 != _last)
            {
              it1->next->prev = it1->prev;
            }
          else
            {
              _last = it1->prev;
            }
          /* Move it1 */
          /* Check where to move it1 */
          if (afterIt == 0)
            {
              /* It1 become the first item of the list */
              it1->prev = 0;
              it1->next = _first;
              _first->prev = it1;
              _first = it1;
            }
          else if (afterIt == _last)
            {
              /* Move It1 to end */
              it1->prev = _last;
              it1->next = 0;
              _last->next = it1;
              _last = it1;
            }
          else
            {
              /* Move It1 after afterIt */
              it1->prev = afterIt;
              it1->next = afterIt->next;

              afterIt->next->prev = it1;
              afterIt->next = it1;
            }
          /* Return item */
          return retIt;
        }
      else
        {
          return it1;
        }
    }

  /* +get */
  template<typename T>
    T&
    DoubleLinkedList<T>::get(typename DoubleLinkedList<T>::item* it)
    {
      if (it == 0)
        return nvalue;
      return it->value;
    }
  /* +operator[] */
  template<typename T>
    T&
    DoubleLinkedList<T>::operator[](signed int id)
    {
      return operator[]((unsigned int) (id));
    }
  /* +operator[] */
  template<typename T>
    T&
    DoubleLinkedList<T>::operator[](unsigned int id)
    {
      /* Check if id no out of item */
      if (id >= _size)
        return nvalue;
      /* if id <= size / 2 start from first to last (++) */
      if (id <= _size / 2)
        {
          for (item* it = _first; it != 0; it = it->next)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it->value;
              id--;
            }
        }
      else
        {
          /* Recalculate id */
          id = (_size - id - 1);
          /* start from last to first (--) */
          for (item* it = _last; it != 0; it = it->prev)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it->value;
              id--;
            }
        }
      /* If no value found return nvalue */
      return nvalue;
    }
  /* +getValue_const */
  template<typename T>
    T&
    DoubleLinkedList<T>::getValue_const(unsigned int id) const
    {
      /* Check if id no out of item */
      if (id >= _size)
        return nvalue;
      /* if id <= size / 2 start from first to last (++) */
      if (id <= _size / 2)
        {
          for (item* it = _first; it != 0; it = it->next)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it->value;
              id--;
            }
        }
      else
        {
          /* Recalculate id */
          id = (_size - id - 1);
          /* start from last to first (--) */
          for (item* it = _last; it != 0; it = it->prev)
            {
              /* if id == 0 then Found */
              if (id == 0)
                return it->value;
              id--;
            }
        }
      /* If no value found return nvalue */
      return nvalue;
    }

  /* +isValid */
  template<typename T>
    bool
    DoubleLinkedList<T>::isValid(const T& value) const
    {
      return (&value != &nvalue);
    }

  /* ----------------------- */
  /* Friend method */

  /// @brief Implement std::ostream display
  template<typename T>
    std::ostream&
    operator<<(std::ostream& out, DoubleLinkedList<T>& lst)
    {
      /* Send to output stream all value */
      for (typename DoubleLinkedList<T>::item* it = lst.begin(); it != 0; it =
          it->next)
        {
          out << it->value;
          /* Separator for value = ',' */
          if (it != lst.back())
            out << ",";
        }
      /* Return the output stream */
      return out;
    }

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::DoubleLinkedList
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////
