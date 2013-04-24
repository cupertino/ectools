///////////////////////////////////////////////////////////////////////////////
/// @file		WindowsProcessEnumerator.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Windows running process enumerator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_WINDOWSPROCESSENUMERATOR_H__
#define LIBEC_WINDOWSPROCESSENUMERATOR_H__
#ifdef _WIN32

#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>

#include "../BaseProcessEnumerator.h"
#include "WindowsProcess.h"

namespace cea
  {

    /// @brief Windows running process enumerator
    class WindowsProcessEnumerator : public BaseProcessEnumerator
      {
      protected:

        /// @brief Get the CPU total usage time
        /// @return CPU total usage time
        long int getCurrentCPUTotalTime();

        /// @brief Retrieve the list of running process
        void enumProcess();

        /// @brief Create a WindowsProcess from a pid
        /// @param pid Process Identificator considered
        /// @return Pointer to the Process created \n
        ///         Or 0 if no process created
        Process* createProcess(pid_t pid);

      };

  }

#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::WindowsProcessEnumerator
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
