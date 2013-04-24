/*
 eclib - Energy Consumption Library
 Copyright (C) 2012 by Leandro Fontoura Cupertino, IRIT
 Author: Leandro Fontoura Cupertino <fontoura@irit.fr>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <libec/device/SystemInfo.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>
#include <libec/sensor/SensorRunningProcs.h>

#if DEBUG
#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>
#endif

namespace cea
{
  const char* RunningProcs::ClassName = "RunningProcs";

  RunningProcs::RunningProcs(bool activeOnly)
  {
    //Set name and alias
    if (activeOnly)
      {
        _name = "NUMBER_OF_ACTIVE_PROCESSES";
        _alias = "APROCS";
      }
    else
      {
        _name = "TOTAL_NUMBER_OF_PROCESSES";
        _alias = "TPROCS";
      }
    _activeOnly = activeOnly;

    _isActive = true;
    _type = U64;

#if DEBUG
    DebugLog::cout << this;
#endif
  }

  RunningProcs::RunningProcs(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
    setParamsXml(xmlTag.c_str());

    _isActive = true;
    _type = U64;
  }

  RunningProcs::~RunningProcs()
  {
  }

  void
  RunningProcs::update()
  {
#if DEBUG
    Debug::StartClock();
#endif

    if (_activeOnly)
      _cValue.U64 = SystemInfo::countActiveProc();
    else
      _cValue.U64 = SystemInfo::countProc();

#if DEBUG
    DebugLog::cout << _name << "  update time (us): "
        << Debug::ElapsedTimeUsec() << "\n";
    DebugLog::cout << _name << "  updated value: " << _cValue.U64 << "\n";
#endif
  }

  sensor_t
  RunningProcs::getValue()
  {
    return _cValue;
  }

  inline const char*
  RunningProcs::getClassName()
  {
    return ClassName;
  }

  std::string
  RunningProcs::getParamsXml(const char* indentation)
  {
    std::stringstream ss;
    ss << indentation << "<type_id value=\"" << _activeOnly << "\"/>"
        << std::endl;
    return ss.str();
  }

  void
  RunningProcs::setParamsXml(const char* xmlTag)
  {
    XMLReader::readSingleValuedTag(xmlTag, "type_id", _activeOnly);
  }

}
