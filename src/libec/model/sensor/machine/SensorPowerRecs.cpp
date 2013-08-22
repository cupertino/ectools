#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <libec/sensor/SensorPowerRecs.h>
#include <libec/tools/Tools.h>
#include <libec/tools/XMLReader.h>
#include <libec/tools/DebugLog.h>

#define NB_MAX_NODES 18

namespace cea
{
  const char* RecsPowerMeter::ClassName = "RecsPowerMeter";
  static const char *COM_ASK = "4x";

  // Public methods
  RecsPowerMeter::RecsPowerMeter(const char* servIp, int servPort, int nodeId)
  {
    _name = "RECS_POWER_METER";
    _alias = "PM_RECS" + Tools::CStr(nodeId);
    _nodeId = nodeId;
    strcpy(_servIp, servIp);
    _servPort = servPort;

    for (int i = 0; i < NB_MAX_NODES; i++)
      {
        _nodes[i].has_board = false;
        _nodes[i].power = 0;
        _nodes[i].status = false;
        _nodes[i].temp = 0;
      }

    _cValue.Float = 0.0;
    _pValue.Float = 0.0;
    _type = Float;
    _isActive = openConnection();
    closeConnection();
  }

  RecsPowerMeter::RecsPowerMeter(const std::string &xmlTag) :
      PowerMeter(xmlTag)
  {
    setParamsXml(xmlTag.c_str());

    _cValue.Float = 0.0;
    _pValue.Float = 0.0;
    _type = Float;
    _isActive = openConnection();
    closeConnection();
  }

  RecsPowerMeter::~RecsPowerMeter()
  {
    close(_sockFd);
  }

  void
  RecsPowerMeter::update()
  {
    char buffer[1024];
    int n;

    if (openConnection())
      {
        n = write(_sockFd, COM_ASK, strlen(COM_ASK));
        if (n < 0)
          {
            _isActive = false; // error: writing to socket
            closeConnection();
            return;
          }

        char* dest = buffer;
        while (true)
          {
            n = read(_sockFd, dest, 1024);
            if (n < 0)
              {
                // error: reading from socket
                closeConnection();
                return;
              }
            dest[n] = '\0';
            if (strstr(buffer, "xxx"))
              break;
            dest = dest + n;
          }
        closeConnection();

        std::istringstream in(buffer);

        char C;
        in >> nr_baseboards >> C;
#if DEBUG
        DebugLog::cout << "Number of baseboards: " << nr_baseboards << "\n";
#endif

        for (int i = 0; i < nr_baseboards; i++)
          {
            in >> _nodes[i].has_board >> C >> _nodes[i].status >> C
                >> _nodes[i].temp >> C >> _nodes[i].power >> C;

#if DEBUG
            DebugLog::cout << "Baseboard " << i + 1 << DebugLog::endl;
            DebugLog::cout << "  Baseboard present: "
                << ((_nodes[i].has_board) ? "yes" : "no") << DebugLog::endl;
            DebugLog::cout << "  Status: "
                << ((_nodes[i].status) ? "on" : "off") << DebugLog::endl;
            DebugLog::cout << "  Temperature: " << _nodes[i].temp
                << DebugLog::endl;
            DebugLog::cout << "  Power: " << _nodes[i].power << DebugLog::endl;
#endif
          }

        // Total RECS2(r) voltage
        char Reste[128];
        in >> global_voltage >> Reste;
        if (!strcmp(Reste, "xxxx"))
          return;

        //sscanf(buffer, "%fxxx", &global_voltage);
        global_voltage = global_voltage / 16.45; // Correcting the voltage-factor
        //global_voltage = round(global_voltage, 2);
        //printf("Global voltage: %f\n", global_voltage);

        //printf("\n%s\n", buffer);
      }
  }

  bool
  RecsPowerMeter::openConnection()
  {
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockFd < 0) // error: opening socket
      return false;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(_sockFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
      return false; // error: could not set timeout value

    _server = gethostbyname(_servIp);
    if (_server == NULL) // error: no such host
      return false;

    bzero((char *) &_servAddr, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    bcopy((char *) _server->h_addr, (char *)&_servAddr.sin_addr.s_addr, _server->h_length);
    _servAddr.sin_port = htons(_servPort);

    if (connect(_sockFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) < 0)
      return false; // error: connecting

    return true;
  }

  bool
  RecsPowerMeter::closeConnection()
  {
    close(_sockFd);

    return true;
  }

  sensor_t
  RecsPowerMeter::getValue()
  {
    _cValue.Float = _nodes[_nodeId].power;
    return _cValue;
  }

  short
  RecsPowerMeter::getValue(int nodeId, int typeId)
  {
    if (typeId == 0)
      return _nodes[nodeId].power;
    else
      return _nodes[nodeId].temp;
  }

  inline const char*
  RecsPowerMeter::getClassName()
  {
    return ClassName;
  }

  std::string
  RecsPowerMeter::getParamsXml(const char* indentation)
  {
    std::stringstream ss;
    ss << indentation << "<serv_ip value=\"" << _servIp << "\"/>" << std::endl;
    ss << indentation << "<serv_port value=\"" << _servPort << "\"/>"
        << std::endl;
    ss << indentation << "<node_id value=\"" << _nodeId << "\"/>" << std::endl;
    return ss.str();
  }

  void
  RecsPowerMeter::setParamsXml(const char* xmlTag)
  {
    XMLReader::readSingleValuedTag(xmlTag, "serv_ip", _servIp);
    XMLReader::readSingleValuedTag(xmlTag, "serv_port", _servPort);
    XMLReader::readSingleValuedTag(xmlTag, "node_id", _nodeId);
  }

}

#undef NB_MAX_NODES
