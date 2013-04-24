#include <libec/sensors.h>
#include <libec/sensor/SensorController.h>

int
main()
{
  cea::Sensor* s0 = cea::SensorController::createSensor(
      cea::CpuTime::ClassName);
  std::cout << s0->getName() << std::endl;
//  delete (s0);
  cea::SensorController::deleteSensors();

  // test storing sensor
  cea::CpuFreq sensor(0);
  cea::SensorController::storeXML(sensor, "/tmp/sensor.xml");

// test loading sensor
  cea::Sensor *s;
  s = cea::SensorController::loadXmlFile("/tmp/sensor.xml");

  if (s != NULL)
    {
      std::cout << s->toXml();
      std::cout << (cea::CpuFreq&)(*s);

      delete s;
    }

//  cea::SensorController::storeXML(sensor, "/tmp/sensor.xml", 'A');
//  cea::SensorController::storeXML(sensor, "/tmp/sensor.xml", 'A');

  return 0;
}
