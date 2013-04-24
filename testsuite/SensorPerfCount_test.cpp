#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/sensor/SensorPerfCount.h>
#include <libec/process.h>

int
main()
{
  cea::PerfCount pc(0, 0, -1);

//  if (pc.getStatus())
//    {
//      //remove dependency between ProcessEnumerator and the view.
//      cea::LinuxProcessEnumerator pe;
//
//      pe.update();
//      std::map<pid_t, cea::Process*> m = pe.getAllProcesses();
//
//      for (std::map<pid_t, cea::Process*>::iterator it = m.begin();
//          it != m.end(); it++)
//        {
//          cea::DebugLog::cout << (int) (*(*it).second).getPid() << "\n";
//          usleep(5000);
//        }
//    }

  if (pc.getStatus())
    {
      DIR* dir = opendir("/proc");
      struct dirent *dir_ent = NULL; // "/proc" directory entity
      int counter = 0;
      float total_share = 0.0f;
      cea::u64 val;

      /* Get all the numeric dir (corresponding to the pid) */
      while ((dir != NULL) && ((dir_ent = readdir(dir)) != NULL))
        {
          if ((dir_ent->d_type == DT_DIR)
              && (cea::Tools::isNumeric(dir_ent->d_name)))
            {
              pid_t pid;
              std::stringstream ss;
              ss << dir_ent->d_name;
              ss >> pid;
              val = pc.getValuePid(pid).U64;
            }
        }
      pc.getValue().U64;
      sleep(1);
      closedir(dir);

      dir = opendir("/proc");
      /* Get all the numeric dir (corresponding to the pid) */
      while ((dir != NULL) && ((dir_ent = readdir(dir)) != NULL))
        {
          if (cea::Tools::isNumeric(dir_ent->d_name))
            {
              pid_t pid;
              std::stringstream ss;
              ss << dir_ent->d_name;
              ss >> pid;
              val = pc.getValuePid(pid).U64;
              total_share += val;
              counter++;
              std::cout << "pid: " << pid << " val: " << val << "\n";
            }
        }
      closedir(dir);
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
