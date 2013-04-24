/*
 ectop - Terminal process monitoring tool.
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

#ifndef ECTOPMONITOR_H_
#define ECTOPMONITOR_H_

#include <libec/monitors.h>
#include <libec/sensors.h>
#include <libec/process.h>

namespace cea
{
  /// \brief ectop specific Monitor
  /// \author Leandro Fontoura Cupertino
  /// \author Dorian Rodde
  /// \date Aug 31 2012
  class MonitorEctop : public Monitor
  {
  public:
    // Members
    bool debugMode; ///< Debug staus (True = active)
    bool isFreezed; ///< Pause status (True = is paused)
    PowerMeter* pow; ///< Power Meter used to evaluate estimator's error
    PidStat* cpu; ///< Cpu information from /proc/\<pid\>/stat file
    Log jsonLog; ///< json file output (updated each timestep)

    // Column tags
    static const int PID = 0; ///< Process id column identifier
    static const int COMMAND = 1; ///< Command column identifier
    static const int SENSOR_U64 = 2; ///< Unsigned long long sensor column identifier
    static const int SENSOR_FLOAT = 3; ///< Float sensor column identifier
    static const int REALVALUE_ROW = 50; ///< Accumulated value of columns

    // Constructor
    MonitorEctop();

    /** Add a PID Sensor as a column */
    void
    addSensor(PIDSensor* s);

    /** Event launched after a row being created */
    void
    onRowCreate(Row& r);

    /** Event launched when a row need to be updated */
    void
    onRowUpdate(Row& r);

    /** Event launched before a row being deleted */
    void
    onRowDelete(Row& r);

    void
    beforeRenderViews();

    void
    afterRenderViews();

    void
    onUpdate();

    void
    saveToJsonFile(std::string path);
  };

} /* namespace cea */
#endif /* ECTOPMONITOR_H_ */
