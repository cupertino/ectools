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

#ifndef LIBEC_SENSOR_RUNNING_PROCS_H__
#define LIBEC_SENSOR_RUNNING_PROCS_H__

#include "Sensor.h"

namespace cea
{
  /// \brief Retrieves the number of processes running on a machine
  class RunningProcs : public Sensor
  {

  public:
    /// Name of the class as a static parameter
    static const char* ClassName;

    /// Constructor
    /// \param activeOnly Tells if the sensor will collect all the existing processes
    ///         or only the one that executed an CPU instruction on the last timestep
    RunningProcs(bool activeOnly = false);

    /// Constructor
    /// \param xmlTag XML tag containing the parameters to load a sensor
    RunningProcs(const std::string &xmlTag);

    ~RunningProcs();

    void
    update();

    sensor_t
    getValue();

    /// \brief Get's the name of the class
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

  private:
    int _activeOnly;
  };

}

#endif
