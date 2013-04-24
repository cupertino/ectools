#include <iostream>
#include <pthread.h>
#include <math.h>
#include <libec/tools.h>
#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidStat.h>

int
main()
{
  cea::PIDSensor* sensor;
  std::list<cea::PIDSensor*> sensorList;
  double load;
  pid_t pid;

  // instantiates PidStat sensors
  for (int i = 0; i < cea::PidStat::TYPE_MAX; i++)
    {
      sensor = new cea::PidStat((cea::PidStat::TypeId) i);

      // add the sensor in the list if it was properly started
      if (sensor->getStatus())
        sensorList.push_back(sensor);
      else
        std::cerr << "error: sensor [" << sensor->getName()
            << "] could not be opened." << std::endl;
    }

  // write headers
  std::cout << "#time";
  for (std::list<cea::PIDSensor*>::iterator it = sensorList.begin();
      it != sensorList.end(); it++)
    std::cout << " " << sensor->getAlias();

  // write system wide values
  while (1)
    {
      std::cout << time(NULL);
      for (std::list<cea::PIDSensor*>::iterator it = sensorList.begin();
          it != sensorList.end(); it++)
        std::cout << " " << sensor->getValue(-1);

      sleep(1);
    }

  return 0;
}
