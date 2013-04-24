//============================================================================
// Name        : daq.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.01
// Copyright   : Your copyright notice
// Description : This is a data acquisition tool. It requests a set of
//               performance indicators during the execution of a predefined
//               benchmark for further analysis.
//============================================================================

#include <string.h>
#include <stdlib.h>

#include <libec/DataAcquisition.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensors.h>
#include <libec/estimators.h>

void
usageMessage()
{
  std::cout << "Usage: ecdaq [OPTION] BENCHMARK" << std::endl;
  std::cout << "Collects energy consumption related data"
      << " while executing a BENCHMARK script." << std::endl << std::endl;
  std::cout << "  -o <file_path>             " << "output file path"
      << std::endl;
  std::cout << "  -i <time>                  "
      << "idle time (in seconds) to collect data before "
      << "and after the execution of a benchmark." << std::endl << std::endl;
}

int
main(int argc, char *argv[])
{
  char* bench;
  std::string outfile = "ecdaq.log";
  int idleTime = 5;

  cea::SystemInfo::countProc();

  if (argc == 1)
    {
      usageMessage();
      exit(1);
    }

  if (!strcmp(argv[1], "--help"))
    {
      usageMessage();
      exit(0);
    }

  bench = argv[argc - 1];

  for (int i = 1; i < argc; i += 2)
    if (!strcmp(argv[i], "-o"))
      outfile = argv[i + 1];

  for (int i = 1; i < argc; i += 2)
    if (!strcmp(argv[i], "-i"))
      idleTime = atoi(argv[i + 1]);

  //Run the data acquisition in verbose mode
  cea::SystemInfo::init();

  std::cout << "Data Acquisition ... [start]\n";
  cea::DataAcquisition daq(bench, idleTime, 1.0, outfile);

  /*
  //Collect data from specific sensors
  daq.addSensor(new cea::PidStat(cea::PidStat::CPU_USAGE));
  daq.addSensor(new cea::CpuFrequency(0));
  daq.addSensor(new cea::CpuFrequency(1));
  daq.addSensor(new cea::AcpiPowerMeter());
  */

  daq.collectData();
  std::cout << "Data Acquisition ... [done]\n";

  return 0;
}
