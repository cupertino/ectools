#include <libec/process/BaseProcessEnumerator.h>

#include <libec/tools/DebugLog.h>

namespace cea
{

  /** Constructor */
  BaseProcessEnumerator::BaseProcessEnumerator() :
      _updateTick(0), _cpuCalculUsageActivate(false), _totalCPULastTime(0), _totalCPUTimeElapsed(
          0), _isBeginUpdateDone(false), _frequency(1000), _lastTick(0)
  {
    ;
  }

  /** Destructor */
  BaseProcessEnumerator::~BaseProcessEnumerator()
  {
    /* Delete from memory the Process Pointer */
    for (ProcessMap::iterator it = _process.begin(); it != _process.end(); ++it)
      {
        delete it->second;
      }
  }

  /** +getFrequency */
  cea_time_t
  BaseProcessEnumerator::getFrequency() const
  {
    return _frequency;
  }
  /** +setFrequency */
  void
  BaseProcessEnumerator::setFrequency(cea_time_t freq)
  {
    _frequency = freq;
  }

  /** #needUpdate */
  bool
  BaseProcessEnumerator::needUpdate() const
  {
    /* Check the Frequency if there is a frequency */
    if (_frequency > 0)
      {
        /* Check the frequency */
        if (Tools::tick() - _lastTick < _frequency)
          {
            return false;
          }
      }
    /* Update needed */
    return true;
  }

  /** +beginUpdate */
  void
  BaseProcessEnumerator::beginUpdate()
  {
    if (_isBeginUpdateDone)
      endUpdate();
    _isBeginUpdateDone = true;
  }

  /** +update */
  void
  BaseProcessEnumerator::update()
  {

    /* Check the Frequency if there is a frequency */
    if (_frequency > 0)
      {
        /* Check the frequency */
        if (Tools::tick() - _lastTick < _frequency)
          {
            return;
          }
        /* Keep the new tick */
        _lastTick = Tools::tick();
      }
    /* Advance the Update Tick */
    _updateTick++;
    if (_updateTick > 1234567890)
      _updateTick = 0;
    /* Get the current Process time */
    if (_cpuCalculUsageActivate)
      {
        long int cpuCurrentTotalTime = getCurrentCPUTotalTime();
        _totalCPUTimeElapsed = (cpuCurrentTotalTime - _totalCPULastTime);
        _totalCPULastTime = cpuCurrentTotalTime;
      }
    /* Enumerate the new Process */
    enumProcess();
    /* Delete the process which are dead */
    for (ProcessMap::iterator it = _process.begin(); it != _process.end();)
      {
        if (it->second->_updateTick != _updateTick)
          {
            /* Feed the monitor */
            feedDeleteItem(FEEDER_PROCESS_ITEM, it->second);
            /* Remove the process from memory */
            if (_isBeginUpdateDone)
              {
                /* Store in a list: the item will be deleted on endUpdate */
                _deletedProcess.push_back(it->second);
              }
            else
              {
                /* Delete the item from memory */
                delete it->second;
              }
            /* Remove the process from the list */
            _process.erase(it++);

          }
        else
          {
            ++it;
          }
      }

  }

  /** +endUpdate */
  void
  BaseProcessEnumerator::endUpdate()
  {
    /* Delete all the deleted Process */
    for (std::vector<Process*>::iterator it = _deletedProcess.begin();
        it != _deletedProcess.end(); ++it)
      {
        delete (*it);
      }
    /* Finish the update */
    _deletedProcess.clear();
    _isBeginUpdateDone = false;
  }

  /** +clear */
  void
  BaseProcessEnumerator::clear()
  {
    /* Delete from memory the Process Pointer */
    for (ProcessMap::iterator it = _process.begin(); it != _process.end(); ++it)
      {
        delete it->second;
      }
    /* Clear the Process List */
    _process.clear();
    _deletedProcess.clear();
    _isBeginUpdateDone = false;
  }

  bool
  BaseProcessEnumerator::isCalculUsageCPUActivate() const
  {
    return _cpuCalculUsageActivate;
  }

  void
  BaseProcessEnumerator::setCalculUsageCPU(bool activate)
  {
    _totalCPULastTime = getCurrentCPUTotalTime();
    _cpuCalculUsageActivate = activate;
  }

  unsigned int
  BaseProcessEnumerator::getProcessCount() const
  {
    return _process.size();
  }

  Process*
  BaseProcessEnumerator::getProcess_const(unsigned int id) const
  {
    /* Search the process */
    for (ProcessMap::const_iterator it = _process.begin(); it != _process.end();
        ++it)
      {
        if (id == 0)
          return it->second;
        id--;
      }
    // If no item found return 0
    return 0;
  }

  Process*
  BaseProcessEnumerator::getProcess(unsigned int id)
  {
    // Search the process
    for (ProcessMap::iterator it = _process.begin(); it != _process.end(); ++it)
      {
        if (id == 0)
          return it->second;
        id--;
      }
    // If no item found return 0
    return 0;
  }

  /** #updateProcess(p : Process*) */
  void
  BaseProcessEnumerator::updateProcess(Process* p)
  {
    if (_cpuCalculUsageActivate)
      p->calculCPUUsage(_totalCPUTimeElapsed);
    if (applyFilter(p))
      {
        p->_updateTick = _updateTick;
        /* Feed the monitor */
        feedUpdateItem(FEEDER_PROCESS_ITEM, p);
      }
  }

  /** Apply Filter on a Process - return True if process ok with filter */
  bool
  BaseProcessEnumerator::applyFilter(Process* p)
  {
    /* Check all the filter */
    for (std::list<ProcessFilter*>::iterator it = _filters.begin();
        it != _filters.end(); ++it)
      {
        if (!((*it)->applyFilter(p)))
          return false;
      }
    /* If all the filter apply and no reject then it's ok */
    return true;
  }

  /** #addProcess(pid : pid_t) */
  Process*
  BaseProcessEnumerator::addProcess(pid_t pid)
  {
    /* Create the Process */
    Process* p = createProcess(pid);
    /* Add the Process */
    if (p != 0)
      {
        if (applyFilter(p))
          {
            /* Add the process - Check if the pid not already present
             * and calcul the best position */
            ProcessMap::iterator lb = _process.lower_bound(pid);
            if ((lb != _process.end())
                && (!(_process.key_comp()(pid, lb->first))))
              {
                /* Item already exist */
                lb->second = p;
              }
            else
              {
                /* Add the process */
                _process.insert(ProcessMap::value_type(pid, p));
              }
            p->_createdTick = _updateTick;
            /* Feed the monitor */
            feedCreateItem(FEEDER_PROCESS_ITEM, p);
          }
        else
          {
            delete p;
            p = 0;
          }
      }
    /* Return the created process */
    return p;
  }

  /** +getProcess(pid : pid_t) */
  Process*
  BaseProcessEnumerator::getProcessByPID_const(pid_t pid) const
  {
    /* Find the item */
    ProcessMap::const_iterator it = _process.find(pid);
    /* Return Item */
    if (it != _process.end())
      {
        return it->second;
      }
    else
      {
        return 0;
      }
  }

  /** +getProcess(pid : pid_t) */
  Process*
  BaseProcessEnumerator::getProcessByPID(pid_t pid)
  {
    /* Find the item */
    ProcessMap::iterator it = _process.find(pid);
    /* Return Item */
    if (it != _process.end())
      {
        return it->second;
      }
    else
      {
        return 0;
      }
  }

  /** +getDeletedProcessCount */
  unsigned int
  BaseProcessEnumerator::getDeletedProcessCount() const
  {
    return _deletedProcess.size();
  }

  /** +getDeletedProcess */
  Process*
  BaseProcessEnumerator::getDeletedProcess(unsigned int id)
  {
    if (id < _deletedProcess.size())
      {
        return _deletedProcess[id];
      }
    else
      {
        return 0;
      }
  }

  /** +addFilter */
  void
  BaseProcessEnumerator::addFilter(ProcessFilter* filter)
  {
    /* Check if the filter already present */
    for (std::list<ProcessFilter*>::iterator it = _filters.begin();
        it != _filters.end(); ++it)
      {
        if ((*it) == filter)
          return;
      }
    /* Add the filter */
    _filters.push_back(filter);
  }
  /** +removeFilterById */
  void
  BaseProcessEnumerator::removeFilterById(unsigned int filterId)
  {
    for (std::list<ProcessFilter*>::iterator it = _filters.begin();
        it != _filters.end(); ++it, filterId--)
      {
        if (filterId == 0)
          {
            _filters.erase(it);
            return;
          }
      }
  }
  /** +removeFilter */
  void
  BaseProcessEnumerator::removeFilter(ProcessFilter* filter)
  {
    for (std::list<ProcessFilter*>::iterator it = _filters.begin();
        it != _filters.end(); ++it)
      {
        if ((*it) == filter)
          {
            _filters.erase(it);
            return;
          }
      }
  }
  /** +clearFilter */
  void
  BaseProcessEnumerator::clearFilter()
  {
    _filters.clear();
  }
  /** +getFilterCount */
  unsigned int
  BaseProcessEnumerator::getFilterCount() const
  {
    return _filters.size();
  }

  /** +operator<<(..) */
  std::ostream&
  operator<<(std::ostream& o, const BaseProcessEnumerator& p)
  {
    o << "Process Count=<" << p._process.size() << ">" << std::endl << "-------"
        << std::endl;
    for (BaseProcessEnumerator::ProcessMap::const_iterator it =
        p._process.begin(); it != p._process.end(); ++it)
      {
        o << (*(it->second)) << std::endl;
      }
    return o;
  }

}
