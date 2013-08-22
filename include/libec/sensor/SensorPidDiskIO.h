#ifndef SENSORPID_DISKIO_H__
#define SENSORPID_DISKIO_H__

#include "SensorPid.h"

namespace cea
{
/// Gets the disk IO  cpu time (user + system) between two calls.
  class DiskIO : public PIDSensor
  {
  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Constructor
    DiskIO(const char* dev);

    /// Destructor
    ~DiskIO();

    sensor_t
    getValue();

    sensor_t
    getValuePid(pid_t pid);

    u64
    getReadBytes();

    u64
    getReadBytes(pid_t pid);

    u64
    getWriteBytes();

    u64
    getWriteBytes(pid_t pid);

    u64
    getCancelWriteBytes(pid_t pid);

    /// Updates the information in a machine level.
    /// Just collect data from one device, specified on the constructor.
    /// The information comes from the /sys/block/[dev]/stat file, described in
    /// Documentation/iostats.txt
    void
    update();

    /// Updates the information in a process level. Retrieves data from the
    /// /proc/[pid]/io file. This data is the total number of reads/writes
    /// and don't takes into account in which device it occured.
    void
    updatePid(pid_t pid);

    void
    add(pid_t pid);

    void
    remove(pid_t pid);

  protected:
    //todo: correlates the iodata from a PID to a specific device
    struct iodata
    {
      u64 read;
      u64 write;
      //todo: find 'cwrite' for the machine level.
    };

    /// path to the machine's disk info file (/sys/block/[dev]/stat)
    char _macPath[32];

    iodata _macValue;
    std::map<pid_t, iodata> _pidValue;
  };

}
#endif
