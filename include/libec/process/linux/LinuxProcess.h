///////////////////////////////////////////////////////////////////////////////
/// @file		LinuxProcess.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Linux process details
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_LINUXPROCESS_H__
#define LIBEC_LINUXPROCESS_H__

#ifdef __unix__
#include <iostream>
#include <string>
#include <stdlib.h>

#include "../BaseProcess.h"
#include "ProcessStat.h"

namespace cea
{

  /// @brief Linux process details
  class LinuxProcess : public Process
  {
  public:

    /* Constructor */
    /// @brief Construct a process item and retrieve process properties
    /// @param pid Process Identificator considered
    LinuxProcess(pid_t pid);

    /* Function */
    /// @brief Calcul the cpu usage by total CPU time elapsed
    /// @param totalCPUTimeElapsed Total CPU time elapsed
    void
    calculCPUUsage(long int totalCPUTimeElapsed);

  protected:

    /// @brief Retrieve the process path
    void
    retrievePath();

  };

}

#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::LinuxProcess
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
