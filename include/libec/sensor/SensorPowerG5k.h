//============================================================================
// Name        : SensorPower.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
// Description : PDU Power Sensor available on the G5K plataform.
//               Requires the
//============================================================================

#ifndef SENSOR_POWER_G5KPDU_H_
#define SENSOR_POWER_G5KPDU_H_

#include <map>
#include "SensorPower.h"

namespace cea
{
  struct IpOut
  {
    std::string Ip; ///< PDU's ip address
    unsigned int Outlet; ///< PDU's outlet id

    IpOut(std::string ip = "127.0.0.1", unsigned int outlet = 0);
  };

  /// \brief            Power distribution unit (PDU) sensor.
  /// \author           Leandro Fontoura Cupertino
  /// \date             2012
  /// \copyright        Copyright © 2012, IRIT, CoolEmAll (INFSO-ICT-288701).
  /// \license          GPL v3.0.
  /// \details
  /// Implements a way to collect Grid5000's PDU's information. This sensor
  /// retrieves the average power consumed by the node in the last 3 seconds.
  class G5kPowerMeter : public PowerMeter
  {
  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Constructor
    G5kPowerMeter();

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load the sensor
    G5kPowerMeter(const std::string &xmlTag);

    ~G5kPowerMeter();

    void
    update();

    /// \brief Get's the name of the class
    const char*
    getClassName();

  private:
    bool
    checkActivity();

    // Searches inside a table for the command line to get the power
    // consumption of a given G5K node
    std::string
    getCmdLine(std::string hostname);

    void
    buildHostPduMap();

    // Connection string
    std::string _cmd;

    // Maps the hostname into the PDU's IP and outlet number
    static std::map<std::string, IpOut> _hostPduMap;

    // Flag that keeps the status of the hostPudMap
    static bool _isMapBuilt;

    sensor_t _pValue;
  };
}

#endif /* SENSOR_POWER_G5KPDU_H_ */
