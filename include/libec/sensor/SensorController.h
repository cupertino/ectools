//============================================================================
// Name        : SensorController.h
// Author      : Leandro Fontoura Cupertino
// Version     : 0.1
// Date        : 2012.06.01
// Copyright   : 2012, CoolEmAll (INFSO-ICT-288701)
// Description : Controls the sensors activities
//============================================================================

#ifndef SENSOR_CONTROLLER_H__
#define SENSOR_CONTROLLER_H__

#include "Sensor.h"
#include <list>

namespace cea
{
  class SensorController
  {

  public:
    std::list<cea::Sensor*>
    getAvailableSensors();

    static cea::Sensor*
    loadXmlFile(const char* filepath);

    static cea::Sensor*
    loadXmlString(const char* xmlstring);

    /// type W write, A append
    static bool
    storeXML(cea::Sensor &sensor, const char* filepath, char type = 'W');

    static void
    addSensor(Sensor* sensor);

    static cea::Sensor*
    createSensor(const char* classname);

    static void
    deleteSensors();

  private:
    static std::list<cea::Sensor*> _sensorList;
  };
}

#endif
