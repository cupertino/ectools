// demos/ex1/MySensor.h
#ifndef MYSENSOR_H__
#define MYSENSOR_H__

#include <libec/sensor/SensorPid.h>

class MySensor : public cea::PIDSensor
{
public:
  MySensor();

  cea::sensor_t
  getValue(pid_t pid);

  void
  update(pid_t pid);
};

#endif
