#ifdef __unix__

#include <libec/process/linux/LinuxProcess.h>

namespace cea
{

  /** Constructor */
  LinuxProcess::LinuxProcess(pid_t pid) :
      Process(pid)
  {
    ProcessStat::get(pid, ProcessStat::COMM, _name);
    retrievePath();
    //ProcessStat::get(pid, ProcessStat::SESSION, _userId);
    //ProcessStat::get(pid, ProcessStat::STIME, _startTime);
  }

  /** #calculCPUUsage */
  void
  LinuxProcess::calculCPUUsage(long int totalCPUTimeElapsed)
  {
    /// This feature does not work well, however as is not an
    /// important part of the library this will remain as it.
    if (totalCPUTimeElapsed != 0)
      {
        /* Get the current Time */
        long int userTime, systemTime;
        ProcessStat::get(_pid, ProcessStat::UTIME, userTime);
        ProcessStat::get(_pid, ProcessStat::UTIME, systemTime);
        /* Calcul the percent of usage */
        _cpuUserUsage = 100 * (userTime - _cpuUserLastTime)
            / (totalCPUTimeElapsed);
        _cpuSystemUsage = 100 * (systemTime - _cpuSystemLastTime)
            / (totalCPUTimeElapsed);
        /* Store the current time */
        _cpuUserLastTime = userTime;
        _cpuSystemLastTime = systemTime;
      }
  }

  /** +retrievePath */
  void
  LinuxProcess::retrievePath()
  {
    /* Open the cmdline file */
    std::string file = "/proc/" + Tools::CStr(_pid) + "/cmdline";
    std::ifstream f(file.c_str());
    std::string cmdLine;

    /* If Opened extract Path */
    if (f.is_open())
      {

        /* Get the first line */
        if (f.good())
          std::getline(f, cmdLine);

        /* Close the file */
        f.close();

      }

    /* Return the first parameter of the cmd line */
    _path.assign(Tools::getWhiteSpaceToken(cmdLine, 0));
  }

}

#endif
