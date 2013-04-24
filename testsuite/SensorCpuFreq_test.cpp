/*
 * SensorCpuFrequency_test.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/tools/DebugLog.h> // if the debug log comes after the sensors.h we have compilations issues. need to check why.
#include <libec/sensors.h>

int
main()
{
  cea::CpuFreq* sensor;
  std::string cpuinfo_str;
  std::stringstream ss;
  unsigned long long value;
  unsigned long long expected;

  cea::DebugLog::create();
  cea::DebugLog::clear();

  std::cout << "Sensor test: CPU Frequency" << std::endl;

  unsigned short cpuid = 0;

  sensor = new cea::CpuFreq(cpuid);

  if (sensor->getStatus())
    {
      while (1)
        {
          ss.clear();
          ss.str("");

          //update sensor and get reference value
          sensor->update();
          ss << "awk '/cpu MHz/ { ++x; if ( x == " << (cpuid + 1)
              << " ) print $4 }' /proc/cpuinfo";

          cpuinfo_str = cea::Tools::exec(ss.str().c_str());
          ss.clear();
          ss.str("");
          //convert string into a double
          ss << cpuinfo_str;
          double tmp;
          ss >> tmp;

          expected = tmp * 1000;

          value = sensor->getValue().U64;

          if (value == expected)
            std::cout << "Test PASSED" << std::endl;
          else
            {
              std::cout << "Test FAILED!!" << std::endl;
              std::cout << "Expected value: " << expected << std::endl;
              std::cout << "Returned value: " << value << std::endl;
            }
        }
    }

  delete sensor;

  return 0;
}
