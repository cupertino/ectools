#include <cstdio>
#include <iostream>
#include <fstream>

#include <libec/tools.h>
#include <libec/Globals.h>
#include <libec/sensor/SensorNetwork.h>

namespace cea
{
  // Static members
  const char* Network::ClassName = "Network";
  u64 Network::_cValue[4];
  u64 Network::_pValue[4];
  time_t Network::_sTime;

  Network::Network(Network::TypeId netType)
  {
    const std::string name[] =
      { "SEND_BYTES", "SEND_PACKETS", "RECV_BYTES", "RECV_PACKETS" };
    const std::string alias[] =
      { "SB", "SP", "RB", "RP" };

    _name = "NET_" + name[netType];
    _alias = "Net" + alias[netType];
    _netType = netType;

    _type = U64;
    _isActive = checkActivity();
  }

  Network::Network(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
    setParamsXml(xmlTag.c_str());

    _type = U64;
    _isActive = checkActivity();
  }

  Network::~Network()
  {
  }

  inline const char*
  Network::getClassName()
  {
    return ClassName;
  }

  void
  Network::setParamsXml(const char* xmlTag)
  {
    XMLReader::readSingleValuedTag(xmlTag, "type_id", _netType);
  }

  std::string
  Network::getParamsXml(const char* indentation)
  {
    std::stringstream ss;
    ss << indentation << "<type_id value=\"" << _netType << "\"/>" << std::endl;
    return ss.str();
  }

  bool
  Network::checkActivity()
  {
    if (_netType >= TYPE_MAX)
      return false;

    if (access("/proc/net/dev", R_OK) == 0)
      return true;

    return false;
  }

  void
  Network::update()
  {
    time_t time_now = time(NULL);

    if (_sTime != time_now)
      {
        _pTime = _cTime;
        _cTime = time_now;
        _sTime = time_now;

        _pValue[0] = _cValue[0]; // Receive bytes
        _pValue[1] = _cValue[1]; // Receive packets
        _pValue[2] = _cValue[2]; // Transmit bytes
        _pValue[3] = _cValue[3]; // Transmit packets

        _cValue[0] = 0; // Receive bytes
        _cValue[1] = 0; // Receive packets
        _cValue[2] = 0; // Transmit bytes
        _cValue[3] = 0; // Transmit packets

        std::ifstream ifs("/proc/net/dev", std::ifstream::in);
        char line[1024];

        // read the first two lines (headers)
        ifs.getline(line, 1024);
        ifs.getline(line, 1024);

        unsigned long d_ulong; //dummy variable
        unsigned long rcv_bytes, rcv_pkt, snt_bytes, snt_pkt;

        // updates the value of each sensor as the sum of the
        // packages/bytes sent/trasmitted from/to each network interface
        while (!ifs.eof())
          {
            ifs.ignore(256, ':');
            if (ifs.eof())
              break;

            ifs >> rcv_bytes;
            ifs >> rcv_pkt;
            ifs >> d_ulong; //ignore value
            ifs >> d_ulong; //ignore value
            ifs >> d_ulong; //ignore value
            ifs >> d_ulong; //ignore value
            ifs >> d_ulong; //ignore value
            ifs >> d_ulong; //ignore value
            ifs >> snt_bytes;
            ifs >> snt_pkt;

            _cValue[0] += rcv_bytes;
            _cValue[1] += rcv_pkt;
            _cValue[2] += snt_bytes;
            _cValue[3] += snt_pkt;
          }

        ifs.close();
      }
  }

  sensor_t
  Network::getValue()
  {
    sensor_t value;
    value.U64 = _cValue[_netType] - _pValue[_netType];
    return value;
  }

}
