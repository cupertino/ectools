/*
 * SensorWattsUp_test.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: adminuser
 */

#include <libec/tools.h>
#include <libec/sensor/SensorPowerWattsUp.h>

int
main()
{
  std::cout << "Starting test" << std::endl;

  cea::DebugLog::create(std::cout, true);
  cea::DebugLog::clear();

  cea::WattsUpPowerMeter wupm;
  if (wupm.getStatus())
  {
    for (int i = 0; i < 10; i++)
    {
      wupm.update();
      std::cout << wupm.getValue().Float << std::endl;
      usleep(100000);
    }
  }
  else
    cea::DebugLog::writeMsg(cea::DebugLog::ERROR, "WattsUpPowerMeter_test",
        "Sensor not available. Check the log file for errors.");

  return 0;
}
