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

#if DEBUG
#include <libec/tools/Debug.h>
#include <libec/tools/DebugLog.h>
#endif

#include <typeinfo>
#include <fstream>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>
#include <libec/tools/XMLReader.h>
#include <libec/sensor/SensorCpuFreq.h>

namespace cea
{
  ///////////////////////////////////////////////////////////////////
  // Static Members
  ///////////////////////////////////////////////////////////////////
  const char* CpuFreq::ClassName = "CpuFreq";

  ///////////////////////////////////////////////////////////////////
  // Public Members
  ///////////////////////////////////////////////////////////////////
  CpuFreq::CpuFreq(unsigned short cpu_id, suseconds_t latency) :
      Sensor(latency)
  {
    clean();

    //Set name and alias
    std::string cpuIdStr;
    cpuIdStr = Tools::CStr(cpu_id);
    _name = "CPU" + cpuIdStr + "_FREQUENCY";
    _alias = "CPU" + cpuIdStr + "FREQ";

    _cpuId = cpu_id;
    _isActive = checkActivity();

    // if no latency was set, use the minimum latency possible
    if (latency == -1)
      latency = minLatency();
  }

  CpuFreq::CpuFreq(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
    setParamsXml(xmlTag.c_str());
    _type = U64;
    _isActive = checkActivity();
  }

  CpuFreq::CpuFreq(const CpuFreq &cf)
  {
    clean();
    copy(cf);
  }

  CpuFreq::~CpuFreq()
  {
    clean();
  }

  inline const char*
  CpuFreq::getClassName()
  {
    return ClassName;
  }

  std::string
  CpuFreq::getParamsXml(const char* indentation)
  {
    std::stringstream ss;
    ss << indentation << "<cpu_id value=\"" << _cpuId << "\"/>" << std::endl;
    return ss.str();
  }

  void
  CpuFreq::setParamsXml(const char* xmlTag)
  {
    XMLReader::readSingleValuedTag(xmlTag, "cpu_id", _cpuId);
  }

  void
  CpuFreq::update()
  {
    _cValue.U64 = (*this.*updatePtr)();
  }

  sensor_t
  CpuFreq::getValue()
  {
    return _cValue;
  }

  CpuFreq&
  CpuFreq::operator=(const CpuFreq& s)
  {
    if (this != &s)
      {
        clean();
        copy(s);
      }
    return *this;
  }

  ///////////////////////////////////////////////////////////////////
  // Protected Members
  ///////////////////////////////////////////////////////////////////
  void
  CpuFreq::clean()
  {
    Sensor::clean();

    _name = "CPU_FREQUENCY";
    _alias = "CPUFREQ";
    _type = U64;
    _cpuId = 0;
    _filepath = "";
  }

  void
  CpuFreq::copy(const CpuFreq &source)
  {
    Sensor::copy(source);

    updatePtr = source.updatePtr;
    _cpuId = source._cpuId;
    _filepath = source._filepath;
  }

  bool
  CpuFreq::checkActivity()
  {
    if (checkRootAccess(_cpuId))
      {
        CpuFreq::updatePtr = &CpuFreq::updateAsRoot;
        return true;
      }
    else if (checkUserAccess(_cpuId))
      {
        CpuFreq::updatePtr = &CpuFreq::updateAsUser;
        return true;
      }

    return false;
  }

  bool
  CpuFreq::checkRootAccess(unsigned short cpu_id)
  {
    std::string cpufreq;
    std::string cpuIdStr;

    cpuIdStr = Tools::CStr(cpu_id);

    cpufreq = "/sys/devices/system/cpu/cpu" + cpuIdStr + "/cpufreq/";

    // Set filepath
    _filepath = cpufreq + "cpuinfo_cur_freq";
    if (access(_filepath.c_str(), R_OK) == -1)
      {
        DebugLog::cout << "error: Sensor " << _name
            << " could not access the file " << _filepath
            << ". Make sure you have cpufreq-utils package installed and you "
                "have the proper permissions to acces the file.\n";
        return false;
      }

    return true;
  }

  bool
  CpuFreq::checkUserAccess(unsigned short cpu_id)
  {
    std::stringstream ss;
    std::string cpuIdStr;
    std::string cpuinfo_str;

    if (access("/proc/cpuinfo", R_OK) == -1)
      return false;

    cpuIdStr = Tools::CStr(cpu_id + 1);

    // Set filepath
    _filepath = "awk '/cpu MHz/ { ++x; if ( x == " + cpuIdStr
        + " ) print $4 }' /proc/cpuinfo";

    cpuinfo_str = cea::Tools::exec(_filepath.c_str());

    // remove return character from string
    unsigned found = cpuinfo_str.rfind("\n");
    if (found != std::string::npos)
      cpuinfo_str = cpuinfo_str.substr(0, found);

    return Tools::isNumeric(cpuinfo_str);
  }

  suseconds_t
  CpuFreq::minLatency()
  {
    std::stringstream cpufreq;
    std::ifstream ifs;
    suseconds_t min;

    cpufreq << "/sys/devices/system/cpu/cpu" << _cpuId
        << "/cpufreq/cpuinfo_transition_latency";

    ifs.open(cpufreq.str().c_str());
    if (ifs.good())
      {
        ifs >> min;
        // cpuinfo_transition_latency is given in nanoseconds
        min = min / 1000000 + 1;
      }
    else
      {
        DebugLog::cout << "warning: Sensor " << _name
            << " could not access the file " << cpufreq
            << ". Make sure you have cpufreq-utils package installed. Using "
            << _latency << " (ms) as sensor's latency.\n";
      }
    return min;
  }

  u64
  CpuFreq::updateAsRoot()
  {
    u64 val;

    std::ifstream ifs(_filepath.c_str());
    if (ifs.good())
      {
        ifs >> val;
        ifs.close();
      }
    return val;
  }

  u64
  CpuFreq::updateAsUser()
  {
    u64 val;
    std::string cpuinfo_str;

    cpuinfo_str = cea::Tools::exec(_filepath.c_str());

    // remove return character from string
    unsigned found = cpuinfo_str.rfind("\n");
    if (found != std::string::npos)
      cpuinfo_str = cpuinfo_str.substr(0, found);

    Tools::CStrTo(cpuinfo_str, val);
    val *= 1000;

    return val;
  }

  ///////////////////////////////////////////////////////////////////
  // Operators overload
  ///////////////////////////////////////////////////////////////////
  std::ostream&
  operator<<(std::ostream &out, CpuFreq &cPoint)
  {
    out << (Sensor&) cPoint;
    return out;
  }

}
