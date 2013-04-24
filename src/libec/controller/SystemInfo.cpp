#include <fstream>
#include <assert.h>
#include <dirent.h>
#include <libec/tools.h>
#include <libec/sensors.h>
//#include <libec/process.h>
#include <libec/device/SystemInfo.h>

namespace cea
{
  //Static members
  bool SystemInfo::isStarted;
  CpuInfo SystemInfo::cpuInfo;
  struct utsname SystemInfo::uname_buf;
  int SystemInfo::uname_ret;
  std::string SystemInfo::_hostname;
  int SystemInfo::gethostname_ret;
  pid_t SystemInfo::pid_max;
  CpuElapsedTime SystemInfo::cpuElapsedTime;
  int SystemInfo::procCounter;
//  LinuxProcessEnumerator SystemInfo::procEnum;

  void
  SystemInfo::init()
  {

    uname_ret = uname(&uname_buf);

    size_t hostname_len = sysconf(_SC_HOST_NAME_MAX);
    if (hostname_len != (size_t) -1)
      {
        char hostname[hostname_len];
        gethostname_ret = gethostname(hostname, hostname_len);
        _hostname = hostname;
      }

    std::ifstream input("/proc/sys/kernel/pid_max", std::ifstream::in);
    input >> pid_max;

    isStarted = true;
  }

  std::string
  SystemInfo::getName()
  {
    if (uname_ret == 0)
      return uname_buf.sysname;
    else
      return "SystemInfo was not initialized. Try running SystemInfo::init();";
  }

  std::string
  SystemInfo::getRelease()
  {
    if (uname_ret == 0)
      return uname_buf.release;
    else
      return "SystemInfo was not initialized. Try running SystemInfo::init();";
  }

  std::string
  SystemInfo::getVersion()
  {
    if (uname_ret == 0)
      return uname_buf.version;
    else
      return "SystemInfo was not initialized. Try running SystemInfo::init();";
  }

  std::string
  SystemInfo::getArchitecture()
  {
    if (uname_ret == 0)
      return uname_buf.machine;
    else
      return "SystemInfo was not initialized. Try running SystemInfo::init();";
  }

  std::string
  SystemInfo::getHostName()
  {
    if (gethostname_ret == 0)
      return _hostname;
    else
      return "SystemInfo was not initialized. Try running SystemInfo::init();";
  }

  int
  SystemInfo::getCpuCount()
  {
    return cpuInfo.getCpuCount();
  }

  int
  SystemInfo::getCpuDies()
  {
    return cpuInfo.getCpuDies();
  }

  int
  SystemInfo::getCpuCores()
  {
    return cpuInfo.getCpuCores();
  }

  int
  SystemInfo::countProc()
  {
    struct dirent *dir = NULL;
    DIR* proc = opendir("/proc");

    /* Get all the numeric dir (corresponding to the pid) */
    procCounter = 0;
    while ((proc != NULL) && ((dir = readdir(proc)) != NULL))
      {
        if ((dir->d_type == DT_DIR) && (Tools::isNumeric(dir->d_name)))
          procCounter++;
      }

    /* Close the proc dir */
    closedir(proc);

    return procCounter;
  }

  int
  SystemInfo::countActiveProc()
  {
    int proc_count = 0;
// Open proc dir
    DIR* proc = opendir("/proc");
    struct dirent *dir;

// Get all the numeric dir (corresponding to the pid)
    while ((dir = readdir(proc)) != NULL)
      {
        if (dir->d_type == 0x4) // type folder=0x4, file=0x8
          if (cea::Tools::isNumeric(dir->d_name))
            {
              pid_t pid;
              std::stringstream ss;
              ss << dir->d_name;
              ss >> pid;
              if (cpuElapsedTime.getValuePid(pid).U64 > 0)
                proc_count++;
            }
      }
    return proc_count;
  }

  pid_t
  SystemInfo::getPidMax()
  {
    return pid_max;
  }
}
