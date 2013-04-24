#include <iostream>
#include <fstream>
#include <math.h>

#include <libec/sensors.h>
#include <libec/estimators.h>

int
main()
{
  cea::PidStat *cpu;
  cea::PowerEstimator *pe;

  cpu = new cea::PidStat(cea::PidStat::CPU_USAGE);
  pe = new cea::MinMaxCpu(22, 55, cpu);

  if (pe->getStatus())
    {
      while (true)
        {
          std::ofstream ofs("/var/tmp/ecdaq-ganglia.out");
          ofs << pe->getValue(-1).Float << std::endl;
          ofs.close();

//          std::cout << "pe: " << pe->getValue(-1).Float << "\tcpu: "
//              << cpu->getValue(-1).Float << std::endl;

          sleep(2);
        }
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;
}
