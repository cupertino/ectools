#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorController.h>
#include <libec/sensor/SensorPowerAcpi.h>
#include <libec/process/linux/LinuxProcessEnumerator.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))

int
main()
{
  cea::DebugLog::create();
  cea::DebugLog::clear();

  cea::AcpiPowerMeter pm;

  std::cout << "Testing class:        " << pm.ClassName << std::endl;
  std::cout << "  Class size (bytes): " << sizeof(pm) << std::endl;
  std::cout << "  Status:             ";
  if (pm.getStatus())
    {
      std::cout << "Active" << std::endl;
      char state = pm.getState();
      std::cout << "  State:              ";
      switch (state)
        {
      case cea::AcpiPowerMeter::Charging:
        std::cout << "Charging";
        break;
      case cea::AcpiPowerMeter::Discharging:
        std::cout << "Discharging";
        break;
      case cea::AcpiPowerMeter::Full:
        std::cout << "Full";
        break;
      default:
        std::cout << "Unknown";
        break;
        }
      std::cout << std::endl;

      std::cout << "  Value (W):          " << pm.getValue().Float << std::endl;
      std::cout << "  Latency (ms):       " << pm.getLatency() << std::endl;
    }
  else
    std::cout << "Inactive" << std::endl;

  if (pm.getStatus())
    {
      unsigned long i, v;
      float expected, actual;
      double error;

      error = 0;

      for (int id = 0; id < 10; id++)
        {
          cea::Tools::CStrTo(
              cea::Tools::exec("cat /sys/class/power_supply/BAT0/current_now"),
              i);
          cea::Tools::CStrTo(
              cea::Tools::exec("cat /sys/class/power_supply/BAT0/voltage_now"),
              v);
          expected = v * i * (1.0e-12f);

          actual = pm.getValue().Float;

          error += ABS(expected - actual);

          usleep(10000);
        }

      if (error == 0)
        std::cout << "PASSED" << std::endl;
      else
        std::cout << "FAILED!! error: " << error << std::endl;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  cea::SensorController::storeXML(pm, "/tmp/acpi.xml");

  cea::Sensor* s;
  s = cea::SensorController::loadXmlFile("/tmp/acpi.xml");
  if (s != NULL)
    std::cout << s->toXml();

  return 0;
}
