#include <typeinfo>
#include <iostream>
#include <pthread.h>
#include <math.h>

#include <libec/sensor/Sensor.h>
#include <libec/sensor/SensorCpuFreq.h>

using namespace cea;
using namespace std;

void
copy(Sensor& sensor, Sensor& copy)
{
  cout << "void copy(Sensor& sensor, Sensor& copy)" << endl;
  cout << "sensor.getName()=" << sensor.getName() << endl;
  cout << "copy = sensor;" << endl;
  copy = sensor;
  cout << "copy.setName(\"New name\");" << endl;
  copy.setName("New name");
  cout << "sensor.getName()=" << sensor.getName() << endl;
  cout << "copy.getName()=" << copy.getName() << endl;
}

void
copy(Sensor* sensor, Sensor* copy)
{
  cout << "void copy(Sensor* sensor, Sensor* copy)" << endl;
  cout << "sensor->getName()=" << sensor->getName() << endl;
  cout << "copy = sensor;" << endl;
  copy = sensor;
  cout << "copy->setName(\"New name\");" << endl;
  copy->setName("New name");
  cout << "sensor->getName()=" << sensor->getName() << endl;
  cout << "copy->getName()=" << copy->getName() << endl;
}

int
main()
{
  Sensor* sensorPtr;
  Sensor* copyPtr;

  sensorPtr = new CpuFreq(0);
  copyPtr = new CpuFreq(((CpuFreq&) (*sensorPtr)));

  copy(*sensorPtr, *copyPtr);
  copy(sensorPtr, copyPtr);

  delete sensorPtr;
  delete copyPtr;

  cea::CpuFreq cf, cfc;
  cfc = cf;

  std::cout << cf.getName() << std::endl;
  cfc.setName("New name");
  std::cout << cf.getName() << std::endl;
  std::cout << cfc.getName() << std::endl;
  cfc = cf;
  std::cout << cf.getName() << std::endl;
  std::cout << cfc.getName() << std::endl;


  return 0;
}
