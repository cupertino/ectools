#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/device/SystemInfo.h>
#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidStat.h>
#include <libec/process/linux/LinuxProcessEnumerator.h>

// to compile:
// [wp5-t5.2] g++ -Lbin/libec.a -Iinclude -I/usr/src/linux-cea/tools/perf/ -lm -O0 testsuite/SensorPID_test.cpp bin/libec.a

int
main()
{
  cea::PIDSensor* cpu_share;
  cea::PIDSensor* sensor1;
  double load;
  pid_t pid;

  cea::DebugLog::create();
  cea::DebugLog::clear();

  // TESTING CPU SHARE
  cpu_share = new cea::PidStat(cea::PidStat::CPU_USAGE);
  sensor1 = new cea::PidStat(cea::PidStat::VIRTUAL_MEM_SIZE);

  sleep(1);

  // Check if the CPU share is being correctly calculated
  // This consist in verifying if the sum of all process share
  // is the same as the total CPU share
  if (cpu_share->getStatus())
    {
      struct dirent *dir;
      /* Open proc dir */
      DIR* proc = opendir("/proc");
      int counter = 0;
      float total_share = 0.0f;
      float val;

      /* Get all the numeric dir (corresponding to the pid) */
      while ((dir = readdir(proc)) != NULL)
        {
          if (cea::Tools::isNumeric(dir->d_name))
            {
              pid_t pid ;
              std::stringstream ss;
              ss << dir->d_name;
              ss >> pid;
              val = cpu_share->getValue(pid).Float;
            }
        }
      cpu_share->getValue(-1).Float;
      sleep(1);

      proc = opendir("/proc");
      /* Get all the numeric dir (corresponding to the pid) */
      while ((dir = readdir(proc)) != NULL)
        {
          if (cea::Tools::isNumeric(dir->d_name))
            {
              pid_t pid ;
              std::stringstream ss;
              ss << dir->d_name;
              ss >> pid;
              val = cpu_share->getValue(pid).Float;
              total_share += val;
              counter++;
              std::cout << "pid: " << pid << " val: " << val << "\n";
            }
        }
      std::cout << "CPU share test ";
      float expected_share = cpu_share->getValue(-1).Float;
      if (expected_share == total_share)
        std::cout << "[passed]\n";
      else
        std::cout << "[FAILED]\n";
      std::cout << "number of processes: " << counter << std::endl;
      std::cout << "expected value:      " << expected_share << std::endl;
      std::cout << "computed value:      " << total_share << std::endl;
    }

  load = 0.5f;

  // check if the sensor was properly started
  if ((cpu_share->getStatus()) && (sensor1->getStatus()))
    {
      pid = getpid();
      std::cout << "----------------------------" << std::endl;
      std::cout << "pid \t ticks \t " << cpu_share->getAlias() << " \t "
          << sensor1->getAlias() << std::endl;
      for (int i = 0; i < 10; i++)
        {
          std::cout << pid << " \t " << time(NULL) << " \t "
              << cpu_share->getValue(pid).Float << " \t "
              << sensor1->getValue(pid).U64 << std::endl;
          for (int j = 1; j < 10000000; j++)
            {
              load = (100.1 * sin(load) / log(j));
            }
          sleep(1);
        }
      std::cout << "----------------------------" << std::endl;
      return load;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
