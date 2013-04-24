#ifdef _WIN32
#include <libec/process/windows/WindowsProcess.h>

namespace cea
  {

    /** Constructor */
    WindowsProcess::WindowsProcess(pid_t pid) : Process(pid)
      {
        retrieveName();
        retrievePath();
        retrieveUserId();
        retrieveStartTime();
      }

    /** #calculCPUUsage */
    void WindowsProcess::calculCPUUsage(long int totalCPUTimeElapsed)
      {
        /// This feature does not work well, however as is not an
        /// important part of the library this will remain as it.
        if (totalCPUTimeElapsed != 0)
          {
            /* Retrieve the Process Handle */
            HANDLE procHandle = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, _pid);
            if (procHandle == 0) return;
            /* Get the Process Time */
            FILETIME userFileTime, sysFileTime, createFileTime, exitFileTime;
            GetProcessTimes(procHandle, &createFileTime, &exitFileTime, &sysFileTime, &userFileTime);
            /* Close Handle */
            CloseHandle(procHandle);
            /* Get the LARGE_INTEGER */
            LARGE_INTEGER userLDTime, sysLDTime;
            userLDTime.LowPart = userFileTime.dwLowDateTime;
            userLDTime.HighPart = userFileTime.dwHighDateTime;
            sysLDTime.LowPart = sysFileTime.dwLowDateTime;
            sysLDTime.HighPart = sysFileTime.dwHighDateTime;
            /* Get the Microseconds value */
            long int userTime = (long int)(userLDTime.QuadPart / 1000);
            long int systemTime = (long int)(sysLDTime.QuadPart / 1000);
            /* Calcul the percent of usage */
            _cpuUserUsage = 100 * (userTime - _cpuUserLastTime) / (totalCPUTimeElapsed);
            _cpuSystemUsage = 100 * (systemTime - _cpuSystemLastTime) / (totalCPUTimeElapsed);
            /* Store the current time */
            _cpuUserLastTime = userTime;
            _cpuSystemLastTime = systemTime;
          }
      }

    /** +retrieveName */
    void WindowsProcess::retrieveName()
      {
        char name[MAX_PATH] = " ";
        /* Get handle of the process */
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _pid);
        /* Get the name */
        if (hProcess)
          {
            HMODULE hMod;
            DWORD sizeName;
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &sizeName))
              {
                sizeName = GetModuleBaseName(hProcess, hMod, name, sizeof(name));
                _name.assign(name,sizeName);
              }
            CloseHandle( hProcess );
          }
      }

    /** +retrievePath */
    void WindowsProcess::retrievePath()
      {
        /// TODO
      }

    /** +retrieveUserId */
    void WindowsProcess::retrieveUserId()
      {
        /// TODO
      }

    /** +retrieveStartTime */
    void WindowsProcess::retrieveStartTime()
      {
        /// TODO
      }

  }

#endif
