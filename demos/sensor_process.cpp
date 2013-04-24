/*
 ectools demos - Energy Consumption Library
 Copyright (C) 2013 by Leandro Fontoura Cupertino, IRIT
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

///////////////////////////////////////////////////////////////////////////////
// Demo description
//
// This demo describes how the user can extend the PIDSensor class to create
// his own process related sensor. Its important to observe that all process
// level sensors retrieves also the machine level information, this makes this
// kind of sensor bigger.
//
// New machine and application sensors can be easily implemented by
// extending the Sensor and SensorPID classes, respectively. To do so,
// the user must at least overload the update() and updatePid() methods,
// if specific data structures are used, it may be necessary to overload
// the getValue() and getValuePid() methods as well.
//
// This demo was presented at the EE-LSDS 2013, Energy Efficiency in Large
// Scale Distributed Systems conference.
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <libec/sensor/SensorPid.h>
#include <libec/tools/Tools.h>

#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>
#include <unistd.h>

#define PAGES_TO_KB(n)  (unsigned long)( (n) << page_to_kb_shift )
#define KB_TO_PAGES(n)  (unsigned long)( (n) >> page_to_kb_shift )

///////////////////////////////////////////////////////////////////////////////
// MySensor class
///////////////////////////////////////////////////////////////////////////////
class MyMemorySensor : public cea::PIDSensor
{
public:
  MyMemorySensor()
  {
    _type = cea::Float;
    _isActive = true;

    _memFree = _memUsed = _memBuffers = _memShared = _memCached = 0;
    _memTotal = 1;
    _memPid = 0;
    page_to_kb_shift = 0;

    std::ifstream ifs("/proc/meminfo");
    if (ifs.good())
      {
        ifs.ignore(256, ' '); // ignore until space
        ifs >> _memTotal;
      }
    ifs.close();

    // The run-time acquired page size

    // from top: get virtual page size -- nearing huge!
    Page_size = getpagesize();
    int i = Page_size;
    Page_size /= 1024;
    while (i > 1024)
      {
        i >>= 1;
        page_to_kb_shift++;
      }
  }

  // The update method defines how the sensor change its state
  void
  update()
  {
    std::stringstream ss;
    ss << cea::Tools::exec("free -k");
    ss.ignore(256, '\n');
    ss.ignore(256, ' '); // ignore until space
    ss >> _memTotal >> _memUsed >> _memFree >> _memShared >> _memBuffers
        >> _memCached;

    _cValue.Float = (float) (_memUsed - _memBuffers - _memCached) / _memTotal;
  }

  // Collects the Resident Set Size info from the /proc/[pid]/statm file
  void
  updatePid(pid_t pid)
  {
    char buff[32];

    sprintf(buff, "/proc/%d/statm", pid);
    std::ifstream ifs(buff);
    if (ifs.good())
      {
        ifs.ignore(256, ' '); // ignore until space
        ifs >> _memPid;
      }
  }

  // The getValue method returns the sensor's last updated value
  cea::sensor_t
  getValue()
  {
    return _cValue;
  }

  cea::sensor_t
  getValuePid(pid_t pid)
  {
    updatePid(pid);

    cea::sensor_t ret;
    ret.Float = ((float) PAGES_TO_KB(_memPid) / _memTotal);

    return ret;
  }

protected:
  cea::u64 _memTotal;
  cea::u64 _memUsed;
  cea::u64 _memShared;
  cea::u64 _memBuffers;
  cea::u64 _memCached;
  cea::u64 _memFree;
  cea::u64 _memPid;

  // The run-time acquired page size
  unsigned Page_size;
  unsigned page_to_kb_shift;

};

#define ITER 10

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////
int
main()
{
  MyMemorySensor s;

  // check if the sensor is active
  if (s.getStatus() == true)
    {
      struct dirent *dir;
      float tmp, sum;

      for (int i = 0; i < ITER; i++)
        {
          s.update();
          sum = 0;

          // Open proc dir
          DIR* proc = opendir("/proc");

          std::cout << "-------------" << std::endl;

          // Get all the numeric dir (corresponding to the pid)
          while ((dir = readdir(proc)) != NULL)
            {
              if (cea::Tools::isNumeric(dir->d_name))
                {
                  pid_t pid;
                  std::stringstream ss;
                  ss << dir->d_name;
                  ss >> pid;
                  tmp = s.getValuePid(pid).Float;
                  sum += tmp;
                  if (tmp > 0.01)
                    std::cout << pid << " " << tmp << std::endl;
                }
            }
          std::cout << "Sum: " << sum << std::endl;
          std::cout << "Machine: " << s.getValue().Float << std::endl;

          usleep(500000);
        }
    }

  return (0);
}
