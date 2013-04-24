// demos/ex1/MySensor.cpp
#include "MySensor.h"
#include <libec/tools/Tools.h>

MySensor::MySensor()
{
  _name = "MySensor";
  _alias = "MS";
  _type = cea::U64;
  _isActive = true;
}

cea::sensor_t
MySensor::getValue(pid_t pid)
{
  update(pid);
  return _cValue;
}

void
MySensor::update(pid_t pid)
{
  _cValue.U64 = pid * cea::Tools::rnd(1, 10);
}
