#include <iostream>
#include <libec/tools/DebugLog.h>
#include <libec/sensors.h>

int
main()
{
  cea::RunningProcs s0(0);
  cea::RunningProcs s1(1);

  cea::SensorController::storeXML(s1, "/tmp/running_procs.xml");
  cea::SensorController::storeXML(s0, "/tmp/running_procs.xml", 'A');

  cea::Sensor *s;
  s = cea::SensorController::loadXmlFile("/tmp/running_procs.xml");

  if (s != NULL)
    std::cout << s->toXml();

  delete s;

  return 0;
}
