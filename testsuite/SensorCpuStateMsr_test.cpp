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
  cea::CpuStateMsr* sensor;
  std::string cpuinfo_str;
  std::stringstream ss;

  cea::DebugLog::create();
  cea::DebugLog::clear();

  cea::SystemInfo::init();

  std::cout << "Sensor test: CPU Frequency" << std::endl;

  unsigned short cpuid = 0;

  sensor = new cea::CpuStateMsr(cpuid);

  if (sensor->getStatus())
    {
      cea::DebugLog::write("Status ok");
      while (1)
        {
          //update sensor and get reference value
          sensor->update();
          std::cout << time(NULL) << " " << sensor->getValue().Float
              << std::endl;
          sleep(1);
        }
    }

  delete sensor;

  return 0;
}
