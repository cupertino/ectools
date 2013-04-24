///////////////////////////////////////////////////////////////////////////////
/// @file		WindowsProcess.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Windows process details
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_WINDOWSPROCESS_H__
#define LIBEC_WINDOWSPROCESS_H__
#ifdef _WIN32

#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>

#include "../BaseProcess.h"

namespace cea
  {

    /// @brief Windows process details
    class WindowsProcess : public Process
      {
      public:

        /* Constructor */
        /// @brief Construct a process item and retrieve process properties
        /// @param pid Process Identificator considered
        WindowsProcess(pid_t pid);

        /* Function */
        /// @brief Calcul the cpu usage by total CPU time elapsed
        /// @param totalCPUTimeElapsed Total CPU time elapsed
        void calculCPUUsage(long int totalCPUTimeElapsed);

      protected:

        /* Tools function */
        /// @brief Retrieve the process name
        void retrieveName();
        /// @brief Retrieve the process path
        void retrievePath();
        /// @brief Retrieve the process userid
        void retrieveUserId();
        /// @brief Retrieve the process start time
        void retrieveStartTime();

      };

  }

#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::WindowsProcess
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
