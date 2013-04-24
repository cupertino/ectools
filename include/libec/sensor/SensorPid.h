///////////////////////////////////////////////////////////////////////////////
/// \file               SensorPid.h
/// \author             Leandro Fontoura Cupertino
/// \version            0.1
/// \date               Jul 31 2012
/// \copyright          2012, IRIT, CoolEmAll (INFSO-ICT-288701), GPL.
/// \license            GPL
/// \brief              Process Id related Sensor
///////////////////////////////////////////////////////////////////////////////

#ifndef PIDSENSOR_H_
#define PIDSENSOR_H_

#include "Sensor.h"
#include <map>

namespace cea
{

  /// \brief            PID (process id) related Sensor.
  /// \author           Leandro Fontoura Cupertino
  /// \date             2012
  /// \copyright        Copyright © 2012, IRIT, CoolEmAll (INFSO-ICT-288701).
  /// \license          GPL.
  /// \details
  /// The PIDSensor is a Sensor which can be accessed independently for each
  /// process. The pure virtual method update(pid_t) define how the sensor
  /// should recalculate its current value variable. There is an already
  /// implemented getValue(pid_t pid) method which updates its current value,
  /// add its PID if its not present it the mapping and return the
  /// _cvPIDMap[pid] sensor_t variable.
  ///
  ///////////////////////////////////////////////////////////////////////////////
  /// @section examples Usage examples
  /// In the sections we describe some examples of how to use the PIDSensor
  /// to implement your own metric/estimator.
  ///
  /// <h3>Example 1: Simple PID sensor</h3>
  /// This is an introductory example that shows how a programmer can implement
  /// its own sensor. In this example,  FakeSensor returns the pid multiplied
  /// by a random number.
  ///
  /// Note that there is only three methods implemented. The constructor define
  /// the name, alias and type of the sensor. The name is used in the data
  /// acquisition output, while the alias is used as the column header on the
  /// ectop. The sensor can have two types: U64 (unsigned long long) or Float
  /// this information is used to handle the sensor_t union.
  ///
  /// The getValue(pid_t pid) is a virtual method to return the sensor value as
  /// a sensor_t (a union with float and u64). This method was kept virtual
  /// because some sensors may require the previous value and/or elapsed time
  /// to be computed, so the user can re-implement it according to his need.
  ///
  /// The last important method to be overwritten is the update, which is pure
  /// virtual. On this method one need to define how the update of the current
  /// value of the sensor is done.
  ///
  /// @code
  /// // demos/ex1/MySensor.h
  /// #ifndef MYSENSOR_H__
  /// #define MYSENSOR_H__
  ///
  /// #include <libec/sensor/SensorPid.h>
  ///
  /// class MySensor : public cea::PIDSensor
  /// {
  /// public:
  ///   MySensor();
  ///
  ///   cea::sensor_t getValue(pid_t pid);
  ///
  ///   void update(pid_t pid);
  /// };
  ///
  /// #endif
  /// @endcode
  ///
  /// @code
  /// // demos/ex1/MySensor.cpp
  /// #include "MySensor.h"
  /// #include <libec/tools/Tools.h>
  ///
  /// MySensor::MySensor() {
  ///   _name = "MySensor";
  ///   _alias = "MS";
  ///   _type = cea::U64;
  ///   _isActive = true;
  /// }
  ///
  /// cea::sensor_t MySensor::getValue(pid_t pid) {
  ///   update(pid);
  ///   return _cValue;
  /// }
  ///
  /// void MySensor::update(pid_t pid) {
  ///   _cValue.U64 = pid * cea::Tools::rnd(1, 10);
  /// }
  /// @endcode
  ///
  /// @code
  /// // demos/ex1/main.cpp
  /// #include "MySensor.h"
  /// #include <ctime>
  ///
  /// int main() {
  ///   cea::PIDSensor* sensor;
  ///   pid_t pid;
  ///
  ///   sensor = new MySensor();
  ///
  ///   // check if the sensor was properly started
  ///   if (sensor->getStatus())
  ///   {
  ///     pid = getpid();
  ///     std::cout << "----------------------------" << std::endl;
  ///     std::cout << "Name: " << sensor->getName() << std::endl;
  ///     std::cout << "Alias: " << sensor->getAlias() << std::endl;
  ///     for (int i = 0; i < 10; i++)
  ///     {
  ///       std::cout << "pid: " << pid << " \tticks: " << time(NULL)
  ///                 << " \tvalue:" << sensor->getValue(pid).U64 << std::endl;
  ///       sleep(1);
  ///     }
  ///     std::cout << "----------------------------" << std::endl;
  ///   }
  ///   else
  ///     std::cerr << "error: sensor could not be opened." << std::endl;
  ///
  ///   return 0;
  /// }
  /// @endcode
  ///
  /// <h3>Example 2: Time dependent PID sensor</h3>
  /// As mentioned before, several PID sensors needs to keep track of its
  /// previous value in order to calculate its current value. This example shows
  /// the easier way to implement such sensors, by providing the sum of user and
  /// system time of the current process.
  ///
  /// @code
  /// // demos/ex2/MySensor.h
  /// #ifndef MYSENSOR_H__
  /// #define MYSENSOR_H__
  ///
  /// #include <libec/sensor/SensorPid.h>
  ///
  /// class MySensor : public cea::PIDSensor {
  /// public:
  ///   MySensor();
  ///
  /// cea::sensor_t getValue(pid_t pid);
  ///
  ///   void  update(pid_t pid);
  /// };
  ///
  /// #endif
  /// @endcode
  ///
  /// @code
  /// // demos/ex2/MySensor.cpp
  /// #include "MySensor.h"
  ///
  /// #include <libec/tools/Tools.h>
  /// #include <fcntl.h>
  ///
  /// MySensor::MySensor() {
  ///   int fd;
  ///
  ///   _name = "MySensor User Time";
  ///   _alias = "UT";
  ///   _type = cea::U64;
  ///
  ///   fd = open("/proc/stat", O_RDONLY, 0);
  ///   _isActive = (fd != -1);
  ///   close(fd);
  /// }
  ///
  /// cea::sensor_t MySensor::getValue(pid_t pid) {
  ///   cea::sensor_t ret;
  ///
  ///   update(pid);
  ///   ret.U64 = _cvPIDMap[pid].U64 - _pvPIDMap[pid].U64;
  ///   return ret;
  /// }
  ///
  /// void MySensor::update(pid_t pid) {
  ///   int fd;
  ///   static char path[1024];
  ///   char sbuf[1024];
  ///   unsigned long int putime, pstime;
  ///   //dummy variables
  ///   int d_int;
  ///   char d_buf[200];
  ///   char d_char;
  ///   long d_long;
  ///   unsigned long d_ulong;
  ///
  ///   sprintf(path, "/proc/%d/stat", pid);
  ///
  ///   fd = open(path, O_RDONLY, 0);
  ///   if (fd == -1)
  ///     return;
  ///   read(fd, sbuf, 1023);
  ///   close(fd);
  ///
  ///   sscanf(sbuf, "%d (%s %c %d %d %d %d %d %lu %lu %lu %lu %lu "
  ///                "%lu %lu ", /* utime stime */
  ///          &d_int, &d_buf, &d_char, &d_int, &d_int, &d_int, &d_int, &d_int,
  ///          &d_long, &d_long, &d_long, &d_long, &d_long, &putime, &pstime);
  ///
  ///   _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
  ///   _cvPIDMap[pid].U64 = putime + pstime;
  /// }
  /// @endcode
  ///
  /// @code
  /// // demos/ex2/main.cpp
  /// #include "MySensor.h"
  ///
  /// #include <ctime>
  /// #include <cmath>
  ///
  /// int main() {
  ///   cea::PIDSensor* sensor;
  ///   pid_t pid;
  ///
  ///   sensor = new MySensor();
  ///
  ///   double load = 0.5f; // just a variable to inclease the cpu load
  ///
  ///   // check if the sensor was properly started
  ///   if (sensor->getStatus()) {
  ///       pid = getpid();
  ///       std::cout << "----------------------------" << std::endl;
  ///       std::cout << "Name: " << sensor->getName() << std::endl;
  ///       std::cout << "Alias: " << sensor->getAlias() << std::endl;
  ///       for (int i = 0; i < 10; i++)
  ///         {
  ///           std::cout << "pid: " << pid << " \tticks: " << time(NULL)
  ///               << " \tvalue:" << sensor->getValue(pid).U64 << std::endl;
  ///
  ///           // small cpu load for the sensor to change
  ///           for (int j = 1; j < 10000000; j++)
  ///               load = (100.1*sin(load)/log(j));
  ///
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
  ///
  ///////////////////////////////////////////////////////////////////////////////
  class PIDSensor : public Sensor
  {
  public:
    /// \brief Constructor
    /// Set name, alias and type of Sensor
    PIDSensor();

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    PIDSensor(const std::string &xmlTag);

    virtual
    ~PIDSensor();

    /// \brief Updates sensor's state.
    /// \param pid Process id. (-1 for all processes)
    virtual void
    updatePid(pid_t pid) = 0;

    /// @brief Gets sensor's current value
    /// @param pid Process ID (-1 for all processes)
    virtual sensor_t
    getValuePid(pid_t pid);

    /// \brief Adds a new PID entry into the current and previous maps.
    /// \param pid Process ID
    virtual void
    add(pid_t pid);

    /// \brief Removes a PID entry from the current and previous maps.
    /// \param pid Process ID
    virtual void
    remove(pid_t pid);
  };

} /* namespace cea */
#endif /* PIDSENSOR_H_ */

///////////////////////////////////////////////////////////////////////////////
///     @class cea::PIDSensor
///     @ingroup sensor
///////////////////////////////////////////////////////////////////////////////
