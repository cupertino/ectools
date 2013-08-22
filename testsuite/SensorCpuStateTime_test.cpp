/*
 * SensorCpuFrequency_test.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/tools/DebugLog.h> // if the debug log comes after the sensors.h we have compilations issues. need to check why.
#include <libec/sensors.h>
#include <libec/device/SystemInfo.h>

int
main()
{
  cea::CpuStateTime* sensor;
  std::string cpuinfo_str;
  std::stringstream ss;

  cea::DebugLog::create();
  cea::DebugLog::clear();

  std::cout << "Sensor test: CPU States" << std::endl;

  unsigned short cpuid = 0;

  sensor = new cea::CpuStateTime(0, cpuid);

  int s = cea::CpuStateTime::getNumberofStates();
  int c = cea::SystemInfo::getCpuCount();
  std::cout << "Available CPUs: " << c << std::endl;
  std::cout << "Available states: " << s << std::endl;
  std::cout << "Name: " << sensor->getName() << std::endl;
  std::cout << "Alias: " << sensor->getAlias() << std::endl;

  if (sensor->getStatus())
    {
      std::cout << "Sensor access OK!" << std::endl;
      while (1)
        {
          ss.clear();
          ss.str("");

          //update sensor and get reference value
          sensor->update();
          std::cout << "Value: " << sensor->getValue().U64 << std::endl;

          sleep(1);
        }
    }

  delete sensor;

  return 0;
}
