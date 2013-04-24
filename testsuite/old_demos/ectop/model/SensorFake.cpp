#include "../headers/SensorFake.h"

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
  FakeSensor::getValue(pid_t pid)
  {
    update(pid);
    return _cValue;
  }

  void
  FakeSensor::update(pid_t pid)
  {
    if (_type == U64)
      _cValue.U64 = pid * Tools::rnd(1, 10);
    else
      _cValue.Float = (float) pid / Tools::rnd(1, 10);
  }
}
