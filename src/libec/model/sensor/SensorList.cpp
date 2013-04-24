/*
 * SensorList.cpp
 *
 *  Created on: 28 mars 2013
 *      Author: fontoura
 */

#include <algorithm>

#include <libec/sensor/Sensor.h>
#include <libec/sensor/SensorList.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  SensorList::SensorList()
  {
  }

  SensorList::~SensorList()
  {
    clean();
  }

  bool
  SensorList::add(Sensor &s)
  {
    std::list<Sensor*>::iterator findIter = std::find(this->begin(),
        this->end(), &s);

    if (findIter == this->end())
      {
        if (s.getStatus())
          {
            this->push_back(&s);
            return true;
          }
        else
          DebugLog::writeMsg(DebugLog::ERROR, "SensorList::add(Sensor &s)",
              "Cannot add sensor to the list. The provided sensor is not active.");
      }
    return false;
  }

  void
  SensorList::clean()
  {
    // Delete all sensors from the list
/*
    for (std::list<Sensor*>::iterator it = this->begin(); it != this->end();
        it++)
      {
        if (*it != NULL)
          delete *it;
      }
*/
  }

} /* namespace cea */
