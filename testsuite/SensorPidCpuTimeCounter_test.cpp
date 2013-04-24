#include <libec/tools.h>
#include <libec/sensors.h>

#include <dirent.h>

int
main()
{
  cea::PIDSensor* sensor;

  struct dirent *dir;
  int proc_count;

  cea::DebugLog::create();
  cea::DebugLog::clear();

  sensor = new cea::CpuTime();

  if (sensor->getStatus())
    {
      while (1)
        {
          proc_count = 0;
          // Open proc dir
          DIR* proc = opendir("/proc");


          // Get all the numeric dir (corresponding to the pid)
          while ((dir = readdir(proc)) != NULL)
            {
              if (cea::Tools::isNumeric(dir->d_name))
                {
                  pid_t pid;
                  std::stringstream ss;
                  ss << dir->d_name;
                  ss >> pid;
                  if (sensor->getValue(pid).U64 > 0)
                    proc_count++;
                }
            }
          std::cout << "Number of running process on the last timestep is: "
              << proc_count << std::endl;
          sleep(1);
        }
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;
}
