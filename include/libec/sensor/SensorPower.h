//============================================================================
// Name        : SensorPower.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
// Description : Power Sensor available on the G5K plataform
//============================================================================

#ifndef SENSOR_POWER_H_
#define SENSOR_POWER_H_

#include "Sensor.h"

namespace cea
{

  class PowerMeter : public Sensor
  {
  public:
    /// Constructor
    PowerMeter();

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    PowerMeter(const std::string &xmlTag);

    /// Gets the powermeter's latency.
    /// This method may not work for all powermeters. For low precision tools
    /// the latency must be overloaded. Some vendors make available their
    /// average latency.
    virtual unsigned
    getLatency();

    /// Waits until the value of the power meter changes (update detection)
    void
    waitUpdate();

  protected:
    void
    clean();
  };

}

#endif /* SENSOR_POWER_H_ */
