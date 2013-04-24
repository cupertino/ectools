//============================================================================
// Name        : Sensor.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0.1
// Date        : 2012.06.01
// Copyright   : 2012, CoolEmAll (INFSO-ICT-288701)
// Description : This is an abstract class of sensors.
//============================================================================

#include <typeinfo>
#include <libec/sensor/Sensor.h>
#include <libec/tools/Tools.h>
#include <libec/tools/XMLReader.h>

namespace cea
{
  const char* Sensor::ClassName = "Sensor";

  Sensor::Sensor(suseconds_t latency)
  {
    clean();
    _latency = latency;
  }

  Sensor::Sensor(const std::string &xmlTag)
  {
    clean();
    setParamsXml(xmlTag.c_str());
  }

  Sensor::Sensor(const Sensor &s)
  {
    clean();
    copy(s);
  }

  Sensor::~Sensor()
  {
    clean();
  }

  std::ostream&
  operator<<(std::ostream &out, Sensor &cPoint)
  {
    out << "sensor name: " << cPoint._name << "\n";
    out << "  alias: " << cPoint._alias << "\n";
    out << "  status: ";
    if (cPoint._isActive)
      out << "active\n";
    else
      out << "error (make sure you are executing as root user)\n";
    out << "  type: ";
    switch (cPoint._type)
      {
    case U64:
      out << "U64\n";
      break;
    case Float:
      out << "Float\n";
      break;
    default:
      out << "Unknown";
      break;
      }
    out << "  latency: " << cPoint._latency << "(ms)\n";

    return out;
  }

  Sensor&
  Sensor::operator=(const Sensor& s)
  {
    if (this != &s)
      {
        clean();
        copy(s);
      }
    return *this;
  }

  void
  Sensor::setParamsXml(const char* sensorXmlTag)
  {
    XMLReader::readAttributeFromTag(sensorXmlTag, "sensor", "name", _name);
    XMLReader::readAttributeFromTag(sensorXmlTag, "sensor", "alias", _alias);
    XMLReader::readSingleValuedTag(sensorXmlTag, "latency", _latency);
  }

  const char*
  Sensor::getClassName()
  {
    return ClassName;
  }

  std::string
  Sensor::getParamsXml(const char* indentation)
  {
    std::string str("");
    return str;
  }

  std::string
  Sensor::toXml()
  {
    std::stringstream ss;
    ss << "<sensor";
    ss << " class=\"" << getClassName() << "\"";
//    ss << " typeid=\"" << typeid(this).name() << "\"";
    ss << " name=\"" << _name << "\"";
    ss << " alias=\"" << _alias << "\"";
    ss << ">" << std::endl;
    ss << "  <params>" << std::endl;
    ss << "    <latency value=\"" << _latency << "\"/>" << std::endl;
    ss << getParamsXml("    ");
    ss << "  </params>" << std::endl;
    ss << "</sensor>" << std::endl;

    return (ss.str());
  }

  std::string
  Sensor::getAlias()
  {
    return _alias;
  }

  std::string
  Sensor::getName()
  {
    return _name;
  }

  void
  Sensor::setName(const char* s)
  {
    _name = s;
  }

  bool
  Sensor::getStatus() const
  {
    return _isActive;
  }

  SensorType
  Sensor::getType() const
  {
    return _type;
  }

  sensor_t
  Sensor::getValue()
  {
    return _cValue;
  }

  bool
  Sensor::needUpdate(const struct timeval &tv_now,
      const struct timeval &tv_prev)
  {
    return (Tools::timevaldiff(tv_prev, tv_now) > _latency);
  }

  void
  Sensor::clean()
  {
    _alias = _name = "";
    _latency = 0;
    _isActive = false;
    _type = Unknown;
    _cTime = _pTime = 0;
    _cTimeval.tv_sec = _cTimeval.tv_usec = 0;
//    _cTime = _pTime = time(NULL);
//    gettimeofday(&_cTimeval, NULL);
  }

  void
  Sensor::copy(const Sensor &source)
  {
    _alias = source._alias;
    _name = source._name;
    _latency = source._latency;
    _isActive = source._isActive;
    _type = source._type;
    _cValue = source._cValue;
    _cTime = source._cTime;
    _pTime = source._pTime;
    _cTimeval = source._cTimeval;
  }
}
