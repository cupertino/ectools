#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/sensor/SensorController.h>
#include <libec/sensor/SensorPowerG5k.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))

int
main()
{
  cea::G5kPowerMeter pm;

  std::cout << "Sensor test: " << pm.getName() << std::endl;

  if (pm.getStatus())
    {
      // implement test
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  cea::SensorController::storeXML(pm, "/tmp/g5k.xml");

  cea::Sensor* s;
  s = cea::SensorController::loadXmlFile("/tmp/g5k.xml");
  if (s != NULL)
    std::cout << s->toXml();

  delete s;

  return 0;
}
