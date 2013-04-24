#ifndef DATAACQUISITION_H_
#define DATAACQUISITION_H_

#include <list>
#include <libec/logs.h>
#include <libec/sensors.h>

namespace cea
{

  enum AcquisitionType
  {
    DAQ_PER_NODE = 0, // Data acquisition will be made for the entire node
    DAQ_PER_DIE = 1, // Data acquisition will be made for each CPU die/socket
    DAQ_PER_CORE = 2, // Data acquisition will be made for each CPU core
    DAQ_PER_SMT = 3 // Data acquisition will be made for each SMT (hyperthread)
  };

  /// @brief  Collect data while running a benchmark.
  /// @author Leandro Fontoura Cupertino
  /// @date   Jun 27, 2012
  class DataAcquisition
  {
  public:
    /// Constructor
    ///
    /// \param bench The complete path to execute a benchmark from a system call.
    /// \param idleTime Time in seconds that the system must wait before and after
    /// the execution of the benchmark. During this time, data will be collected.
    /// \param freq Frequency at which data will be collected.
    /// \param logfile Path to the log file (including its name).
    DataAcquisition(char* bench, unsigned int idleTime = 2, float freq = 1.0,
        std::string logfile = "daq_out.log");
    virtual
    ~DataAcquisition();

    /// Collects the data into the log file while running the specified
    /// benchmark. The log is appended to the file defined on the constructor.
    void
    collectData();

    /// Adds a sensor in the list of sensors to collect data if its active.
    /// @return true if the sensor was added, false otherwise.
    bool
    addSensor(Sensor *sensor);

    /// Tries to instantiate all the implemented sensors and return a list
    /// with the available ones.
    void
    getAvailableSensors(std::list<Sensor*> &sensor);

    void
    setBenchmark(char* bench);

    void
    setFrequency(float freq);

    void
    setIdleTime(unsigned int time);

  private:
    char* _benchCmd; // Benchmark comand line
    time_t _benchStart, _benchEnd;

    std::list<Sensor*> _sensors;
    int _periodTime;

    std::list<double*> _data;
    int _nofSensors;

    std::stringstream _ss;

    Log _logfile;

    /// Flag to indicate if the benchmark is running
    bool _isBenchRunning;

    // Time to collect data before and after benchmark's execution
    int _idleTime;

    /// Adds the sensor in a list if its active.
    /// @return true if the sensor was added, false otherwise.
    bool
    addSensor(std::list<Sensor*> *list, Sensor *sensor);

    void
    logHeader(std::list<Sensor*> sensor);

    void
    printHeader(std::list<Sensor*> sensor);

    void
    printSensors(std::list<Sensor*> sensor);

    void
    updateSensors(std::list<Sensor*> sensors);

    static void*
    runBenchmark(void* data);
  };

} /* namespace cea */
#endif /* DATAACQUISITION_H_ */
