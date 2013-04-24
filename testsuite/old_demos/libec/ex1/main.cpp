// demos/ex1/main.cpp
//
// how to compile:
// $ cd lib_demos/ex1
// $ g++ -L../../bin/ -I../../include -I/usr/src/linux-cea/tools/perf/ -lm -O0 MySensor.cpp main.cpp -lec
//
#include "MySensor.h"

#include <ctime>

int
main()
{
  cea::PIDSensor* sensor;
  pid_t pid;

  sensor = new MySensor();

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
          sleep(1);
        }
      std::cout << "----------------------------" << std::endl;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
