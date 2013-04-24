#ifdef _WIN32

#include <libec/process/windows/WindowsProcessEnumerator.h>

namespace cea
  {

    /** Get the cpu total time */
    long int WindowsProcessEnumerator::getCurrentCPUTotalTime()
      {
        /// This feature does not work well, however as is not an
        /// important part of the library this will remain as it.
        /* Get the current system time */
        SYSTEMTIME sysTime;
        GetSystemTime(&sysTime);
        /* Convert into file time */
        FILETIME sysFileTime;
        SystemTimeToFileTime(&sysTime, &sysFileTime);
        /* Get the LARGE_INTEGER */
        LARGE_INTEGER sysLDTime;
        sysLDTime.LowPart = sysFileTime.dwLowDateTime;
        sysLDTime.HighPart = sysFileTime.dwHighDateTime;
        /* Return the value */
        return (long int)((sysLDTime.QuadPart / 1000));
      }

    /* #enumProcess : void */
    void WindowsProcessEnumerator::enumProcess()
      {
        /* Check if target Windows */
        DWORD procTab[1024], sizeProcNeed;
        Process* p;
        if (EnumProcesses(procTab, sizeof(procTab), &sizeProcNeed))
          {
            for (unsigned int i = 0, max = sizeProcNeed / sizeof(DWORD); i < max; i++)
              {
                /* Search the Process */
                pid_t pid = procTab[i];
                p = getProcessByPID(procTab[i]);
                /* If Process Not Found create it */
                if (p == 0) p = addProcess(pid);
                /* Update */
                if (p != 0) updateProcess(p);
              }
          }
      }

    /** +createProcess */
    Process* WindowsProcessEnumerator::createProcess(pid_t pid)
      {
        return new WindowsProcess(pid);
      }

  }

#endif
