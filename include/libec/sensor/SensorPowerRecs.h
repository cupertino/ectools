//============================================================================
// Name        : SensorPowerRecs.h
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
// Description : RECS Power Sensor
//============================================================================

#ifndef LIBEC_SENSOR_POWER_RECS_H_
#define LIBEC_SENSOR_POWER_RECS_H_

#include "SensorPower.h"

#include <netinet/in.h>
#include <netdb.h>

namespace cea
{
#define LG_MAX_IP 15
#define NB_MAX_NODES 18

  /// \brief Christmann's RECS2(r) sensors
  ///
  /// Christmann's RECS2 18 nodes cluster has an extremely high packing density,
  /// it is composed by one backbone and up to 18 modules with one node per
  /// module in a single rack unity. The RECS2(r) backbone comes with several
  /// sensors (such as thermal and power sensors placed for each module).
  /// Each RECS2(r) module is composed by a CPU socket, memory and a fan.
  ///
  /// This class implements TCP socket connection to the RECS2 microcontroller
  /// to retrieve the temperature and power of each node. One must be aware
  /// that RECS2(r) microcontroller supports just one socket connection at a time
  /// if more than one request is made at the same time, a connection error event
  /// will take place.
  class RecsPowerMeter : public PowerMeter
  {
  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Constructor
    /// \param serv_ip Microcontroller's internet protocol. Default vaue is 10.11.12.253
    /// \param serv_port Microcontroller's port. Default value is 10001
    /// \param node_id RECS2 node identifier (from 1 to 18)
    RecsPowerMeter(const char* serv_ip = "10.11.12.253", int serv_port = 10001,
        int node_id = 0);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load the sensor
    RecsPowerMeter(const std::string &xmlTag);

    ~RecsPowerMeter();

    void
    update();

    sensor_t
    getValue();

    /// Get sensor's value
    /// \param nodeId RECS2 node identifier (from 1 to 18)
    /// \param sensorType 0 - Power; 1 - Temperature
    short
    getValue(int nodeId, int sensorType);

    /// \brief Get's the name of the class
    const char* getClassName();

    /// \brief Returns all parameters as a XML string
    /// \param indentation Indentation for each output line break
    std::string
    getParamsXml(const char* indentation = "");

    /// \brief Read the parameters from a XML file and set their values
    /// \param xmlTag  XML tag where the parameters may be found
    void
    setParamsXml(const char* xmlTag);

  private:
    struct node
    {
      bool has_board; /// < Flag to determine if the board is plugged in the slot
      bool status; /// < Flag to determine if the node is turned on or off
      short temp; /// < Node's temperature in degrees Celsius (°C)
      short power; /// < Node's power in Watts (W)
    };

    /// Open socket connection with RECS2 microcontroller
    bool
    openConnection();

    /// Close socket connection with RECS2 microcontroller
    bool
    closeConnection();

    char _servIp[LG_MAX_IP + 1]; /// < server's ip address in the form xxx.xxx.xxx.xxx
    int _servPort; /// < RECS microcontroller port number
    int _sockFd; /// < socket's file descriptor
    int _nodeId; /// < REC's node id
    struct sockaddr_in _servAddr;
    struct hostent *_server;
    struct node _nodes[NB_MAX_NODES];

    short nr_baseboards; /// < Total number of baseboards available

    float global_voltage;

    sensor_t _pValue;
  };
}

#undef LG_MAX_IP
#undef NB_MAX_NODES

#endif /* SENSOR_POWER_PDU_H_ */
