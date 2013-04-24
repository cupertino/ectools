///////////////////////////////////////////////////////////////////////////////
/// @file               SensorProcNet.h
/// @author             Leandro Fontoura Cupertino
/// @version            0.1
/// @date               Sep 19 2012
/// @copyright          2012, CoolEmAll (INFSO-ICT-288701)
/// @brief              Networking information from /proc/net/dev file
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_SENSOR_NETWORK_H_
#define LIBEC_SENSOR_NETWORK_H_

#include "Sensor.h"

namespace cea
{
  class Network : public Sensor
  {

  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// \brief Identify the type of sensor to be returned through getValue()
    /// calls.
    enum TypeId
    {
      /// Total number of bytes sent by the Node on the last timestep
      SendBytes = 0,
      /// Total number of packets sent by the Node on the last timestep
      SendPkt = 1,
      /// Total number of bytes received by the Node on the last timestep
      ReceiveBytes = 2,
      /// Total number of packets received by the Node on the last timestep
      ReceivePkt = 3,
      /// Maximum number of networking types
      TYPE_MAX = 4
    };

    /// \brief Constructor
    /// \param netType  Network type to be returned by getValue() function
    Network(TypeId netType = SendPkt);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load the sensor
    Network(const std::string &xmlTag);

    ~Network();

    void
    update();

    /// \brief Get machine's network activity (sent/received packages/bytes)
    /// \return Network activity
    sensor_t
    getValue();

    /// \brief Get's the name of the class
    const char*
    getClassName();

  protected:
    static u64 _cValue[4]; ///< current values from the 4 available sensors (Send/Transmit package/bytes).
    static u64 _pValue[4]; ///< previous values from the 4 available sensors (Send/Transmit package/bytes).
    static time_t _sTime; ///< static time to update all the sensors at once.

    /// Checks wheather the sensor is active or not
    /// \returns 1 if active, 0 otherwise
    bool
    checkActivity();

    std::string
    getParamsXml(const char* indentation);

    void
    setParamsXml(const char* xmlTag);

    unsigned short _netType; ///< network sensor type to be returned by getValue() function
  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///     @class cea::ProcNet
///     @ingroup sensor
///////////////////////////////////////////////////////////////////////////////
