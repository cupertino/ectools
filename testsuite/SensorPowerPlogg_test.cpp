#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorController.h>
#include <libec/sensor/SensorPowerPlogg.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))

int
main()
{
  cea::DebugLog::create();
  cea::DebugLog::clear();

  cea::PloggPowerMeter ppm;
  if (ppm.getStatus())
    {
      for (int i = 0; i < 10; i++)
        {
          ppm.update();
          std::cout << ppm.getValue().Float << std::endl;
          usleep(100000);
        }
    }
  else
    std::cout << "Sensor not available. Check the log file for errors."
        << std::endl;

  return 0;
}
