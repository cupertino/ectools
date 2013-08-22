/*
 * CpuIdleStates.cpp
 *
 *  Created on: 3 juil. 2013
 *      Author: fontoura
 */

#include <cstdio>

#include <libec/sensor/SensorCpuStateTime.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>

namespace cea
{
  CpuStateTime::CpuStateTime(int stateId, int cpuId)
  {
    _cpuId = cpuId;
    _cstateId = stateId;

    _name = "CPU_TIME_IN_STATE";
    _alias = "CState_" + Tools::CStr(cpuId);
    _type = U64;

    _isActive = checkAccess();
  }

  CpuStateTime::~CpuStateTime()
  {
    // TODO Auto-generated destructor stub
  }

  bool
  CpuStateTime::checkAccess()
  {
    char _file[SYSFS_PATH_MAX];

    snprintf(_filepath, SYSFS_PATH_MAX, PATH_TO_CPU "cpu%u/cpuidle/state%u",
        _cpuId, _cstateId);

    if (access(_filepath, R_OK) == -1)
      {
        std::stringstream ss;
        ss << "Sensor " << _name << " could not access the path " << _filepath
            << ". Make sure you have cpuidle package installed.";
        DebugLog::writeMsg(DebugLog::ERROR, "CpuStates", ss.str().c_str());
        return false;
      }

    snprintf(_file, SYSFS_PATH_MAX, "%s/time", _filepath);
    if (access(_file, R_OK) == -1)
      {
        std::stringstream ss;
        ss << "Sensor " << _name << " could not access the file " << _file
            << ". Make sure you have cpuidle package installed.";
        DebugLog::writeMsg(DebugLog::ERROR, "CpuStates", ss.str().c_str());
        return false;
      }

    std::ifstream ifs;
    snprintf(_file, SYSFS_PATH_MAX, "%s/name", _filepath);
    ifs.open(_file);
    if (ifs.good())
      {
        ifs >> _cstateName;
        _name.append(_cstateName);
        _alias.append(_cstateName);
      }
    ifs.close();

    snprintf(_file, SYSFS_PATH_MAX, "%s/desc", _filepath);
    ifs.open(_file);
    if (ifs.good())
      ifs >> _cstateDesc;
    ifs.close();

    snprintf(_file, SYSFS_PATH_MAX, "%s/latency", _filepath);
    ifs.open(_file);
    if (ifs.good())
      ifs >> _cstateLatency;
    ifs.close();

    snprintf(_file, SYSFS_PATH_MAX, "%s/power", _filepath);
    ifs.open(_file);
    if (ifs.good())
      ifs >> _cstatePower;
    ifs.close();

    return true;
  }

  void
  CpuStateTime::update()
  {
    char _file[SYSFS_PATH_MAX];
    std::ifstream ifs;

    snprintf(_file, SYSFS_PATH_MAX, "%s/time", _filepath);
    ifs.open(_file);
    if (ifs.good())
      ifs >> _cstateTime;
    ifs.close();

    snprintf(_file, SYSFS_PATH_MAX, "%s/usage", _filepath);
    ifs.open(_file);
    if (ifs.good())
      ifs >> _cstateUsage;
    ifs.close();

    _cValue.U64 = _cstateTime;
  }

  sensor_t
  CpuStateTime::getValue()
  {
    return _cValue;
  }

  int
  CpuStateTime::getNumberofStates()
  {
    char filepath[SYSFS_PATH_MAX];
    int counter = 0;

    snprintf(filepath, SYSFS_PATH_MAX, PATH_TO_CPU "cpu0/cpuidle/state0");

    while (access(filepath, R_OK) == 0)
      {
        counter++;
        snprintf(filepath, SYSFS_PATH_MAX, PATH_TO_CPU "cpu0/cpuidle/state%d",
            counter);
      }
    return counter;
  }

  std::string
  CpuStateTime::getStateName()
  {
    return _cstateName;
  }

} /* namespace cea */
