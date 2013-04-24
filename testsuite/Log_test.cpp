#include <ctime>
#include <libec/logs.h>
#include <libec/sensors.h>

#ifdef __FD_SETSIZE
#undef __FD_SETSIZE
#endif

#define __FD_SETSIZE  65535

int
main()
{

  cea::PIDSensor* sensor;

  cea::Log log;

  std::cout << __FD_SETSIZE << std::endl;

  log.set(cea::FileLog("data.log", true), cea::CVSFormat());
  log.setFlushFrequency(5000);

  //  sensor = new cea::PerfCount(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
  sensor = new cea::PerfCount(0, 0);
  if (sensor->getStatus())
    {
      log.addComment("name\t" + sensor->getName());

      while (true)
        {
          log.openBlock("item");
          log.write(time(NULL));
          log.write("s", sensor->getValue(getpid()).U64);
          log.write(sensor->getValue(getpid()).U64);
          log.write(sensor->getValue(getpid()).U64);
          log.write(sensor->getValue(getpid()).U64);
          log.closeBlock("item");
          log.update();
          std::cout << "1\n";
          sleep(1);
        }
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
