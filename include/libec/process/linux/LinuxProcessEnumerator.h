///////////////////////////////////////////////////////////////////////////////
/// @file		LinuxProcessEnumerator.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Linux running process enumerator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_LINUXPROCESSENUMERATOR_H__
#define LIBEC_LINUXPROCESSENUMERATOR_H__
#ifdef __unix__

#include <iostream>
#include <string>
#include <stdlib.h>
#include <dirent.h>

#include "../BaseProcessEnumerator.h"
#include "LinuxProcess.h"
#include "ProcessStat.h"

namespace cea
{

  /// @brief Linux running process enumerator
  class LinuxProcessEnumerator : public BaseProcessEnumerator
  {
  public:
    ProcessMap
    getAllProcesses();

//    bool
//    isProcessAlive(pid_t pid);

  protected:

    /// @brief Get the CPU total usage time
    /// @return CPU total usage time
    long int
    getCurrentCPUTotalTime();

    /// @brief Retrieve the list of running process
    void
    enumProcess();

    /// @brief Create a LinuxProcess from a pid
    /// @param pid Process Identificator considered
    /// @return Pointer to the Process created \n
    ///         Or 0 if no process created
    Process*
    createProcess(pid_t pid);
  };

}

#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::LinuxProcessEnumerator
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
