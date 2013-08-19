//============================================================================
// Author      : Leandro Fontoura Cupertino
// Date        : 2013.08.19
// Copyright   : IRIT
// Description : This is a data acquisition case in which we want to log the
//               estimated power of the entire machine.
//============================================================================

#include <string.h>
#include <stdlib.h>

#include <libec/DataAcquisition.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorPowerPlogg.h>
#include <libec/estimator/DPELRCpu.h>

void
usageMessage(char *cmd)
{
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s [OPTION] BENCHMARK\n", cmd);
  std::cout << "Collects energy consumption related data"
      << " while executing a BENCHMARK script." << std::endl << std::endl;
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -l <file_path>     Log file path.\n");
  fprintf(stderr, "  -c <file_path>     CSV data file path.\n");
  fprintf(stderr,
      "  -i <time>          Idle time (in seconds) to collect data before \n"
          "                     and after the execution of a benchmark.\n");
}

int
main(int argc, char *argv[])
{
  char* bench;
  std::string outfile, csvfile;
  int idleTime;

  outfile = "ecdaq.log";
  csvfile = "ecdaq.csv";
  idleTime = 0;

  if (argc == 1)
    {
      usageMessage(argv[0]);
      exit(EXIT_FAILURE);
    }

  if (!strcmp(argv[1], "--help"))
    {
      usageMessage(argv[0]);
      exit(EXIT_SUCCESS);
    }

  for (int i = 1; i < argc; i += 2)
    if (!strcmp(argv[i], "-l"))
      outfile = argv[i + 1];

  for (int i = 1; i < argc; i += 2)
    if (!strcmp(argv[i], "-c"))
      csvfile = argv[i + 1];

  for (int i = 1; i < argc; i += 2)
    if (!strcmp(argv[i], "-i"))
      idleTime = atoi(argv[i + 1]);

  bench = argv[argc - 1];

  //Run the data acquisition in verbose mode
  cea::SystemInfo::init();
  cea::SystemInfo::countProc();

  std::cout << "Data Acquisition ... [start]\n";
  cea::DataAcquisition daq(bench, idleTime, 1.0, outfile, csvfile);

  cea::Sensor* s;
  cea::PowerMeter* pm;
  pm = new cea::PloggPowerMeter();
  s = new cea::DPELRCpu(*pm);
  daq.addSensor(s);

  daq.collectData();
  std::cout << "Data Acquisition ... [done]\n";

  return 0;
}

