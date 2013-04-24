///////////////////////////////////////////////////////////////////////////////
/// @file		Value.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Generic value
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_VALUE_H__
#define LIBEC_VALUE_H__

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace cea
{

  /// @brief Dynamic memory allocation and deletion mode for value
  ///
  /// If set to 1 Then avoid dynamic memory allocation and deletion
  /// Else use dynamic memory to store value. \n
  ///
  /// When dynamic memory mode is disabled this class run faster
  /// but take more memory space since memory space is reserved.
  ///
#define CEA_VALUE_FAST_MODE 0

  /// @brief Generic value
  class Value
  {
  public:

    /* Static Value */
    /// @brief Default value
    static Value defaultValue;

    /// @brief Type of value supportted
    enum Type
    {
      UNKNOWN = 0, ///< Unknown type
      BOOL, ///< bool type
      INT, ///< long long int type
      UINT, ///< unsigned long long int type
      DOUBLE, ///< double
      STRING ///< std::string
    };

    /* Type generic */
    /// @brief Typedef for bool type
    typedef bool Bool;
    /// @brief Typedef for all signed Integer type
    typedef signed long long int Int;
    /// @brief Typedef for all unsigned Integer type
    typedef unsigned long long int UInt;
    /// @brief Typedef for all Float type
    typedef double Double;
    /// @brief Typedef for std::string type
    typedef std::string String;

    /* Constructor */
    /// @brief Default constructor set value type to Type::UNKNOWN
    Value();
    /// @brief Construct a Value from a copy of a C++ base type
    ///
    /// Construct a Value with corresponding Type of v and
    /// initialize the value with v
    ///
    /// @param v Initial value
    template<typename T>
      Value(const T& v);
    /// @brief Construct a typed Value and set the corresponding
    ///        default value
    ///
    /// <h3>Default value</h3>
    ///		<ul>
    ///			<li><b>UNKNOWN</b> -- No default value</li>
    ///			<li><b>BOOL</b> -- default value == false</li>
    ///			<li><b>INT</b> -- default value == 0</li>
    ///			<li><b>UINT</b> -- default value == 0</li>
    ///			<li><b>DOUBLE</b> -- default value == 0</li>
    ///			<li><b>STRING</b> -- default value == ""</li>
    ///		</ul>
    ///
    /// @param type Type of the Value
    Value(const Type& type);
    /// @brief Construct a BOOL Value set an initial value
    /// @param v Initial value
    Value(const bool& v);
    /// @brief Construct a STRING Value set an initial value
    /// @param v Initial value
    Value(const std::string& v);
    /// @brief Construct a STRING Value set an initial value
    ///
    /// Consider that v value stop at the first '\\0'.
    ///
    /// @param v Initial value
    Value(const char* v);
    /// @brief Construct a Value from a copy of another value
    /// @param v Value to copy
    Value(const Value& v);

    /* Destructor */
#if CEA_VALUE_FAST_MODE == 0
    /// @brief Destroy all the memory created
    ~Value();
#endif

    /* Auto type change property */
    /// @brief Get auto change type status
    /// @return true if Value change type when new type encountered
    bool
    getAutoChangeType() const;
    /// @brief Set auto change type status
    /// @param autoChangeType Auto change type status
    void
    setAutoChangeType(bool autoChangeType);

    /* Reset default value */
    /// @brief Set the value to the default value of the current Type
    ///
    /// <h3>Default value</h3>
    ///		<ul>
    ///			<li><b>UNKNOWN</b> -- No default value</li>
    ///			<li><b>BOOL</b> -- default value == false</li>
    ///			<li><b>INT</b> -- default value == 0</li>
    ///			<li><b>UINT</b> -- default value == 0</li>
    ///			<li><b>DOUBLE</b> -- default value == 0</li>
    ///			<li><b>STRING</b> -- default value == ""</li>
    ///		</ul>
    ///
    void
    reset();

    /* Type property */
    /// @brief Get Type of the value
    /// @return Current value Type
    Type
    getType() const;
    /// @brief Change the value Type
    ///
    /// If the new Type is different then the value is reset.
    ///
    /// @param newType New type to set
    void
    setType(Type newType);

    /* Return the Value::Type from any base type */
    /// @brief Get corresponding value Type of a bool
    /// @param t Ignored
    /// @return Type::BOOL
    static Type
    getValueType(const bool& t);
    /// @brief Get corresponding value Type of signed short int
    /// @param t Ignored
    /// @return Type::INT
    static Type
    getValueType(const signed short int& t);
    /// @brief Get corresponding value Type of unsigned short int
    /// @param t Ignored
    /// @return Type::UINT
    static Type
    getValueType(const unsigned short int& t);
    /// @brief Get corresponding value Type of signed int
    /// @param t Ignored
    /// @return Type::INT
    static Type
    getValueType(const signed int& t);
    /// @brief Get corresponding value Type of unsigned int
    /// @param t Ignored
    /// @return Type::UINT
    static Type
    getValueType(const unsigned int& t);
    /// @brief Get corresponding value Type of signed long int
    /// @param t Ignored
    /// @return Type::INT
    static Type
    getValueType(const signed long int& t);
    /// @brief Get corresponding value Type of unsigned long int
    /// @param t Ignored
    /// @return Type::UINT
    static Type
    getValueType(const unsigned long int& t);
    /// @brief Get corresponding value Type of signed long long int
    /// @param t Ignored
    /// @return Type::INT
    static Type
    getValueType(const signed long long int& t);
    /// @brief Get corresponding value Type of unsigned long long int
    /// @param t Ignored
    /// @return Type::UINT
    static Type
    getValueType(const unsigned long long int& t);
    /// @brief Get corresponding value Type of float
    /// @param t Ignored
    /// @return Type::DOUBLE
    static Type
    getValueType(const float& t);
    /// @brief Get corresponding value Type of double
    /// @param t Ignored
    /// @return Type::DOUBLE
    static Type
    getValueType(const double& t);
    /// @brief Get corresponding value Type of std::string
    /// @param t Ignored
    /// @return Type::STRING
    static Type
    getValueType(const std::string& t);
    /// @brief Get corresponding value Type of Value class
    /// @param v Value considered
    /// @return v.Type
    static Type
    getValueType(const Value& v);

    /* Return true if the type is correct to retrieve value in retValue */
    /// @brief Retrieve the value
    /// @param retValue Out value
    /// @return true if retValue type is correct to contain value
    template<typename T>
      bool
      get(T& retValue) const;

    /* Set the value */
    /// @brief Set the value
    ///
    /// If Type of Value not corresponding with v type then
    /// v is convert to Value Type.
    ///
    /// @param v value to set
    template<typename T>
      void
      set(const T& v);
    /// @brief Set the value
    ///
    /// If Type of Value not corresponding with v type then
    /// v is convert to Value Type.
    ///
    /// @param v value to set
    void
    set(const char* v);
    /// @brief Set the value
    ///
    /// If Type of Value not corresponding with v type then
    /// v is convert to Value Type.
    ///
    /// @param v value to set
    void
    set(const Value& v);

    /* operator() (get) convert operator */
    /// @brief Convert the value into any C++ base type
    /// @return Converted value
    template<typename T>
      operator T() const;

    /* operator= (set) */
    /// @brief Set the value
    ///
    /// If Type of Value not corresponding with v type then
    /// v is convert to Value Type.
    ///
    /// @param v value to set
    template<typename T>
      Value&
      operator=(const T& v);

    /* operator copy */
    /// @brief Copy a Value
    /// @param v Value to copy
    /// @return Reference to the current object
    Value&
    operator=(const Value& v);

    /* Convert function return the value and convert if needed */
    /// @brief Convert value to std::string
    /// @return String result of value
    std::string
    toString() const;

    /* Compare method */
    /// @brief Compare value to another Value class
    /// @param v Value class to compare with
    /// @return If (v > withValue) Then 1 \n
    ///         Else If (v < withValue) Then -1 \n
    ///         Else If (v == withValue) Then 0
    signed short int
    compare(const Value& v) const;

    /* Comparison operator */
    /// @brief Check if value == to v
    /// @param v Value class to compare with
    /// @return true if value == v.value
    bool
    operator==(const Value& v);
    /// @brief Check if value != to v
    /// @param v Value class to compare with
    /// @return true if value != v.value
    bool
    operator!=(const Value& v);
    /// @brief Check if value < to v
    /// @param v Value class to compare with
    /// @return true if value < v.value
    bool
    operator<(const Value& v);
    /// @brief Check if value <= to v
    /// @param v Value class to compare with
    /// @return true if value <= v.value
    bool
    operator<=(const Value& v);
    /// @brief Check if value > to v
    /// @param v Value class to compare with
    /// @return true if value > v.value
    bool
    operator>(const Value& v);
    /// @brief Check if value >= to v
    /// @param v Value class to compare with
    /// @return true if value >= v.value
    bool
    operator>=(const Value& v);

    /* Operator -= */
    /// @brief Decrement value of v
    /// @param v Value class subtracted
    /// @return Reference to the current object
    Value&
    operator-=(const Value& v);

    /* Operator += */
    /// @brief Increment value of v
    /// @param v Value class added
    /// @return Reference to the current object
    Value&
    operator+=(const Value& v);

    /* Operator output stream */
    /// @brief Output on std::ostream the value
    ///
    /// This function allow to use value with out stream
    ///
    /// <h3>Example</h3>
    /// @code
    ///	    // Value to show
    ///     Value v = 23.5;
    ///
    ///     // Show the value
    ///     std::cout << "Value = " << v << std::endl;
    ///
    ///		// Change the value type
    ///     v.setType(Value::STRING);
    ///
    ///     // Set a new value
    ///     v = "text to show";
    ///
    ///     // Show the value
    ///     std::cout << "Value = " << v << std::endl;
    ///
    ///
    ///     // Screen result
    ///     // Value = 23.5
    ///     // Value = text to show
    ///
    /// @endcode
    ///
    /// @param oss std::ostream considered
    /// @param v Value to output
    /// @return Modified oss
    friend std::ostream&
    operator<<(std::ostream& oss, const Value& v);

    /* Operator input stream */
    /// @brief Set the value with a std::istream
    ///
    /// This function allow to use value with in stream
    ///
    /// <h3>Example</h3>
    /// @code
    ///	    // Value to retrieve
    ///     Value v(Value::FLOAT);
    ///
    ///     // Get the value
    ///     std::cin >> v;
    ///
    ///     // Show the value
    ///     std::cout << "Value = " << v << std::endl;
    ///
    ///
    ///     // Screen input
    ///     // 456823.54
    ///     // Screen result
    ///     // Value = 456823.54
    ///
    /// @endcode
    ///
    /// @param iss std::istream considered
    /// @param v Value to set
    /// @return iss
    friend std::istream&
    operator>>(std::istream& iss, Value& v);

  private:

    /* Members */
    Type _type; ///< Current type of the Value
#if CEA_VALUE_FAST_MODE == 0
    /// @brief Pointer of memory block containing the value
    unsigned char* _value;
#else
    /// @brief Block of memory containing the value
    unsigned char _value[128];
#endif
    bool _boolValue; ///< Bool value
#if CEA_VALUE_FAST_MODE == 0
    std::string* _strValue; ///< Pointer to the String value
#else
    std::string _strValue; ///< String value
#endif
    /// @brief Indicate if change the type follow value setted
    bool _autoChangeType;

    /* Private inline function */
    /// @brief Copy value from another Value class
    /// @param v Value class to copy
    template<typename T>
      inline void
      copyValue(const Value& v);

    /// @brief Compare value with a Value class
    /// @param v value of any type to compare
    /// @param withValue Value class to compare width
    /// @return If (v > withValue) Then 1 \n
    ///         Else If (v < withValue) Then -1 \n
    ///         Else If (v == withValue) Then 0
    template<typename T>
      inline int
      compareValue(const T& v, const Value& withValue) const;

  };

  /* -copyValue */
  template<typename T>
    inline void
    Value::copyValue(const Value& v)
    {
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
        {
          _value = new unsigned char[sizeof(T)];
          if (_value != 0)
            {
              *(T*) (_value) = *(T*) (v._value);
            }
        }
      else
        {
          _value = 0;
        }
#else
      *(T*)(_value) = *(T*)(v._value);
#endif
    }

  /* -compareValue */
  template<typename T>
    inline int
    Value::compareValue(const T& v, const Value& withValue) const
    {
      switch (withValue._type)
        {
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (withValue._value == 0)
          return -1;
#endif
        if ((Int) (v) == (*(Int*) (withValue._value)))
          return 0;
        if ((Int) (v) > (*(Int*) (withValue._value)))
          return 1;
        return -1;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (withValue._value == 0)
          return -1;
#endif
        if ((UInt) (v) == (*(UInt*) (withValue._value)))
          return 0;
        if ((UInt) (v) > (*(UInt*) (withValue._value)))
          return 1;
        return -1;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (withValue._value == 0)
          return -1;
#endif
        if ((Double) (v) == (*(Double*) (withValue._value)))
          return 0;
        if ((Double) (v) > (*(Double*) (withValue._value)))
          return 1;
        return -1;
      default:
        return -1;
        }
    }
  /// @brief Compare value with a Value class
  /// @param v value of any type to compare
  /// @param withValue Value class to compare width
  /// @return If (v > withValue) Then 1 \n
  ///         Else If (v < withValue) Then -1 \n
  ///         Else If (v == withValue) Then 0
  template<>
    inline int
    Value::compareValue<bool>(const bool& v, const Value& withValue) const;
  /// @brief Compare value with a Value class
  /// @param v value of any type to compare
  /// @param withValue Value class to compare width
  /// @return If (v > withValue) Then 1 \n
  ///         Else If (v < withValue) Then -1 \n
  ///         Else If (v == withValue) Then 0
  template<>
    inline int
    Value::compareValue<std::string>(const std::string& v,
        const Value& withValue) const;

  /* +Constructor */
  template<typename T>
    Value::Value(const T& v) :
        _type(UNKNOWN),
#if CEA_VALUE_FAST_MODE == 0
            _value(0),
#endif
            _boolValue(false),
#if CEA_VALUE_FAST_MODE == 0
            _strValue(0),
#endif
            _autoChangeType(false)
    {
      setType(Value::getValueType(v));
      set(v);
    }

  /* +get */
  template<typename T>
    bool
    Value::get(T& retValue) const
    {
      switch (_type)
        {
      case BOOL:
        retValue = _boolValue;
        return false;
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (T) (*(Int*) (_value));
            return true;
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (T) (*(UInt*) (_value));
            return true;
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (T) (*(Double*) (_value));
            return true;
          }
        break;
      case STRING:
#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
#endif
          {
            std::stringstream oss;
#if CEA_VALUE_FAST_MODE == 0
            oss << (*_strValue);
#else
            oss << _strValue;
#endif
            oss >> retValue;
            return true;
          }
        break;
      default:
        break;
        }
      return false;
    }
  /// @brief Retrieve the value
  /// @param retValue Out value
  /// @return true if retValue type is correct to contain value
  template<>
    bool
    Value::get<bool>(bool& retValue) const;
  /// @brief Retrieve the value
  /// @param retValue Out value
  /// @return true if retValue type is correct to contain value
  template<>
    bool
    Value::get<std::string>(std::string& retValue) const;

  /* +set */
  template<typename T>
    void
    Value::set(const T& v)
    {
      /* Change the type of the variable if autochange activate */
      if (_autoChangeType)
        {
          setType(getValueType(v));
        }
      /* Set the Value function of the current type of the value */
      switch (_type)
        {
      case BOOL:
        _boolValue = (v == 1);
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(Int*) (_value) = (Int) (v);
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(UInt*) (_value) = (UInt) (v);
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(Double*) (_value) = (Double) (v);
          }
        break;
      case STRING:
#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
#endif
          {
            std::ostringstream oss;
            oss << v;
#if CEA_VALUE_FAST_MODE == 0
            _strValue->assign(oss.str());
#else
            _strValue.assign(oss.str());
#endif
          }
        break;
      default:
        break;
        }
    }
  /// @brief Set the value
  ///
  /// If Type of Value not corresponding with v type then
  /// v is convert to Value Type.
  ///
  /// @param v value to set
  template<>
    void
    Value::set<bool>(const bool& v);
  /// @brief Set the value
  ///
  /// If Type of Value not corresponding with v type then
  /// v is convert to Value Type.
  ///
  /// @param v value to set
  template<>
    void
    Value::set<std::string>(const std::string& v);

  /* +operator (convert) (get) */
  template<typename T>
    Value::operator T() const
    {
      T tmp;
      get(tmp);
      return tmp;
    }

  /* operator= (set) */
  template<typename T>
    Value&
    Value::operator=(const T& v)
    {
      set(v);
      return *this;
    }

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Value
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////
