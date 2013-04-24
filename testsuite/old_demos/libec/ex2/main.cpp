// demos/ex2/main.cpp
//
// to compile:
// $ cd lib_demos/ex1
// $ g++ -L../../bin/libec.a -I../../include -I/usr/src/linux-cea/tools/perf/ -lm -O0 MySensor.cpp main.cpp ../../bin/libec.a
//
#include "MySensor.h"

#include <ctime>
#include <cmath>

int
main()
{
  cea::PIDSensor* sensor;
  pid_t pid;

  sensor = new MySensor();

  double load = 0.5f;

  // check if the sensor was properly started
  if (sensor->getStatus())
    {
      pid = getpid();
      std::cout << "----------------------------" << std::endl;
      std::cout << "Name: " << sensor->getName() << std::endl;
      std::cout << "Alias: " << sensor->getAlias() << std::endl;
      for (int i = 0; i < 10; i++)
        {
          std::cout << "pid: " << pid << " \tticks: " << time(NULL)
              << " \tvalue:" << sensor->getValue(pid).U64 << std::endl;
          for (int j = 1; j < 10000000; j++)
            {
              load = (100.1 * sin(load) / log(j));
            }
          sleep(1);
        }
      std::cout << "----------------------------" << std::endl;
      std::cout << load << std::endl;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
