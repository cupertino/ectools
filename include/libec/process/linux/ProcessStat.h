///////////////////////////////////////////////////////////////////////////////
/// @file		ProcessStat.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Helper class to read Unix proc/[pid]/stat files
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_PROCESSSTAT_H__
#define LIBEC_PROCESSSTAT_H__

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "../../Globals.h"
#include "../../tools/Tools.h"

namespace cea
{

  /// @brief Helper class to read Unix proc/[pid]/stat files
  class ProcessStat
  {
  public:

    /* Declaration */
    /// @brief Enumeration containning all the availible data from
    ///        proc/[pid]/stat (cf. man proc)
    enum Field
    {
      PID = 0, ///< (cf. man proc), corresponding type %d
      COMM, ///< (cf. man proc), corresponding type %s
      STATE, ///< (cf. man proc), corresponding type %c
      PPID, ///< (cf. man proc), corresponding type %d
      PGRP, ///< (cf. man proc), corresponding type %d
      SESSION, ///< (cf. man proc), corresponding type %d
      TTY_NR, ///< (cf. man proc), corresponding type %d
      TPGID, ///< (cf. man proc), corresponding type %d
      FLAGS, ///< (cf. man proc), corresponding type %u
      MINFLT, ///< (cf. man proc), corresponding type %lu
      CMINFLT, ///< (cf. man proc), corresponding type %lu
      MAJFLT, ///< (cf. man proc), corresponding type %lu
      CMAJFLT, ///< (cf. man proc), corresponding type %lu
      UTIME, ///< (cf. man proc), corresponding type %lu
      STIME, ///< (cf. man proc), corresponding type %lu
      CUTIME, ///< (cf. man proc), corresponding type %ld
      CSTIME, ///< (cf. man proc), corresponding type %ld
      PRIORITY, ///< (cf. man proc), corresponding type %ld
      NICE, ///< (cf. man proc), corresponding type %ld
      NUM_THREADS, ///< (cf. man proc), corresponding type %ld
      ITREALVALUE, ///< (cf. man proc), corresponding type %ld
      STARTTIME, ///< (cf. man proc), corresponding type %llu
      VSIZE, ///< (cf. man proc), corresponding type %lu
      RSS, ///< (cf. man proc), corresponding type %ld
      RSSLIM, ///< (cf. man proc), corresponding type %lu
      STARTCODE, ///< (cf. man proc), corresponding type %lu
      ENDCODE, ///< (cf. man proc), corresponding type %lu
      STARTSTACK, ///< (cf. man proc), corresponding type %lu
      KSTKESP, ///< (cf. man proc), corresponding type %lu
      KSTKEIP, ///< (cf. man proc), corresponding type %lu
      SIGNAL, ///< (cf. man proc), corresponding type %lu
      BLOCKED, ///< (cf. man proc), corresponding type %lu
      SIGIGNORE, ///< (cf. man proc), corresponding type %lu
      SIGCATCH, ///< (cf. man proc), corresponding type %lu
      WCHAN, ///< (cf. man proc), corresponding type %lu
      NSWAP, ///< (cf. man proc), corresponding type %lu
      CNSWAP, ///< (cf. man proc), corresponding type %lu
      EXIT_SIGNAL, ///< (cf. man proc), corresponding type %d
      PROCESSOR, ///< (cf. man proc), corresponding type %d
      RT_PRIORITY, ///< (cf. man proc), corresponding type %u
      POLICY, ///< (cf. man proc), corresponding type %u
      DELAYACCT_BLKIO_TICKS, ///< (cf. man proc), corresponding type %llu
      GUEST_TIME, ///< (cf. man proc), corresponding type %lu
      CGUEST_TIME, ///< (cf. man proc), corresponding type %ld

      /// @brief Reserved -- not a stat field used only for optimization
      ProcStatFieldCount
    };

    /// @brief Type of a proc stat field value
    enum Type
    {
      UNKNOWN, ///< Unknown value type
      CHAR, ///< char type (%c)
      INT, ///< signed int type (%d)
      UINT, ///< unsigned int (%u)
      LINT, ///< signed long int (%ld)
      LUINT, ///< long unsigned int (%lu)
      LLUINT, ///< long long unsigned int (%llu)
      STR ///< string (%s)
    };

    /// @brief Generic proc stat field value
    struct Value
    {
      Type type; ///< Type of the proc stat field value

      /// @brief Value of the proc stat field
      union
      {
        char c; ///< Type::CHAR
        int d; ///< Type::INT
        unsigned int u; ///< Type::UINT
        long int ld; ///< Type::LINT
        long unsigned int lu; ///< Type::LUINT
        long long unsigned int llu; ///< Type::LLUINT
      } v;
    };

    /* Helpers functions */
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param c Out char value of the proc stat field
    /// @param defaultValue Value set to c if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, char& c, char defaultValue = ' ');
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param d Out signed int value of the proc stat field
    /// @param defaultValue Value set to d if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, int& d, int defaultValue = 0);
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param u Out unsigned int value of the proc stat field
    /// @param defaultValue Value set to u if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, unsigned int& u, unsigned int defaultValue = 0);
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param ld Out long signed int value of the proc stat field
    /// @param defaultValue Value set to ld if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, long int& ld, long int defaultValue = 0);
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param lu Out lond unsigned int value of the proc stat field
    /// @param defaultValue Value set to lu if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, long unsigned int& lu,
        long unsigned int defaultValue = 0);
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param llu Out long long unsigned int value of the proc stat field
    /// @param defaultValue Value set to llu if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, long long unsigned int& llu,
        long long unsigned int defaultValue = 0);
    /// @brief Get value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param s Out string value of the proc stat field
    /// @param defaultValue Value set to s if no stat field corresponding
    /// @return true if value retrieved
    static bool
    get(pid_t pid, Field stat, std::string& s, const std::string& defaultValue =
        "");

    /// @brief Get direct value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @return Value of the field
    static Value
    getValue(pid_t pid, Field stat);
    /// @brief Get string field of a proc stat file
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @return String value of the field
    static std::string
    getStr(pid_t pid, Field stat);

  protected:

    /* Protected - function */
    /// @brief Get string value of a proc stat field
    /// @param pid Process Identificator considered
    /// @param stat Field wanted
    /// @param retString Out string value of the proc stat field
    /// @return true If value retrieved
    static bool
    getStrValue(pid_t pid, Field stat, std::string& retString);

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::ProcessStat
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
