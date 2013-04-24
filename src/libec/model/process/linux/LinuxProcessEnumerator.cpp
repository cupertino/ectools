#ifdef __unix__

#include <sys/stat.h>

#include <libec/process/linux/LinuxProcessEnumerator.h>
#include <libec/tools/Tools.h>

namespace cea
{
  /** Get the cpu total time */
  long int
  LinuxProcessEnumerator::getCurrentCPUTotalTime()
  {
    /// This feature is not implemented, however as is not an
    /// important part of the library this will remain as it.
    return 0;
  }

//  bool
//  LinuxProcessEnumerator::isProcessAlive(pid_t pid)
//  {
//    std::string strPath = "/proc/" + Tools::CStr(pid);
//
//    if (access(strPath.c_str(), 0) == 0)
//      {
//        struct stat status;
//        stat(strPath.c_str(), &status);
//
//        return (status.st_mode & S_IFDIR);
//      }
//
//    return false;
//  }

  void
  LinuxProcessEnumerator::enumProcess()
  {
    // Check if target UNIX
    Process* p = 0;
    struct dirent *dir;
    long pid;
    char* endptr;
    // Open proc dir
    DIR* proc = opendir("/proc");

    // Get all the numeric dir (corresponding to the pid)
//    while ((dir = readdir(proc)) != NULL)
    while ((proc != NULL) && ((dir = readdir(proc)) != NULL))
      {
        if ((dir->d_type == DT_DIR) && (Tools::isNumeric(dir->d_name)))
          {
            // Get the process id
            pid = strtol(dir->d_name, &endptr, 10);
            if (*endptr != '\0')
              continue;
            // Search the Process
            p = getProcessByPID(pid);
            // If Process Not Found create it
            if (p == 0)
              p = addProcess(pid);
            // Update
            if (p != 0)
              updateProcess(p);
          }
      }

    /* Close the proc dir */
    closedir(proc);
  }

  /** +createProcess */
  Process*
  LinuxProcessEnumerator::createProcess(pid_t pid)
  {
    char state;
    ProcessStat::get(pid, ProcessStat::STATE, state);

    // Ignore zombie processes.
    // Most sensors cannot be evaluated on such state.
    if (state == 'Z')
      return NULL;

    return new LinuxProcess(pid);
  }

  std::map<pid_t, Process*>
  LinuxProcessEnumerator::getAllProcesses()
  {
    return _process;
  }
}

#endif
