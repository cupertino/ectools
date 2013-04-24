///////////////////////////////////////////////////////////////////////////////
/// \file               SensorCpuTemp.h
/// \author             Mateusz Jarus
/// \author             Leandro Fontoura Cupertino
/// \version	        0.1
/// \date               2012.11
/// \copyright	        2012, Mateusz Jarus
/// \brief              Cpu core temperature sensor
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_CPUTEMP_H__
#define LIBEC_CPUTEMP_H__

#include "../tools/Tools.h"
#include "Sensor.h"

#include <string>

namespace cea
{
  /// \brief CPU core's input temperature
  class CpuTemp : public Sensor
  {

  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// \brief Constructor: Set name, alias and type of Sensor
    /// \param cpuId  CPU's identifier, -1 for entire die (socket) temperature
    CpuTemp(short cpuId = -1);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    CpuTemp(const std::string &xmlTag);

    /// Copy Constructor
    CpuTemp(const CpuTemp &ct);

    /// \brief Destructor
    ~CpuTemp();

    /// \brief Update Sensor State
    void
    update();

    /// \brief Get's CPU's temperature
    /// \return CPU's temperature in degrees Celsius
    sensor_t
    getValue();

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

    /// Assignment operator
    CpuTemp&
    operator=(const CpuTemp& s);

  protected:
    /// Bring all sensor's variable back to its default ones
    virtual void
    clean();

    /// Copies the dource variables into the current sensor
    void
    copy(const CpuTemp &source);

    /// Check's if CPU family is AMDK8
    bool
    checkAmdk8();

    /// Check's if CPU family is Intel
    bool
    checkIntel();

    /// Get CPU core's temperature from a AMDK8 processor
    float
    updateAmdk8();

    /// Get CPU core's temperature from an Intel processor
    float
    updateIntel();

    /// Update method to be used
    float
    (CpuTemp::*updatePtr)(void);

    short _cpuId;

    /// Path to CPU's temperature file
    std::string _filepath;

  };
}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::CpuTemp
///	@ingroup sensor
///
///////////////////////////////////////////////////////////////////////////////
