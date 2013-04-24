#include <libec/sensor/SensorPid.h>
#include <map>

namespace cea
{

  PIDSensor::PIDSensor()
  {
  }

  PIDSensor::PIDSensor(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
  }

  PIDSensor::~PIDSensor()
  {
  }

  sensor_t
  PIDSensor::getValuePid(pid_t pid)
  {
//    // if map does not contains key ..
//    if (_cvPIDMap.find(pid) == _cvPIDMap.end())
//      add(pid);
//
//    updatePid(pid);
//
//    return _cvPIDMap[pid];
    sensor_t s;
    s.U64 = 0;
    return s;
  }

  void
  PIDSensor::add(pid_t pid)
  {
//    sensor_t cval; //, pval;
//    _cvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, cval));
//    _pvPIDMap.insert(std::pair<pid_t, sensor_t>(pid, pval));
  }

  void
  PIDSensor::remove(pid_t pid)
  {
//    _pvPIDMap.erase(pid);
//    _cvPIDMap.erase(pid);
  }

}
