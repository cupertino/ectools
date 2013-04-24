//============================================================================
// Name        : Sensor.h
// Author      : Dorian Rodde, Leandro Fontoura Cupertino
// Version     : 0.2
// Date        : 2012.06.01
// Copyright   : 2012, CoolEmAll (INFSO-ICT-288701)
// Description : This is an abstract class of sensors.
//============================================================================

#ifndef LIBEC_SENSOR_H__
#define LIBEC_SENSOR_H__

#include <iostream>
#include <sys/time.h>

#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include "../Globals.h"

namespace cea
{
  /// \brief      Sensor abstract class.
  ///
  /// \details
  /// The Sensor class is an abstract class to represent an instrument
  /// which measures some physical quantity.
  ///
  /// \author     Leandro Fontoura Cupertino
  /// \date       Jul 1 2012
  class Sensor
  {

  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Constructor
    /// \param _latency Sensor's latency (minimum time between updates, in milliseconds)
    Sensor(suseconds_t latency = 0);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    Sensor(const std::string &xmlTag);

    /// Copy constructor
    Sensor(const Sensor& s);

    /// Destructor
    virtual
    ~Sensor();

    /// Updates sensor's state.
    virtual void
    update() = 0;

    /// Returns a XML string with the description of the sensor
    std::string
    toXml();

    /// Gets sensor's class name
    /// \return Sensor's class name
    virtual const char*
    getClassName();

    /// Gets sensor's short name (alias).
    /// \return Sensor's short name
    std::string
    getAlias();

    /// Gets sensor's full name.
    /// \return Sensor's full name
    std::string
    getName();

    /// Sets sensor's full name.
    /// \param name Sensor's full name
    void
    setName(const char* name);

    /// Gets sensor's status flag indicating if the sensor is active or not in
    /// the current hardware.
    /// \return 1 if sensor is active
    bool
    getStatus() const;

    /// Gets sensor's last update time.
    /// \return Sensor's last update time
    time_t
    getTime() const;

    /// Gets sensor's type (U64 or Float).
    /// \return Sensor's type
    SensorType
    getType() const;

    /// Gets sensor's current value (for the entire system).
    virtual sensor_t
    getValue();

    /// \brief Returns all parameters as a XML string
    /// \param indentation Indentation for each output line break
    virtual std::string
    getParamsXml(const char* indentation = "");

    /// \brief Read the parameters from a XML file and set their values
    /// \param xmlTag  XML tag where the parameters may be found
    virtual void
    setParamsXml(const char* sensorXmlTag);

    /// Overloads output operator<<
    friend std::ostream&
    operator<<(std::ostream &out, Sensor &cPoint);

    /// Assignment operator
    Sensor&
    operator=(const Sensor& s);

  protected:
    /// Clean all sensor's variable to its default ones
    void
    clean();

    /// Copy a sensor
    /// \param source  Sensor to be copied
    void
    copy(const Sensor &source);

    /// Determines if the elapsed time is greater than the sensor's latency
    bool
    needUpdate(const struct timeval &tv_now, const struct timeval &tv_prev);

    /// Short name (alias)
    std::string _alias;

    /// Full name
    std::string _name;

    /// Type
    SensorType _type;

    /// Status
    bool _isActive;

    /// Current value
    sensor_t _cValue;

    /// Time when the previous value was collected
    struct timeval _cTimeval;

    time_t _cTime;
    time_t _pTime;

    /// Sensor latency in milliseconds (ms)
    long _latency;
  };

}

#endif
