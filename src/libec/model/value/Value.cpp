#include <libec/value/Value.h>

namespace cea
{
  /** Static value */
  Value Value::defaultValue;

  /** +Constructor */
  Value::Value() :
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
    /* By default value is INT */
    setType(INT);
    set(0);
  }

  Value::Value(const Value::Type& type) :
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
    setType(type);
    reset();
  }

  Value::Value(const bool& v) :
      _type(BOOL),
#if CEA_VALUE_FAST_MODE == 0
          _value(0),
#endif
          _boolValue(v),
#if CEA_VALUE_FAST_MODE == 0
          _strValue(0),
#endif
          _autoChangeType(false)
  {
    ;
  }

  Value::Value(const char* v) :
      _type(UNKNOWN),
#if CEA_VALUE_FAST_MODE == 0
          _value(0),
#endif
          _boolValue(false), _autoChangeType(false)
  {
    setType(STRING);
    set(v);
  }

  Value::Value(const std::string& v) :
      _type(STRING),
#if CEA_VALUE_FAST_MODE == 0
          _value(0),
#endif
          _boolValue(false), _autoChangeType(false)
  {
#if CEA_VALUE_FAST_MODE == 0
    _strValue = new std::string(v);
#else
    _strValue.assign(v);
#endif

  }
  Value::Value(const Value& v) :
      _type(v._type),
#if CEA_VALUE_FAST_MODE == 0
          _value(0),
#endif
          _boolValue(v._boolValue),
#if CEA_VALUE_FAST_MODE == 0
          _strValue(0),
#endif
          _autoChangeType(false)
  {
    switch (_type)
      {
    case BOOL:
      break;
    case STRING:
#if CEA_VALUE_FAST_MODE == 0
      if (v._strValue != 0)
        {
          _strValue = new std::string(*(v._strValue));
        }
      else
        {
          _strValue = 0;
        }
#else
      _strValue.assign(v._strValue);
#endif
      break;
    case INT:
      copyValue<Int>(v);
      break;
    case UINT:
      copyValue<UInt>(v);
      break;
    case DOUBLE:
      copyValue<Double>(v);
      break;
    default:
      ;
      };
  }

  /** Destructor */
#if CEA_VALUE_FAST_MODE == 0
  Value::~Value()
  {
    if (_value != 0)
      delete[] _value;
    if (_strValue != 0)
      delete _strValue;
  }
#endif

  /** Auto type change property */
  bool
  Value::getAutoChangeType() const
  {
    return _autoChangeType;
  }
  void
  Value::setAutoChangeType(bool autoChangeType)
  {
    _autoChangeType = autoChangeType;
  }

  /** Reset default value */
  void
  Value::reset()
  {
    switch (_type)
      {
    case UNKNOWN:
      /* Nothing */
      break;
    case BOOL:
      /* Default = false */
      _boolValue = false;
      break;
    case INT:
      /* Default = 0 */
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          *(Int*) (_value) = 0;
        }
      break;
    case UINT:
      /* Default = 0 */
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          *(UInt*) (_value) = 0;
        }
      break;
    case DOUBLE:
      /* Default = 0.0 */
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          *(Double*) (_value) = 0.0;
        }
      break;
    case STRING:
      /* Default = "" */
#if CEA_VALUE_FAST_MODE == 0
      if (_strValue != 0)
        {
          _strValue->clear();
        }
#else
      _strValue.clear();
#endif
      }
  }

  /** Type property */
  Value::Type
  Value::getType() const
  {
    return _type;
  }
  void
  Value::setType(Value::Type newType)
  {
    if (_type != newType)
      {
#if CEA_VALUE_FAST_MODE == 0
        if (newType == STRING)
          {
            /* Erase the value memory */
            if (_value != 0)
              {
                delete[] _value;
                _value = 0;
              }
            /* Create the string memory */
            _strValue = new std::string;
          }
        else
          {
            /* Erase the string memory */
            if (_strValue != 0)
              {
                delete _strValue;
                _strValue = 0;
              }
            if (_value != 0)
              {
                delete[] _value;
                _value = 0;
              }
            /* Create the value memory */
            switch (newType)
              {
            case INT:
              _value = new unsigned char[sizeof(Int)];
              break;
            case UINT:
              _value = new unsigned char[sizeof(UInt)];
              break;
            case DOUBLE:
              _value = new unsigned char[sizeof(Double)];
              break;
            default:
              ;
              }
          }
#endif
        /* Store the new type */
        _type = newType;
        /* Reset */
        reset();
      }
  }

  /** Return the Value::Type from any base type */
  Value::Type
  Value::getValueType(const bool& t)
  {
    return BOOL;
  }
  Value::Type
  Value::getValueType(const signed short int& t)
  {
    return INT;
  }
  Value::Type
  Value::getValueType(const unsigned short int& t)
  {
    return UINT;
  }
  Value::Type
  Value::getValueType(const signed int& t)
  {
    return INT;
  }
  Value::Type
  Value::getValueType(const unsigned int& t)
  {
    return UINT;
  }
  Value::Type
  Value::getValueType(const signed long int& t)
  {
    return INT;
  }
  Value::Type
  Value::getValueType(const unsigned long int& t)
  {
    return UINT;
  }
  Value::Type
  Value::getValueType(const signed long long int& t)
  {
    return INT;
  }
  Value::Type
  Value::getValueType(const unsigned long long int& t)
  {
    return UINT;
  }
  Value::Type
  Value::getValueType(const float& t)
  {
    return DOUBLE;
  }
  Value::Type
  Value::getValueType(const double& t)
  {
    return DOUBLE;
  }
  Value::Type
  Value::getValueType(const std::string& t)
  {
    return STRING;
  }
  Value::Type
  Value::getValueType(const Value& v)
  {
    return v._type;
  }

  /** operator copy */
  Value&
  Value::operator=(const Value& v)
  {
    if (&v != this)
      {
        /* Set the type */
        if (_autoChangeType)
          setType(v._type);
        /* Set the value */
        switch (_type)
          {
        case BOOL:
          _boolValue = v._boolValue;
          break;
        case INT:
#if CEA_VALUE_FAST_MODE == 0
          if ((_value != 0) && (v._value != 0))
#endif
            {
              *(Int*) (_value) = *(Int*) (v._value);
            }
          break;
        case UINT:
#if CEA_VALUE_FAST_MODE == 0
          if ((_value != 0) && (v._value != 0))
#endif
            {
              *(UInt*) (_value) = *(UInt*) (v._value);
            }
          break;
        case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
          if ((_value != 0) && (v._value != 0))
#endif
            {
              *(Double*) (_value) = *(Double*) (v._value);
            }
          break;
        case STRING:
#if CEA_VALUE_FAST_MODE == 0
          if ((_strValue != 0) && (v._strValue != 0))
            {
              _strValue->assign(*v._strValue);
            }
#else
          _strValue.assign(v._strValue);
#endif

          break;
        default:
          ;
          }
      }
    /* Return the current Object */
    return *this;
  }

  /** +toString */
  std::string
  Value::toString() const
  {
    std::string tmp;
    get<std::string>(tmp);
    return tmp;
  }

  /** +compare */
  signed short int
  Value::compare(const Value& v) const
  {
    switch (_type)
      {
    case BOOL:
      return compareValue(_boolValue, v);
    case INT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value == 0)
        return -1;
#endif
      return compareValue(*(Int*) (_value), v);
    case UINT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value == 0)
        return -1;
#endif
      return compareValue(*(UInt*) (_value), v);
    case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
      if (_value == 0)
        return -1;
#endif
      return compareValue(*(Double*) (_value), v);
    case STRING:
#if CEA_VALUE_FAST_MODE == 0
      if (_strValue == 0)
        return -1;
      return compareValue(*_strValue, v);
#else
      return compareValue(_strValue,v);
#endif
    default:
      return -1;
      }
  }

  /** +get */
  template<>
    bool
    Value::get<bool>(bool& retValue) const
    {
      switch (_type)
        {
      case BOOL:
        retValue = _boolValue;
        return true;
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (*(Int*) (_value) == 1);
            return true;
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (*(UInt*) (_value) == 1);
            return true;
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            retValue = (*(Double*) (_value) == 1);
            return true;
          }
        break;
      case STRING:

#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
          {
            retValue = ((_strValue->length() > 0) && ((*_strValue)[0] == '1'));
            return true;
          }
#else
        retValue = ((_strValue.length() > 0) && (_strValue[0]=='1'));
#endif
        break;
      default:
        ;
        }
      return false;
    }
  template<>
    bool
    Value::get<std::string>(std::string& retValue) const
    {
      switch (_type)
        {
      case BOOL:
        if (_boolValue)
          retValue.assign("1");
        else
          retValue.assign("0");
        return true;
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::ostringstream oss;
            oss << (*(Int*) (_value));
            retValue.assign(oss.str());
            return true;
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::ostringstream oss;
            oss << (*(UInt*) (_value));
            retValue.assign(oss.str());
            return true;
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::ostringstream oss;
            oss << (*(Double*) (_value));
            retValue.assign(oss.str());
            return true;
          }
        break;
      case STRING:
#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
#endif
          {
#if CEA_VALUE_FAST_MODE == 0
            retValue.assign(*_strValue);
#else
            retValue.assign(_strValue);
#endif
            return true;
          }
        break;
      default:
        ;
        }
      return false;
    }

  /** +set */
  template<>
    void
    Value::set<bool>(const bool& v)
    {
      /* Change the type of the variable if autochange activate */
      if (_autoChangeType)
        {
          setType(Value::getValueType(v));
        }
      /* Set the Value function of the current type of the value */
      switch (_type)
        {
      case BOOL:
        _boolValue = v;
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(Int*) (_value) = (v ? 1 : 0);
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(UInt*) (_value) = (v ? 1 : 0);
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            *(Double*) (_value) = (v ? 1 : 0);
          }
        break;
      case STRING:
#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
          _strValue->assign((v ? "1" : "0"));
#else
        _strValue.assign((v ? "1" : "0"));
#endif
        break;
      default:
        ;
        }
    }
  template<>
    void
    Value::set<std::string>(const std::string& v)
    {
      /* Change the type of the variable if autochange activate */
      if (_autoChangeType)
        {
          setType(Value::getValueType(v));
        }
      /* Set the Value function of the current type of the value */
      switch (_type)
        {
      case BOOL:
        _boolValue = ((v.length() == 1) && (v[0] == '1'));
        break;
      case INT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::stringstream oss;
            oss << v;
            oss >> (*(Int*) (_value));
          }
        break;
      case UINT:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::stringstream oss;
            oss << v;
            oss >> (*(UInt*) (_value));
          }
        break;
      case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
        if (_value != 0)
#endif
          {
            std::stringstream oss;
            oss << v;
            oss >> (*(Double*) (_value));
          }
        break;
      case STRING:
#if CEA_VALUE_FAST_MODE == 0
        if (_strValue != 0)
          _strValue->assign(v);
#else
        _strValue.assign(v);
#endif
        break;
      default:
        ;
        }
    }
  void
  Value::set(const char* v)
  {
    /* Change the type of the variable if autochange activate */
    if (_autoChangeType)
      {
        setType(STRING);
      }
    /* Convert the const char* to string */
    std::string tmp;
    const char* c = v;
    while ((c != 0) && (*c != '\0'))
      {
        tmp.append(c, 1);
        c++;
      }
    /* Set the Value function of the current type of the value */
    this->set<std::string>(tmp);
  }
  void
  Value::set(const Value& v)
  {
    *this = v;
  }

  /** std::ostream::+operator<< */
  std::ostream&
  operator<<(std::ostream& oss, const Value& v)
  {
    switch (v._type)
      {
    case Value::BOOL:
      oss << v._boolValue;
      break;
    case Value::INT:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          oss << (*(Value::Int*) (v._value));
        }
      break;
    case Value::UINT:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          oss << (*(Value::UInt*) (v._value));
        }
      break;
    case Value::DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          oss << (*(Value::Double*) (v._value));
        }
      break;
    case Value::STRING:
#if CEA_VALUE_FAST_MODE == 0
      if (v._strValue != 0)
        oss << (*(v._strValue));
#else
      oss << v._strValue;
#endif
      break;
    default:
      ;
      }
    return oss;
  }

  /** std::istream::+operator>> */
  std::istream&
  operator>>(std::istream& iss, Value& v)
  {
    switch (v._type)
      {
    case Value::BOOL:
      iss >> v._boolValue;
      break;
    case Value::INT:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          iss >> (*(Value::Int*) (v._value));
        }
      break;
    case Value::UINT:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          iss >> (*(Value::UInt*) (v._value));
        }
      break;
    case Value::DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
      if (v._value != 0)
#endif
        {
          iss >> (*(Value::Double*) (v._value));
        }
      break;
    case Value::STRING:
#if CEA_VALUE_FAST_MODE == 0
      if (v._strValue != 0)
        iss >> (*(v._strValue));
#else
      iss >> v._strValue;
#endif
      break;
    default:
      ;
      }
    return iss;
  }

  /** +operator== */
  bool
  Value::operator==(const Value& v)
  {
    return (compare(v) == 0);
  }
  /** +operator!= */
  bool
  Value::operator!=(const Value& v)
  {
    return (compare(v) != 0);
  }
  /** operator< */
  bool
  Value::operator<(const Value& v)
  {
    return (compare(v) < 0);
  }
  /** +operator<= */
  bool
  Value::operator<=(const Value& v)
  {
    return (compare(v) <= 0);
  }
  /** +operator> */
  bool
  Value::operator>(const Value& v)
  {
    return (compare(v) > 0);
  }
  /** +operator>= */
  bool
  Value::operator>=(const Value& v)
  {
    return (compare(v) >= 0);
  }

  /** +operator-= */
  Value&
  Value::operator-=(const Value& v)
  {
    /* Sub the value */
    switch (_type)
      {
    case UNKNOWN:
    case BOOL:
    case STRING:
      break;
    case INT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          Int tmp;
          v.get(tmp);
          /* Set the value */
          *(Int*) (_value) -= tmp;
        }
      break;
    case UINT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          UInt tmp;
          v.get(tmp);
          /* Set the value */
          *(UInt*) (_value) -= tmp;
        }
      break;
    case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          Double tmp;
          v.get(tmp);
          /* Set the value */
          *(Double*) (_value) -= tmp;
        }
      break;
    default:
      ;
      }
    /* Return the object */
    return *this;
  }

  /** +operator+= */
  Value&
  Value::operator+=(const Value& v)
  {
    /* Sub the value */
    switch (_type)
      {
    case UNKNOWN:
    case BOOL:
    case STRING:
      break;
    case INT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          Int tmp;
          v.get(tmp);
          /* Set the value */
          *(Int*) (_value) += tmp;
        }
      break;
    case UINT:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          UInt tmp;
          v.get(tmp);
          /* Set the value */
          *(UInt*) (_value) += tmp;
        }
      break;
    case DOUBLE:
#if CEA_VALUE_FAST_MODE == 0
      if (_value != 0)
#endif
        {
          /* Get the parameter value */
          Double tmp;
          v.get(tmp);
          /* Set the value */
          *(Double*) (_value) += tmp;
        }
      break;
      }
    /* Return the object */
    return *this;

  }

  /** +compareValue */
  template<>
    inline int
    Value::compareValue<bool>(const bool& v, const Value& withValue) const
    {
      if ((_type != BOOL))
        return -1;
      if (v == withValue._boolValue)
        return 0;
      if (v)
        return 1;
      return -1;
    }
  template<>
    inline int
    Value::compareValue<std::string>(const std::string& v,
        const Value& withValue) const
    {
#if CEA_VALUE_FAST_MODE == 0
      /* Check if comparaison needed */
      if ((_type != STRING) || (v.empty()))
        return 1;
      if ((withValue._strValue == 0) || (withValue._strValue->empty()))
        {
          return -1;
        }
      /* Set the compare option */
      const char* s1 = v.c_str();
      const char* s2 = withValue._strValue->c_str();
      unsigned int len1 = v.length();
      unsigned int len2 = withValue._strValue->length();
      unsigned int n = (len1 <= len2 ? len1 : len2);
#else
      /* Check if comparaison needed */
      if ((_type != STRING) || (v.empty())) return 1;
      if (withValue._strValue.empty())
        {
          return -1;
        }
      /* Set the compare option */
      const char* s1 = v.c_str();
      const char* s2 = withValue._strValue.c_str();
      unsigned int len1 = v.length();
      unsigned int len2 = withValue._strValue.length();
      unsigned int n = (len1 <= len2 ? len1 : len2);
#endif
      /* Do the ignore case comparaison */
      unsigned char c1, c2;
      while (n-- != 0)
        {
          c1 = (((*s1 >= 'a') && (*s1 <= 'z')) ? (*s1 - 32) : *s1);
          c2 = (((*s2 >= 'a') && (*s2 <= 'z')) ? (*s2 - 32) : *s2);
          if (c1 < c2)
            return -1;
          if (c1 > c2)
            return 1;
          ++s1;
          ++s2;
        }
      if (len1 > len2)
        {
          return 1;
        }
      else if (len1 < len2)
        {
          return -1;
        }
      else
        {
          return 0;
        }
    }

}
