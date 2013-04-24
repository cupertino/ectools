#include <iostream>
#include <fstream>
#include <libec/sensor/SensorPowerAcpi.h>
#include <libec/tools/Tools.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  const char* AcpiPowerMeter::ClassName = "AcpiPowerMeter";

  // Public methods
  AcpiPowerMeter::AcpiPowerMeter()
  {
    _name = "ACPI_POWER_METER";
    _alias = "PM_ACPI";

    _cValue.Float = 0.0;

    // Voltage in uV (micro volts):
    _fpVoltage = "/sys/class/power_supply/BAT0/voltage_now";
    // Current in uA (micro amperes):
    _fpCurrent = "/sys/class/power_supply/BAT0/current_now";
    // Battery state
    _fpState = "/sys/class/power_supply/BAT0/status";

    _type = Float;
    _isActive = checkActivity();
  }

  AcpiPowerMeter::AcpiPowerMeter(const std::string &xmlTag) :
      PowerMeter(xmlTag)
  {
    // Voltage in uV (micro volts):
    _fpVoltage = "/sys/class/power_supply/BAT0/voltage_now";
    // Current in uA (micro amperes):
    _fpCurrent = "/sys/class/power_supply/BAT0/current_now";
    // Battery state
    _fpState = "/sys/class/power_supply/BAT0/status";

    _type = Float;
    _isActive = checkActivity();
  }

  bool
  AcpiPowerMeter::checkActivity()
  {
    bool val;

    // Check if files containing the current voltage and current
    // have reading permissions
    val = (access(_fpVoltage, R_OK) == 0);
    val &= (access(_fpCurrent, R_OK) == 0);

    return val;
  }

  AcpiPowerMeter::~AcpiPowerMeter()
  {
  }

  void
  AcpiPowerMeter::update()
  {
    unsigned long int c, v;

    _pTime = _cTime;
    _cTime = time(NULL);

    if (getState() == Discharging)
      {
        _input.open(_fpCurrent, std::ifstream::in);
        _input >> c;
        _input.close();

        _input.open(_fpVoltage, std::ifstream::in);
        _input >> v;
        _input.close();

        _cValue.Float = c * v * (1.0e-12f);
      }
    else
      _cValue.Float = 0.0f;

  }

//  unsigned int
//  AcpiPowerMeter::getLatency()
//  {
//    unsigned int time;
//    float curr, prev;
//
//    time = 0;
//    curr = prev = getValue().Float;
//    while (curr == prev)
//      {
//        curr = getValue().Float;
//        usleep(10000);
//      }
//
//    prev = curr;
//    while (curr == prev)
//      {
//        usleep(10000);
//        curr = getValue().Float;
//        time += 10;
//      }
//    return time;
//  }

  char
  AcpiPowerMeter::getState()
  {
    char c;

    _input.open(_fpState, std::ifstream::in);
    _input >> c;
    _input.close();

    switch (c)
      {
    case 'F':
      return Full;
    case 'D':
      return Discharging;
    case 'C':
      return Charging;
    case 'U':
    default:
      return Unknown;
      }
  }

  sensor_t
  AcpiPowerMeter::getValue()
  {
    return _cValue;
  }

  inline const char*
  AcpiPowerMeter::getClassName()
  {
    return ClassName;
  }

}
