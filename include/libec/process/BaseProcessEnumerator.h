///////////////////////////////////////////////////////////////////////////////
/// @file		BaseProcessEnumerator.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Base class for running process enumeration
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_PROCESSENUMERATOR_H__
#define LIBEC_PROCESSENUMERATOR_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>

#include "BaseProcess.h"
#include "ProcessFilter.h"
#include "../Globals.h"
#include "../tools/Tools.h"
#include "../monitor/feeder/MonitorFeeder.h"

namespace cea
{

  /// @brief Base class for running process enumeration
  class BaseProcessEnumerator : public MonitorFeeder
  {
  public:

    /// @brief Default constructor
    BaseProcessEnumerator();

    /// @brief Destroy all process created
    virtual
    ~BaseProcessEnumerator();

    /* Property */
    /// @brief Get update frequency
    /// @return Update frequency in ms
    cea_time_t
    getFrequency() const;
    /// @brief Set enumerate process frequency in ms
    /// @param freq Update frequency in ms
    void
    setFrequency(cea_time_t freq);

    /// \brief CPU usage
    /// @return True
    bool
    isCalculUsageCPUActivate() const;

//    /// \brief Check if the process is still alive
//    virtual bool
//    isProcessAlive(pid_t pid) = 0;

    /// @brief Activate/Desactivate CPU usage calcul
    ///
    /// By defaul CPU usage is not calculated.
    ///
    void
    setCalculUsageCPU(bool activate);

    /* Update function */
    /// @brief Check if update needed
    /// @return true if update needed following the frequency
    bool
    needUpdate() const;

    /// @brief Begin update store all deleted process until endUpdate
    ///
    /// This enumaration mode give you access to manage directly deleted
    /// process.
    ///
    /// If this function is not call then the deleted process will not be
    /// recorded
    ///
    void
    beginUpdate();

    /// @brief Enumerate all the running process follow frequency and
    ///        feed connected Monitor.
    ///
    /// Add new process, delete dead process, update other.
    ///
    /// If frequency = 0 Then Update all rows at each call \n
    /// Else Update following the frequency of the monitor.
    ///
    void
    update();

    /// @brief End update clear all deleted process recorded due to
    ///        begindUpdate
    ///
    /// This enumaration mode give you access to manage directly deleted
    /// process.
    ///
    /// If this function is not call then the deleted recorded process
    /// will be real deleted on next beginUpdate call.
    ///
    void
    endUpdate();

    /// @brief Clear all list of running process enumerated
    ///
    /// This function causes the refresh of all the running process
    /// list.
    ///
    void
    clear();

    /// @brief Get count of running process enumerated
    /// @return Process count enumerated
    unsigned int
    getProcessCount() const;

    /* Get the process */
    /// @brief Get a process without modify ProcessEnumerator class
    /// @param id Number of process to get (start from 0)
    /// @return Pointer to the corresponding process. \n
    ///         Or 0 if no process found
    Process*
    getProcess_const(unsigned int id) const;
    /// @brief Get a process
    /// @param id Number of process to get (start from 0)
    /// @return Pointer to the corresponding process. \n
    ///         Or 0 if no process found
    Process*
    getProcess(unsigned int id);

    /// @brief Get a process by PID without modify ProcessEnumerator class
    /// @param pid Sytem Process Identificator
    /// @return Pointer to the corresponding process. \n
    ///         Or 0 if no process found
    Process*
    getProcessByPID_const(pid_t pid) const;
    /// @brief Get a process by PID
    /// @param pid Sytem Process Identificator
    /// @return Pointer to the corresponding process. \n
    ///         Or 0 if no process found
    Process*
    getProcessByPID(pid_t pid);

    /// @brief Get count of deleted process recorded due to beginUpdate
    /// @return Number of deleted process since last beginUpdate \n
    ///			Or 0 if beginUpdate was not called before update
    unsigned int
    getDeletedProcessCount() const;
    /// @brief Get a deleted recorded process
    /// @param id Number of recorded process considered
    /// @return Pointer to the corresponding process. \n
    ///         Or 0 if no process found
    Process*
    getDeletedProcess(unsigned int id);

    /* Filter mangement */
    /// @brief Add a process filter
    ///
    /// Process filter used to consider only some runnning process.
    /// If no filter set then consider all the runnning process.
    ///
    /// @param filter Process Filter to add
    void
    addFilter(ProcessFilter* filter);
    /// @brief Remove a process filter
    ///
    /// Process filter used to consider only some runnning process.
    /// If no filter set then consider all the runnning process.
    ///
    /// @param filterId Process Filter number to remove (start from 0)
    void
    removeFilterById(unsigned int filterId);
    /// @brief Remove a process filter
    ///
    /// Process filter used to consider only some runnning process.
    /// If no filter set then consider all the runnning process.
    ///
    /// @param filter Process Filter to remove
    void
    removeFilter(ProcessFilter* filter);

    /// @brief Clear all process filter
    ///
    /// Process filter used to consider only some runnning process.
    /// If no filter set then consider all the runnning process.
    ///
    void
    clearFilter();

    /// @brief Get filter count
    ///
    /// Process filter used to consider only some runnning process.
    /// If no filter set then consider all the runnning process.
    ///
    /// @return Filter count
    unsigned int
    getFilterCount() const;

    /* Operator << */
    /// @brief Write in an output stream a list of running process
    /// @param o Output stream
    /// @param p Process enumerator to write
    /// @return output stream: o
    friend std::ostream&
    operator<<(std::ostream& o, const BaseProcessEnumerator& p);

  protected:

    /* Protected - function */

    /// @brief Add a process to the running process list
    /// @param pid Process Identificator considered
    /// @return Pointer to the Process added \n
    ///         Or 0 if no process added
    Process*
    addProcess(pid_t pid);

    /// @brief Update a process to the running process list
    /// @param p Process to update
    void
    updateProcess(Process* p);

    /// @brief Apply all filters on Process and select process only
    ///        if all filters passed.
    /// @param p Pointer to the Process to test
    /// @return true if all filters passed
    bool
    applyFilter(Process* p);

    /// @brief Get the CPU total usage time
    /// This function need to be overridden.
    /// \return CPU total usage time
    virtual long int
    getCurrentCPUTotalTime() = 0;

    /// \brief Retrieve the list of running process
    /// This function need to be overridden.
    virtual void
    enumProcess() = 0;

    /// \brief Create a process of a ProcessEnumerator
    /// This function need to be overridden.
    /// \param pid Process Identificator considered
    /// \return Pointer to the Process created \n
    ///         Or 0 if no process created
    virtual Process*
    createProcess(pid_t pid) = 0;

    // Protected - Members
    /// \brief Map of process mapped with Process Identificator
    typedef std::map<pid_t, Process*> ProcessMap;

    ProcessMap _process; ///< List of running process

  private:

    /* Private - Members */
    /// @brief ProcessEnumerator update counter
    ///
    /// Used to know if a Process is alive.
    ///
    int _updateTick;

    /// @brief Indicate if CPU usage need to be calculated
    bool _cpuCalculUsageActivate;

    /// @brief Total CPU time stored for CPU usage calcul
    long int _totalCPULastTime;
    /// @brief Total CPU time elapsed stored for CPU usage calcul
    long int _totalCPUTimeElapsed;

    /// @brief Indicate if beginUpdate was called
    bool _isBeginUpdateDone;

    /// @brief List of deleted process recorded due to beginUpdate
    std::vector<Process*> _deletedProcess;

    /// @brief List of filter to use for enumeration of running process
    std::list<ProcessFilter*> _filters;

    /// @brief Frequency in millisecond of update function
    ///
    /// If 0 always update.
    /// By default=1000ms
    ///
    cea_time_t _frequency;
    cea_time_t _lastTick; ///< Last time of update call

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::BaseProcessEnumerator
///	@ingroup process
///
/// This class is the base for LinuxProcessEnumerator and 
/// WindowsProcessEnumerator. \n
///
/// When a new OS need to be supported a BaseProcessEnumerator and
/// a Process class need to be created to enumerate running process of the OS.
///
///////////////////////////////////////////////////////////////////////////////
