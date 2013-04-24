///////////////////////////////////////////////////////////////////////////////
/// @file		BaseProcess.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base class for Process implementation
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_PROCESS_H__
#define LIBEC_PROCESS_H__

#include <iostream>
#include <string>

#include "../tools/Tools.h"
#include "../Globals.h"

namespace cea
{

  /* Prototype */
  class BaseProcessEnumerator;

  /// @brief Base class for Process implementation
  class Process
  {
  public:
    /* Constructor */
    /// @brief Create a base process from a Process Identificator
    /// @param pid Process Identificator considered
    Process(pid_t pid);

    virtual
    ~Process();

    /* Properties */
    /// @brief Get process name
    /// @return Process name
    std::string
    getName() const;

    /// @brief Get process path
    /// @return Process path
    std::string
    getPath() const;

    /// @brief Get process user id
    /// @return Process user id
    int
    getUserId() const;

    /// @brief Get process Identificator
    /// @return Process Identificator
    pid_t
    getPid() const;

    /// @brief Get process start time
    /// @return Process start time
    long int
    getStartTime() const;

    /// @brief Get CPU user usage
    /// @return CPU user usage calculated
    double
    getCPUUserUsage() const;
    /// @brief Get CPU system usage
    /// @return CPU system usage calculated
    double
    getCPUSystemUsage() const;
    /// @brief Get CPU total usage
    /// @return (CPU User usage + CPU system usage) / 2
    double
    getCPUTotalUsage() const;

    /* Friend */
    friend class BaseProcessEnumerator;

    /** Operator << */
    /// @brief Write in an output stream a process
    /// @param o Output stream
    /// @param p Process to write
    /// @return output stream: o
    friend std::ostream&
    operator<<(std::ostream& o, const Process& p);

    /// @brief Calcul the cpu usage by total CPU time elapsed
    /// @param totalCPUTimeElapsed Total CPU time elapsed
    virtual void
    calculCPUUsage(long int totalCPUTimeElapsed) = 0;

  protected:

    /* Members */
    pid_t _pid; ///< Process Identificator
    std::string _name; ///< Process name
    std::string _path; ///< Process path
    int _userId; ///< Process user id

    long int _startTime; ///< Process start time

    int _createdTick; ///< Process time creation

    /// @brief Process update tick
    ///
    /// If update tick != ProcessEnumerator::update tick Then
    ///		<ul>
    ///			<li>Process is dead</li>
    ///		</ul>
    /// Else
    ///		<ul>
    ///			<li>Process is alive</li>
    ///		</ul>
    ///
    int _updateTick;

    long int _cpuUserLastTime; ///< Process CPU user time
    long int _cpuSystemLastTime; ///< Process CPU system time
    double _cpuUserUsage; ///< Process CPU user usage
    double _cpuSystemUsage; ///< Process CPU system usage

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Process
///	@ingroup process
///
///////////////////////////////////////////////////////////////////////////////
