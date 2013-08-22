//============================================================================
// Author      : Leandro Fontoura Cupertino
// Date        : 2013.08.19
// Copyright   : IRIT
// Description : This demo presents how a user can calibrate a dynamic sensor
//               and use it to log the power dissipated by the machine during
//               the execution of an application.
//============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <libec/tools/DebugLog.h>
#include <libec/device/SystemInfo.h>
#include <libec/DataAcquisition.h>
#include <libec/sensors.h>
#include <libec/estimator/DPELRCpu.h>

void
usageMessage(char *cmd)
{
  printf("Usage:\n");
  printf("  %s [OPTION] APPLICATION\n", cmd);
  std::cout << "Collects energy consumption related data"
      << " while executing a BENCHMARK script." << std::endl << std::endl;
  printf("Options:\n");
  printf(
      "  --logfile <file_path>     Log file with the execution information.\n");
  printf(
      "  --datafile <file_path>    CSV file containing the data collected while running the application.\n");
  printf(
      "  --calibrate               Calibrate the sensor based on the stress benchmark.\n");
}

void*
benchmark(void* data)
{
  int ncpus, i = 1;
  char buff[128];

  ncpus = sysconf(_SC_NPROCESSORS_CONF);

  for (i = 1; i <= ncpus; i++)
    {
      sprintf(buff, "stress --cpu %d --timeout 30", i);
      system(buff);
      sleep(10);
      sprintf(buff, "stress --vm %d --timeout 30", i);
      system(buff);
      sleep(10);
      sprintf(buff, "stress --io %d --timeout 30", i);
      system(buff);
      sleep(10);
    }
  sprintf(buff, "stress --cpu %d --vm %d --io %d --timeout 30", ncpus, ncpus,
      ncpus);
  system(buff);
  sleep(10);

  return (void*) 0;
}

int
main(int argc, char *argv[])
{
  char* bench;
  std::string outfile, csvfile;
  cea::DPELinearRegression* dpelr;

  outfile = "ecdaq_log.txt";
  csvfile = "ecdaq_data.csv";

  // Load system information
  cea::SystemInfo::init();

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

  for (int i = 1; i < argc - 1; i++)
    if (!strcmp(argv[i], "--logfile"))
      outfile = argv[i + 1];

  for (int i = 1; i < argc - 1; i++)
    if (!strcmp(argv[i], "--datafile"))
      csvfile = argv[i + 1];

  for (int i = 1; i < argc; i++)
    {
      if ((!strcmp(argv[i], "--calibrate")))
        { // Sensor Calibration
          cea::PowerMeter* pm;
          pthread_t thread;

          cea::DebugLog::writeMsg(cea::DebugLog::WARNING, "calibration process",
              "Please close all other applications during the calibration");
          cea::DebugLog::writeMsg(cea::DebugLog::WARNING, "calibration process",
              "The notebook must be kept unplugged during the entire calibration.");

          // Set your power meter here.
          //pm = new cea::WattsUpPowerMeter();
          //pm = new cea::PloggPowerMeter();
          //pm = new cea::RecsTlsePowerMeter("141.115.28.11", 11100, 17);
          pm = new cea::AcpiPowerMeter();

          // Set the dynamic power estimator.
          dpelr = new cea::DPELRCpu(*pm);

          // Runs the benchmark in a new thread
          if (pthread_create(&thread, NULL, benchmark, NULL) == -1)
            {
              cea::DebugLog::writeMsg(cea::DebugLog::ERROR, "model calibration",
                  "The benchmark execution thread could not be created.");
              exit(EXIT_FAILURE);
            }
          dpelr->collectData(cea::SystemInfo::getCpuCount() * 120 + 40);
//          dpelr->collectData(120);
          pthread_cancel(thread);

          dpelr->calibrate();

          std::ofstream ofs("sensor.cfg");
          ofs << *dpelr;
          ofs.close();

          exit(EXIT_SUCCESS);
        }
    }

  if (argc > 1)
    { // Data acquisition
      bench = argv[argc - 1];

      dpelr = new cea::DPELRCpu();
      std::cout << *dpelr << std::endl;

      //read information stored into config file
      std::ifstream ifs("sensor.cfg");
      if (ifs.good())
        {
          ifs >> *dpelr;
        }
      ifs.close();
      std::cout << *dpelr << std::endl;

      std::cout << "Data Acquisition ... [start]\n";
      cea::DataAcquisition daq(bench, 0, 1.0, outfile, csvfile);
      daq.addSensor(dpelr);
      daq.collectData();
      std::cout << "Data Acquisition ... [done]\n";
    }
  exit(EXIT_SUCCESS);
}
