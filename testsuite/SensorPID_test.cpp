#include <iostream>
#include <pthread.h>
#include <math.h>

#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPerfCount.h>

// to compile:
// [wp5-t5.2] g++ -Lbin/libec.a -Iinclude -I/usr/src/linux-cea/tools/perf/ -lm -O0 testsuite/SensorPID_test.cpp bin/libec.a

int
main()
{
  cea::PIDSensor* sensor;

//  sensor = new cea::PerfCount(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
  sensor = new cea::PerfCount(0, 0);
  if (sensor->getStatus())
    {
      std::cout << sensor->getName() << std::endl;
      std::cout << "----------------------------" << std::endl;

      while (true)
        {
          std::cout << sensor->getValuePid(getpid()).U64 << std::endl;
          sleep(1);
        }
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;
}
