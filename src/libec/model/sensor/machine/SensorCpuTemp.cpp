#include <libec/tools.h>
#include <libec/sensor/SensorCpuTemp.h>
#include <libec/device/SystemInfo.h>

#include <dirent.h>
#include <stdio.h>
#include <string.h>

namespace cea
{
  ///////////////////////////////////////////////////////////////////
  // Static Members
  ///////////////////////////////////////////////////////////////////
  const char* CpuTemp::ClassName = "CpuTemp";

  ///////////////////////////////////////////////////////////////////
  // Public Members
  ///////////////////////////////////////////////////////////////////
  CpuTemp::CpuTemp(short cpuId)
  {
    std::string cpuIdStr;

    //Set private variables
    cpuIdStr = Tools::CStr(cpuId);
    _name = "CPU" + cpuIdStr + "_TEMPERATURE";
    _alias = "CPU" + cpuIdStr + "TEMP";
    _cpuId = cpuId;

    if (checkIntel())
      CpuTemp::updatePtr = &CpuTemp::updateIntel;
    else if (checkAmdk8())
      CpuTemp::updatePtr = &CpuTemp::updateAmdk8;
  }

  CpuTemp::CpuTemp(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
    setParamsXml(xmlTag.c_str());

    _type = Float;

    if (checkIntel())
      CpuTemp::updatePtr = &CpuTemp::updateIntel;
    else if (checkAmdk8())
      CpuTemp::updatePtr = &CpuTemp::updateAmdk8;
  }

  CpuTemp::CpuTemp(const CpuTemp &ct)
  {
    clean();
    copy(ct);
  }

  CpuTemp::~CpuTemp()
  {
  }

  inline const char*
  CpuTemp::getClassName()
  {
    return ClassName;
  }

  sensor_t
  CpuTemp::getValue()
  {
    return _cValue;
  }

  void
  CpuTemp::update()
  {
    _cValue.Float = (*this.*updatePtr)();
  }

  std::string
  CpuTemp::getParamsXml(const char* indentation)
  {
    std::stringstream ss;
    ss << indentation << "<cpu_id value=\"" << _cpuId << "\"/>" << std::endl;
    return ss.str();
  }

  void
  CpuTemp::setParamsXml(const char* xmlTag)
  {
    XMLReader::readSingleValuedTag(xmlTag, "cpu_id", _cpuId);
  }

  CpuTemp&
  CpuTemp::operator=(const CpuTemp& s)
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
  CpuTemp::clean()
  {
    Sensor::clean();

    _name = "CPU_TEMPERATURE";
    _alias = "CPUTEMP";
    _type = Float;
    _cpuId = 0;
    _filepath = "";
    updatePtr = NULL;
  }

  void
  CpuTemp::copy(const CpuTemp &source)
  {
    Sensor::copy(source);

    updatePtr = source.updatePtr;
    _cpuId = source._cpuId;
    _filepath = source._filepath;
  }

  bool
  CpuTemp::checkAmdk8()
  {
    // incomplete
    return (access("/sys/bus/pci/drivers/k8temp/", F_OK) == 0);
  }

  bool
  CpuTemp::checkIntel()
  {
    if (access("/sys/bus/platform/drivers/coretemp/", R_OK) == 0)
      {
        if (_cpuId >= 0)
          {
            std::stringstream ss;

            // Consider that there is only one die in and that the cpu's id start from 0
            ss << "/sys/bus/platform/drivers/coretemp/coretemp." << _cpuId
                << "/temp1_input";

            _filepath = ss.str();

            if (access(_filepath.c_str(), R_OK) == 0)
              _isActive = true;
            else
              DebugLog::cout << "error: The specified CPU (cpuId " << _cpuId
                  << ") has no temperature CPU sensor.\n";

          }
        /* old code -- dont work on recs .. why??
         if (_cpuId == -1)
         {
         // Consider that there is only one die (coretemp.0)
         _filepath =
         "/sys/bus/platform/drivers/coretemp/coretemp.0/temp1_input";

         if (access(_filepath.c_str(), R_OK) == 0)
         _isActive = true;
         else
         DebugLog::writeMsg(DebugLog::WARNING, "CpuTemp::checkIntel()",
         "The specified CPU has no die sensor.");
         }
         else
         {
         std::stringstream ss;

         // Consider that there is only one die in and that the cpu's id start from 0
         ss << "/sys/bus/platform/drivers/coretemp/coretemp.0/temp"
         << _cpuId + 2 << "_input";

         _filepath = ss.str();

         if (access(_filepath.c_str(), R_OK) == 0)
         _isActive = true;
         else
         DebugLog::cout << "error: The specified CPU has CPU (cpuId "
         << _cpuId << ") sensor.\n";
         }
         *
         */
        // Intel CPU's temperature sensor exists
        return true;
      }

    // Intel CPU's temperature sensor not available
    DebugLog::writeMsg(DebugLog::WARNING, "CpuTemp::checkIntel()",
        "Intel CPU's temperature sensor was not found. "
            "If you are using a Intel CPU, make sure you have the "
            "coretemp module runing on your system. You can retrieve this info "
            "with the 'lsmod | grep coretemp' command and execute the module "
            "with 'modprobe coretemp'.");
    return false;
  }

  float
  CpuTemp::updateAmdk8()
  {
    // incomplete
    return -1;
  }

  float
  CpuTemp::updateIntel()
  {
    std::ifstream ifs;
    double val = -1;

    ifs.open(_filepath.c_str());
    if (ifs.good())
      {
        ifs >> val;
        val /= 1000;
      }

    return val;
  }

}
