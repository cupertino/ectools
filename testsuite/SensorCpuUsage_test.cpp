/*
 * SensorCpuUsage_test.cpp
 *
 *  Created on: Aug 15, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/sensors.h>

int
main()
{
  cea::SensorCpuUsage s;
  pid_t pid;

  pid = getpid();

  if (s.getStatus())
    {
      s.add(pid);

      while (1)
        {
          std::cout << s.getValuePid(pid).Float << std::endl;
          sleep(1);
        }
    }

  return 0;
}
