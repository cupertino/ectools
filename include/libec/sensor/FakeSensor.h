///////////////////////////////////////////////////////////////////////////////
/// @file               FakeSensor.h
/// @author             Dorian RODDE
/// @version	        0.1
/// @date               2012.07
/// @copyright	        2012, CoolEmAll (INFSO-ICT-288701)
/// @brief              Fake sensor used for Monitor test
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_FAKESENSOR_H__
#define LIBEC_FAKESENSOR_H__

#include "../tools/Tools.h"
#include "SensorPid.h"

#include <string>

namespace cea
{
  /// @brief Fake sensor used for Monitor test
  class FakeSensor : public PIDSensor
  {
  public:
    /// @brief Constructor: Set name, alias and type of Sensor
    FakeSensor(SensorType type = U64);

    /// @brief Simulate value for a pid (pid * rnd(1,10))
    /// @param pid Process Identifier
    /// @return Sensor U64 rand value
    sensor_t
    getValue();

    sensor_t
    getValuePid(pid_t pid);

    /// @brief Update Sensor State
    void
    update();

    void
    updatePid(pid_t pid);
  };
}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::FakeSensor
///	@ingroup sensor
///
///////////////////////////////////////////////////////////////////////////////
