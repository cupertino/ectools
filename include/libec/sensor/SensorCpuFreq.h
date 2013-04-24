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

#ifndef LIBEC_SENSORCPUFREQ_H__
#define LIBEC_SENSORCPUFREQ_H__

#include "Sensor.h"

namespace cea
{
  /// \brief Gets CPU frequency
  ///
  /// \details
  /// This sensor collects the current CPU frequency in KHz from the
  /// /sys/devices/system/cpu/cpuID/cpufreq/cpuinfo_cur_freq virtual file.
  /// To access this file, one need to have administrative permissions.
  /// This sensor has the latency as described into the
  /// cpuinfo_transition_latency file.
  ///
  /// \author     Leandro Fontoura Cupertino
  /// \date       Jul 1 2012
  /// \copyright  2012, CoolEmAll (INFSO-ICT-288701)
  class CpuFreq : public Sensor
  {

  public:
    static const char* ClassName;

    /// Constructor
    /// \param cpu_id   The identification of the cpu to collect data
    /// \param latency  Minimum time between updates, in milliseconds
    CpuFreq(unsigned short cpu_id = 0, suseconds_t latency = -1);

    CpuFreq(const std::string &xmlTag);

    CpuFreq(const CpuFreq &cf);

    /// Destructor
    ~CpuFreq();

    /// Updates CPU's frequency 'current value'
    void
    update();

    /// Current CPU's frequency value in KHz (u64)
    /// \return A sensor_t with the current value
    sensor_t
    getValue();

    /// Returns class name
    const char*
    getClassName();

    /// \brief Returns all parameters as a XML string
    /// \param indentation Indentation for each output line break
    std::string
    getParamsXml(const char* indentation = "");

    /// \brief Read the parameters from a XML file and set their values
    /// \param xmlTag  XML tag where the parameters may be found
    void
    setParamsXml(const char* xmlTag);

    /// Overloads output operator<<
    friend std::ostream&
    operator<<(std::ostream &out, CpuFreq &cPoint);

    /// Assignment operator
    CpuFreq&
    operator=(const CpuFreq& s);

  protected:
    /// Bring all sensor's variable back to its default ones
    virtual void
    clean();

    /// Copies the dource variables into the current sensor
    void
    copy(const CpuFreq &source);

    /// \brief Check if the sensor is active
    /// \return true if active, false otherwise
    bool
    checkActivity();

    /// Check if there is a privileged access to collect CPU's frequency info
    /// \param cpuId  CPU's identifier
    /// \return       true if the access is allowed, false otherwise
    bool
    checkRootAccess(unsigned short cpuId);

    /// Check if the user have access to the used files to collect the CPU's frequency
    /// \param cpuId  CPU's identifier
    /// \return       true if the access is allowed, false otherwise
    bool
    checkUserAccess(unsigned short cpuId);

    /// Gets the minimum latency which the sensor may operate.
    /// This value is besed on the the maximum transition latency from
    /// cpuinfo_transition_latency
    suseconds_t
    minLatency();

    /// Update sensor's value with privileged access
    /// \return updated value
    u64
    updateAsRoot();

    /// Update sensor's value with uses access
    /// \return updated value
    u64
    updateAsUser();

    /// Pointer to the function charged to update sensor's value
    /// \return updated value
    u64
    (CpuFreq::*updatePtr)(void);

    /// CPU's identifier
    unsigned _cpuId;

    /// Path to cpuinfo_cur_freq file
    std::string _filepath;
  };

}

#endif
