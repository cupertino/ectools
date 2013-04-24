///////////////////////////////////////////////////////////////////////////////
/// @file		process.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		All headers file needed for process enumeration
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBCEA_PROCESS_H__
#define LIBCEA_PROCESS_H__

#include "process/BaseProcessEnumerator.h"

/* If the target OS is Windows then use WindowsProcessEnumerator */
#ifdef _WIN32
#include "process/windows/WindowsProcessEnumerator.h"
/// @brief If Windows platform use WindowsProcessEnumerator
#define ProcessEnumerator WindowsProcessEnumerator
#endif

/* If the target OS is Linux then use LinuxProcessEnumerator */
#ifdef __unix__
#include "process/linux/LinuxProcessEnumerator.h"
/// @brief If Unix platform use LinuxProcessEnumerator
#define ProcessEnumerator LinuxProcessEnumerator
#endif

#include "process/linux/ProcessStat.h"

#endif

///////////////////////////////////////////////////////////////////////////////
/// @defgroup process Process module
/// @brief Running process enumeration system
///////////////////////////////////////////////////////////////////////////////
