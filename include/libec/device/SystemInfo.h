#ifndef LIBEC_SYSTEMINFO_H__
#define LIBEC_SYSTEMINFO_H__

#include <unistd.h>
#include <sys/utsname.h>

#include "CpuInfo.h"
#include "MemInfo.h"
#include "../sensors.h"
#include "../process.h"

namespace cea
{
  /// \brief   System information data.
  /// \author  Leandro Fontoura Cupertino
  /// \date    Aug 9, 2012
  class SystemInfo
  {
  public:
    /// Initializes the SystemInfo class and its static members.
    static void
    init();

    /// Counts the total number of process are available. This values comes
    /// from listing all numeric directories in /proc filesystem.
    static int
    countProc();

    /// Counts the number of active process are available. This value is calculated by
    /// counting the number of precesses which were executed in the last time step.
    static int
    countActiveProc();

    /// Check if process is alive. Zombies processes are not considered in this case.
    static bool
    isProcessAlive(pid_t pid);

    /// Gets the operating system name.
    static std::string
    getName();

    /// Gets the operating system release (e.g., "2.6.28").
    static std::string
    getRelease();

    /// Gets the operating system version.
    static std::string
    getVersion();

    /// Gets the hardware identifier (e.g., x86_64).
    static std::string
    getArchitecture();

    /// Gets the hostname of the current processor.
    static std::string
    getHostName();

    /// Gets CPU information.
    /// \return A CpuInfo pointer.
    static CpuInfo*
    getCpuInfo();

    /// Gets the total number of CPUs available.
    /// \returns The number of CPUs or -1 if an error occurred.
    static int
    getCpuCount();

    /// Gets the total number of CPU dies available.
    /// \returns The number of dies or -1 if an error occurred.
    static int
    getCpuDies();

    /// Gets the total number of CPU cores available.
    /// \returns The number of cores or -1 if an error occurred.
    static int
    getCpuCores();

    /// Gets memory information.
    /// \return A MemoryInfo pointer.
    static Memory*
    getMemoryInfo();

    /// \brief Gets PID's maximum value
    /// \return A pid_t with the PID maximum value
    /// \description
    /// Gets the value at which PIDs wrap around (i.e., this value is one
    /// greater  than  the  maximum  PID). Its default value is 32768.
    /// On 32-bit platforms, 32768 is the maximum value for pid_max.
    /// On 64-bit systems, pid_max can be set to any value up to 2^22
    /// (PID_MAX_LIMIT, approximately 4 million).
    static pid_t
    getPidMax();

    /// \brief Retrieves if the program is running by the root user
    static int
    isRootUser();

  private:
    static bool isStarted;
    static CpuInfo cpuInfo;
    static struct utsname uname_buf;
    static int uname_ret;
    static std::string _hostname;
    static int gethostname_ret;
    static pid_t pid_max;
    static CpuElapsedTime cpuElapsedTime;
    static int procCounter;
    static int runAsRoot;
  };
}

#endif
