#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorPowerRecs.h>
#include <libec/process/linux/LinuxProcessEnumerator.h>

int
main()
{
  cea::DebugLog::create();
  cea::DebugLog::clear();

  // RECS2 Powermeter
  //sensor1 = new cea::RecsPowerMeter("192.168.0.250");
  cea::RecsPowerMeter sensor1("10.11.12.254");

  // Check if the CPU share is being correctly calculated
  // This consist in verifying if the sum of all process share
  // is the same as the total CPU share
  if (sensor1.getStatus())
    {
      while (1)
        {
          for (int id = 0; id < 18; id++)
            std::cout << id << " " << sensor1.getValue(id, 0) << " "
                << sensor1.getValue(id, 1) << std::endl;

          std::cout << std::endl << "Press ENTER to continue...\n";

          getchar();
        }
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  cea::SensorController::storeXML(sensor1, "/tmp/recs.xml");

  cea::Sensor *s;
  s = cea::SensorController::loadXmlFile("/tmp/recs.xml");
  if (s != NULL)
    std::cout << s->toXml();

  delete s;

  return 0;
}
