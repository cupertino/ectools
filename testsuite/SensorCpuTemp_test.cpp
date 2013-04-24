/*
 * SensorCpuUsage_test.cpp
 *
 *  Created on: Aug 15, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/sensor/SensorCpuTemp.h>
#include <libec/sensor/SensorController.h>

int
main()
{
  cea::CpuTemp temp;
  cea::CpuTemp temp0(0);
  cea::CpuTemp temp1(1);

  if (temp.getStatus())
    {
      temp.update();
      std::cout << "CPU die temperature:   " << temp.getValue().Float
          << std::endl;
    }
  if (temp0.getStatus())
    {
      temp0.update();
      std::cout << "CPU0 Temperature (dC): " << temp0.getValue().Float
          << std::endl;
    }
  if (temp1.getStatus())
    {
      temp1.update();
      std::cout << "CPU1 Temperature (dC): " << temp1.getValue().Float
          << std::endl;
    }

  // Test Sensor Controller for CpuTemp
  cea::Sensor *s;

  cea::SensorController::storeXML(temp1, "/tmp/cpuTemp.xml");
  s = cea::SensorController::loadXmlFile("/tmp/cpuTemp.xml");
  if (s != NULL)
    {
      std::cout << s->toXml();
      std::cout << s;

      delete s;
    }
  else
    std::cout << "Null pointer" << std::endl;

  return 0;
}
