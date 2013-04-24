#include <libec/sensor/FakeSensor.h>

namespace cea
{
  FakeSensor::FakeSensor(SensorType type)
  {
    _type = type;

    if (_type == U64)
      {
        _name = "FakeSensor_U64";
        _alias = "FS_U";
      }
    else
      {
        _name = "FakeSensor_Float";
        _alias = "FS_F";
      }

    _isActive = true;
  }

  sensor_t
  FakeSensor::getValue()
  {
    return _cValue;
  }

  sensor_t
  FakeSensor::getValuePid(pid_t pid)
  {
    updatePid(pid);
    return _cValue;
  }

  void
  FakeSensor::update()
  {
    if (_type == U64)
      _cValue.U64 = Tools::rnd(1, 10);
    else
      _cValue.Float = (float) 1 / Tools::rnd(1, 10);
  }

  void
  FakeSensor::updatePid(pid_t pid)
  {
    if (_type == U64)
      _cValue.U64 = pid * Tools::rnd(1, 10);
    else
      _cValue.Float = (float) pid / Tools::rnd(1, 10);
  }

}
