/*
 valgreen - Application's energy profiling tool.
 Copyright (C) 2012 by Leandro Fontoura Cupertino, IRIT
 Author: Leandro Fontoura Cupertino <fontoura@irit.fr>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/wait.h>

#include <libec/sensors.h>
#include <libec/estimators.h>
#include <libec/tools/DebugLog.h>
#include <libec/DataAcquisition.h>

#include "DPELRCpuProcs.h"

#define LG_LOG_ARG 11

void
usageMessage()
{
  std::cout << "Usage: valgreen [option] progs-and-args" << std::endl;
  std::cout << "Collects energy consumption of an application" << std::endl
      << std::endl;
  std::cout << "  --log-file=<file>          " << "output file path"
      << std::endl;
  std::cout << "  --calibrate                " << "calibrate the power model"
      << std::endl << std::endl;
}

void*
benchmark(void* data)
{
  system("stress --cpu 1 --timeout 60");
  sleep(20);
  system("stress --cpu 2 --timeout 60");
  sleep(20);
  system("stress --vm 2 --timeout 60");
  sleep(20);
  system("stress --io 2 --timeout 60");
  sleep(20);
  system("stress --cpu 2 --vm 2 --io 2 --timeout 60");

  return (void*) 0;
}

int
main(int argc, char *argv[])
{
  pid_t pid;
  char log[128];
  unsigned int used_args;

  strcpy(log, "valgreen.log");

  cea::DebugLog::create(log);
  cea::DebugLog::clear();

  // Wattmeter
  cea::AcpiPowerMeter pm;
  cea::DebugLog::writeMsg(cea::DebugLog::INFO, "main()",
      "ACPI power meter created.");

  // Initial values
  double w[3] =
    { -1.13, 17.16, 32.60 };

  // Power estimator
  cea::DPELRCpuProcs pe(pm, w);
  cea::DebugLog::writeMsg(cea::DebugLog::INFO, "main()",
      "DPELRCpuProcs power estimator created.");

  used_args = 1;

  if ((argc == 1) || (!strcmp(argv[1], "--help")))
    {
      usageMessage();

      return 0;
    }

  if ((!strcmp(argv[1], "--calibrate")))
    {
      cea::DebugLog::writeMsg(cea::DebugLog::WARNING, "calibration process",
          "Please close all other applications during the calibration");
      cea::DebugLog::writeMsg(cea::DebugLog::WARNING, "calibration process",
          "The notebook must be kept unplugged during the entire calibration.");

      pthread_t thread;
      // Runs the benchmark in a new thread
      if (pthread_create(&thread, NULL, benchmark, NULL) == -1)
        {
          cea::DebugLog::writeMsg(cea::DebugLog::ERROR, "model calibration",
              "The benchmark execution thread could not be created.");
          exit(1);
        }
      pe.getLatency();
//      pe.collectData(400);
      pe.collectData(100);
      pe.calibrate();

      std::ofstream ofs("valgreen.cfg");
      ofs << pe;
      ofs.close();

      pthread_cancel(thread);

      return 0;
    }

  for (int i = 1; i < argc; i++)
    {
      if (!strncmp(argv[i], "--log-file=", LG_LOG_ARG))
        {
          strcpy(log, (argv[i] + LG_LOG_ARG));
          used_args++;
          std::cout << "log=" << log;
          cea::DebugLog::create(log);
          cea::DebugLog::clear();
        }
    }

  //read information stored into config file
  std::ifstream ifs("valgreen.cfg");
  if (ifs.good())
    {
      ifs >> pe;
    }
  ifs.close();

  std::cout << pe << std::endl;

  pid = fork();
  switch (pid)
    {
  case -1:
    return 1;
    break;
  case 0:
    //Execute arguments
    execvp(argv[used_args], argv + used_args);
    return 0;
  default:
    //Monitor Pid
    if (pe.getStatus())
      {
        cea::DebugLog::writeMsg(cea::DebugLog::INFO, "power estimator status",
            "ok");
        int status;
        pid_t tpid;
        double power;
        double energy;
        unsigned int period = 200; //milliseconds

        energy = 0;

        time_t start;
        start = time(NULL);
        do
          {
            usleep(period * 1000);

            tpid = waitpid(pid, &status, WNOHANG);

            pe.updatePid(pid);
            power = pe.getValuePid(pid).Float;

            energy += power * period / 1000; //from milliseconds to seconds
          }
        while (tpid != pid);

        if (energy > 1000)
          std::cout << " energy (kJ): " << energy / 1000 << std::endl;
        else
          std::cout << " energy (J): " << energy << std::endl;

        std::cout << " time (s):   " << time(NULL) - start << std::endl;
        break;
      }
    else
      cea::DebugLog::writeMsg(cea::DebugLog::ERROR, "power estimator status",
          "fail");

    std::cout << pid << std::endl;
    return 0;
    }

  return 1;
}
