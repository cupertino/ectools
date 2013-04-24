/*
 ectools demos - Energy Consumption Library
 Copyright (C) 2013 by Leandro Fontoura Cupertino, IRIT
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

///////////////////////////////////////////////////////////////////////////////
// Demo description
//
// This demo describes how the user can extend the Sensor class to create his
// own sensor.
//
// New machine and application sensors can be easily implemented by
// extending the Sensor and SensorPID classes, respectively. To do so,
// the user must at least overload the update() and updatePid() methods,
// if specific data structures are used, it may be necessary to overload
// the getValue() and getValuePid() methods as well.
//
// This demo was presented at the EE-LSDS 2013, Energy Efficiency in Large
// Scale Distributed Systems conference.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <libec/sensor/Sensor.h>

///////////////////////////////////////////////////////////////////////////////
// MySensor class
///////////////////////////////////////////////////////////////////////////////
class MySensor : public cea::Sensor
{
public:
  MySensor()
  {
    _type = cea::U64;
    _isActive = true;
  }

  // The update method defines how the sensor change its state
  void
  update()
  {
    _cValue.U64 = rand();
  }

  // The getValue method returns the sensor's last updated value
  cea::sensor_t
  getValue()
  {
    return _cValue;
  }
};

#define ITER 10

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////
int
main()
{
  MySensor s;

  // check if the sensoris active
  if (s.getStatus() == true)
    {
      for (int i = 0; i < ITER; i++)
        {
          s.update();
          std::cout << s.getValue().U64 << std::endl;
          usleep(500000);
        }
    }

  return (0);
}
