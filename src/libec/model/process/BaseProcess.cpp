#include <libec/process/BaseProcess.h>

namespace cea
{
  /** Constructor */
  Process::Process(pid_t pid) :
      _pid(pid), _userId(-1), _startTime(0), _createdTick(0), _updateTick(0), _cpuUserLastTime(
          0), _cpuSystemLastTime(0), _cpuUserUsage(0.f), _cpuSystemUsage(0.f)
  {
    ;
  }

  Process::~Process()
  {
  }

  /** +getName : string */
  std::string
  Process::getName() const
  {
    return _name;
  }

  /** +getPath : string */
  std::string
  Process::getPath() const
  {
    return _path;
  }

  /** +getUserId : int, return -1 if not set */
  int
  Process::getUserId() const
  {
    return _userId;
  }

  /** +getPid : pid_t */
  pid_t
  Process::getPid() const
  {
    return _pid;
  }

  /** +getStartTime */
  long int
  Process::getStartTime() const
  {
    return _startTime;
  }

  /** +getCPUUserUsage */
  double
  Process::getCPUUserUsage() const
  {
    return _cpuUserUsage;
  }

  /** +getCPUSystemUsage */
  double
  Process::getCPUSystemUsage() const
  {
    return _cpuSystemUsage;
  }

  /** +getCPUTotalUsage */
  double
  Process::getCPUTotalUsage() const
  {
    return (_cpuUserUsage + _cpuSystemUsage) / 2.0;
  }

  /** +operator<<(..) */
  std::ostream&
  operator<<(std::ostream& o, const Process& p)
  {
    o << p._name << '\t' << p._pid << '\t' << p._userId; // << '\t' << p.m_Path;
    return o;
  }

}
