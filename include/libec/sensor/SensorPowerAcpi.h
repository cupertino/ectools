//============================================================================
// Name        : SensorPowerAcpi.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
//============================================================================

#ifndef LIBEC_SENSOR_POWER_ACPI_H_
#define LIBEC_SENSOR_POWER_ACPI_H_

#include <fstream>

#include "SensorPower.h"

namespace cea
{
  /// \brief A power meter based on ACPI information
  /// This PowerMeter uses the actual current and voltage
  /// information from ACPI enabled hardware.
  /// It checks for the reading access of the following files:
  /// - Voltage (v in uV): /sys/class/power_supply/BAT0/voltage_now
  /// - Current (c in uA): /sys/class/power_supply/BAT0/current_now
  ///
  /// The machine's power (P) is then calculated by
  /// \f[ P = c \times v \times 10^{-12}\f]
  class AcpiPowerMeter : public PowerMeter
  {
  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Battery's states
    enum StateId
    {
      Full = 0, Charging = 1, Discharging = 2, Unknown = 3,
    };

    /// Constructor
    AcpiPowerMeter();

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load the sensor
    AcpiPowerMeter(const std::string &xmlTag);

    ~AcpiPowerMeter();

    void
    update();

    sensor_t
    getValue();

    /// Get battery's state
    /// \returns A char representing its StateId
    char
    getState();

    /// \brief Get's the name of the class
    const char*
    getClassName();

  private:
    std::ifstream _input; /// < Input file
    const char* _fpCurrent; /// < Current file path
    const char* _fpVoltage; /// < Voltage file path
    const char* _fpState; /// < Battery's state file path

    /// Check's if the sensor is active
    /// \returns  True if active, false otherwise
    bool
    checkActivity();
  };
}

#endif /* SENSOR_POWER_PDU_H_ */
