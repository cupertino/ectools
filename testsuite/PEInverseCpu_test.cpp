/*
 * SensorCpuFrequency_test.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/tools/DebugLog.h> // if the debug log comes after the sensors.h we have compilations issues. need to check why.
#include <libec/sensors.h>
#include <libec/estimators.h>

int
main()
{
//  cea::DebugLog::create();
//  cea::DebugLog::clear();

  std::cout << "Power estimator test: Power Meter x CPU share" << std::endl;

  // Estimator's input variables
  cea::AcpiPowerMeter pm;
  cea::CpuElapsedTime ct;

  cea::InverseCpu pe(&pm, &ct, 10); // Power estimator

  if (pe.getStatus())
    {
      pid_t pid;
      pid = getpid();

      // the first value of the update is not ok because it is time dependent
      pe.updatePid(pid);
      usleep(10000);

      for (int i = 0; i < 10; i++)
        {
//          float f = pe.getValuePid(pid).Float;
//          std::cout << f << std::endl;

          usleep(100000);
        }
    }

  return 0;
}
