///////////////////////////////////////////////////////////////////////////////
/// @file               SensorPidStat.h
/// @author             Leandro Fontoura Cupertino
/// @version            0.1
/// @date               Jun 28 2012
/// @copyright          2012, CoolEmAll (INFSO-ICT-288701)
/// @brief              Process related information from /proc/[pid]/stat file
///////////////////////////////////////////////////////////////////////////////

#ifndef SENSOR_PIDSTAT_H_
#define SENSOR_PIDSTAT_H_

#define MAX_COMM_LEN    128
#define MAX_CMDLINE_LEN 128

#include "SensorPid.h"

#include <vector>

namespace cea
{
  struct pid_stats
  { // from systat (C) 2007-2011 by Sebastien Godard (sysstat <at> orange.fr)
//    unsigned long long read_bytes __attribute__ ((aligned (8)));
//    unsigned long long write_bytes __attribute__ ((packed));
//    unsigned long long cancelled_write_bytes __attribute__ ((packed));
//    unsigned long long total_vsz __attribute__ ((packed));
//    unsigned long long total_rss __attribute__ ((packed));
//    unsigned long long total_stack_size __attribute__ ((packed));
//    unsigned long long total_stack_ref __attribute__ ((packed));
//    unsigned long minflt __attribute__ ((packed));
//    unsigned long cminflt __attribute__ ((packed));
//    unsigned long majflt __attribute__ ((packed));
//    unsigned long cmajflt __attribute__ ((packed));
    unsigned long utime __attribute__ ((packed));
//    unsigned long cutime __attribute__ ((packed));
    unsigned long stime __attribute__ ((packed));
//    unsigned long cstime __attribute__ ((packed));
    unsigned long gtime __attribute__ ((packed));
//    unsigned long cgtime __attribute__ ((packed));
    unsigned long vsz __attribute__ ((packed));
    long rss __attribute__ ((packed));
//    unsigned long nvcsw __attribute__ ((packed));
//    unsigned long nivcsw __attribute__ ((packed));
//    unsigned long stack_size __attribute__ ((packed));
//    unsigned long stack_ref __attribute__ ((packed));
    /* If pid is null, the process has terminated */
//    unsigned int pid __attribute__ ((packed));
    /* If tgid is not null, then this PID is in fact a TID */
//    unsigned int tgid __attribute__ ((packed));
//    unsigned int rt_asum_count __attribute__ ((packed));
//    unsigned int rc_asum_count __attribute__ ((packed));
//    unsigned int uc_asum_count __attribute__ ((packed));
    unsigned int processor __attribute__ ((packed));
//    unsigned int flags __attribute__ ((packed));
//    char comm[MAX_COMM_LEN];
//    char cmdline[MAX_CMDLINE_LEN];
    char state;
  };

  /// \brief Implements sensors with information from /proc/[pid]/stat and
  /// /proc/stat files
  /// \details
  /// This class is a PIDSensor which handle data from /proc/[pid]/stat and
  /// /proc/stat. The idea is to decrease the file reading. For this,
  /// each time an update is done, all the data used by the sensor is kept on
  /// memory, this way, if more than one type of PidStat sensor is required in
  /// a frequency lower then 1 second, the stored data is used.
  ///
  /// This classes contains several distinct sensors that may be instantiated
  /// by its TypeId public enumerator. A short description of each sensor are
  /// described on the TypeId enum.
  ///
  /// All time related values available on both files (/proc/stat and
  /// /proc/[pid]/stat) are presented as the total number of jiffies since boot
  /// time.
  /// A jiffy is the duration of one tick of the system timer interrupt. It is
  /// not an absolute time interval unit, since its duration depends on the
  /// clock interrupt frequency of the particular hardware platform. In linux
  /// systems the interrupt frequency can be calculated as the inverse of the
  /// clock tick, which can be obtained by sysconf call: sysconf(_SC_CLK_TCK).
  ///
  /// Its important to notice that the data related to cpu time (user, system,
  /// nice and idle) from the /proc/stat file does not match the sum of those
  /// in /proc/[pid]/stat file. The reason for this was not identified.
  /// Thus we identified two ways to compute the CPU usage (%CPU). As the proc
  /// files gives the jiffies since boot time, one can only estimate CPU usage
  /// in a given time step and work with the difference of each variable over
  /// time.
  ///
  /// The first one is to mix the data from both files and assume that the
  /// existing error will not affect the measured data, this way, one can just
  /// say that the CPU usage for a given processes is given as the following
  /// equation:
  /// \f[
  ///   \%CPU^{pid} = \frac{\Delta t_{usr}^{pid} + \Delta t_{sys}^{pid}}{\Delta t_{usr} + \Delta t_{nice} + \Delta t_{sys} + \Delta t_{idl}}
  /// \f]
  /// where \f$t_{usr}^{pid}\f$ and \f$t_{sys}^{pid}\f$ are the process' user
  /// and system time in jiffies (from /proc/[pid]/stat), while \f$t_{usr}\f$,
  /// \f$t_{nice}\f$, \f$t_{sys}\f$ and \f$t_{idl}\f$ are the user, nice,
  /// system and idle times for all the CPU cores in jiffies (from /proc/stat).
  ///
  /// Other way to compute \f$\%CPU^{pid}\f$ is to convert the time into
  /// seconds and divide by the time between calls times the number of
  /// available CPU cores.
  /// To convert jiffies into seconds, one need to divide it by the CPU's clock
  /// tick duration (sysconf(_SC_CLK_TCK)).
  /// \f[
  ///   \%CPU^{pid} = \frac{\Delta t_{usr}^{pid} + \Delta t_{sys}^{pid}}{\textrm{CLK\_TCK} \times \textrm{CPUs} \times \Delta t}
  /// \f]
  ///
  /// \b Example
  ///
  /// To show the usage of a PidStat sensor, the following code instantiates
  /// two different sensors: CPU_USAGE and RESIDENT_SET_SIZE.
  /// @code
  /// #include <iostream>
  /// #include <pthread.h>
  /// #include <math.h>
  ///
  /// #include <libec/tools.h>
  /// #include <libec/sensor/SensorPid.h>
  /// #include <libec/sensor/SensorPidStat.h>
  ///
  /// int main() {
  ///   cea::PIDSensor* sensor;
  ///   double load;
  ///   pid_t pid;
  ///
  ///   sensor = new cea::PidStat(cea::PidStat::CPU_USAGE);
  ///   load = 0.5f;
  ///
  ///   // check if the sensor was properly started
  ///   if (sensor->getStatus())
  ///     {
  ///       pid = getpid();
  ///       std::cout << "----------------------------" << std::endl;
  ///       std::cout << "Name: " << sensor->getName() << std::endl;
  ///       std::cout << "Alias: " << sensor->getAlias() << std::endl;
  ///       for (int i = 0; i < 10; i++)
  ///         {
  ///           std::cout << "pid: " << pid << " \tticks: " << time(NULL)
  ///               << " \tvalue:" << sensor->getValue(pid).Float << std::endl;
  ///           for (int j = 1; j < 10000000; j++)
  ///             {
  ///               load = (100.1*sin(load)/log(j));
  ///             }
  ///           sleep(1);
  ///         }
  ///       std::cout << "----------------------------" << std::endl;
  ///       std::cout << load << std::endl;
  ///     }
  ///   else
  ///     std::cerr << "error: sensor could not be opened." << std::endl;
  ///
  ///   return 0;
  /// }
  /// @endcode
  ///////////////////////////////////////////////////////////////////////////////
  class PidStat : public PIDSensor
  {
  public:

    /// @brief Type of PidStat sensor to return
    enum TypeId
    {
      /// Process' user + system CPU time / total CPU time.
      CPU_USAGE = 0,
      /// CPU number last executed on.
      CPU_LAST = 1,
      /// Virtual memory size in bytes.
      VIRTUAL_MEM_SIZE = 2,
      /// Number of pages the  process  has in  real memory. This is just the
      /// pages which count towards text, data, or stack space. This does not
      /// include pages which have not been demand-loaded in, or which are
      /// swapped out.
      RESIDENT_SET_SIZE = 3,
      /// Process state. One character from the string "RSDZTW" where R is
      /// running, S is sleeping in an interruptible wait, D is waiting in
      /// uninterruptible disk sleep, Z is zombie, T is traced or stopped (on
      /// a signal), and W is paging. Returns the char as a u64.
      PID_STATE = 4,
      /// Total number of types available.
      TYPE_MAX
    };

    PidStat(TypeId type = CPU_USAGE);
    virtual
    ~PidStat();

    void
    update();

    void
    updatePid(pid_t pid = -1);

    sensor_t
    getValuePid(pid_t pid = -1);

    void
    add(pid_t pid);

    void
    remove(pid_t pid);

  private:
    float _value[];
    u32 _typeId;
    u32 _ps_sensor_id;

    int _fd;
    char _sbuf[1024];
    static char _path[1024];

    /// Previous value
    sensor_t _pValue;

    std::map<pid_t, struct timeval> _ctimePIDMap, _ptimePIDMap;

//    std::map<pid_t, std::vector<u64>*> _cvPIDMap, _pvPIDMap;
    std::map<pid_t, struct pid_stats> _cvPIDMap, _pvPIDMap;

    void
    readProcPidStat(pid_t pid, unsigned long int *utime,
        unsigned long int *stime, unsigned long int *vsize, long int *rss);

    void
    readProcStat(unsigned long *user, unsigned long *nice, unsigned long *sys,
        unsigned long *idle);

  };

} /* namespace cea */
#endif /* SENSORPIDSTAT_H_ */

///////////////////////////////////////////////////////////////////////////////
///     @class cea::PIDSensor
///     @ingroup sensor
///     @ingroup pidSensor
///////////////////////////////////////////////////////////////////////////////
