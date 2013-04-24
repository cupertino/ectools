#include <iostream>
#include <fstream>
#include <libec/sensor/SensorController.h>
#include <libec/sensors.h>

namespace cea
{
  std::list<cea::Sensor*> SensorController::_sensorList;

  void
  SensorController::addSensor(Sensor* sensor)
  {
    if (sensor != NULL)
      _sensorList.push_back(sensor);
  }

//  void
//  SensorController::removeSensor(Sensor* sensor)
//  {
//    _sensorList.remove(sensor);
//  }

  Sensor*
  SensorController::createSensor(const char* classname)
  {
    Sensor* sensor = NULL;

    if (classname == CpuFreq::ClassName)
      sensor = new CpuFreq();
    else if (classname == CpuTemp::ClassName)
      sensor = new CpuTemp();
    else if (classname == Network::ClassName)
      sensor = new Network();
    else if (classname == RunningProcs::ClassName)
      sensor = new RunningProcs();
    else if (classname == AcpiPowerMeter::ClassName)
      sensor = new AcpiPowerMeter();
    else if (classname == RecsPowerMeter::ClassName)
      sensor = new RecsPowerMeter();
    else if (classname == G5kPowerMeter::ClassName)
      sensor = new G5kPowerMeter();
    else if (classname == CpuTime::ClassName)
      sensor = new CpuTime();

    SensorController::addSensor(sensor);

    return sensor;
  }

  void
  SensorController::deleteSensors()
  {
    for (std::list<Sensor*>::iterator it = _sensorList.begin();
        it != _sensorList.end(); it++)
      {
        if ((*it) != NULL)
          delete (*it);
      }
  }

  cea::Sensor*
  SensorController::loadXmlFile(const char *filepath)
  {
    cea::Sensor* sensor = NULL;

    if (access(filepath, R_OK) == -1)
      {
        return sensor;
      }

    std::ifstream ifs(filepath);
    if (ifs.good())
      {
        int length;
        char *buffer;
        std::string str;

        // get length of file:
        ifs.seekg(0, std::ios::end);
        length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        // allocate memory:
        buffer = new char[length];

        // read data as a block:
        ifs.read(buffer, length);
        ifs.close();

        str = std::string(buffer, length);
        delete[] buffer;

        std::string sensorXmlTag;
        unsigned found, found2;
        found = str.find("<sensor ");
        found2 = str.substr(found).find("</sensor>");
        if ((found != std::string::npos) && (found2 != std::string::npos))
          {
            // get sensor tag
            sensorXmlTag = str.substr(found, found2 + 9);

            // get class name
            std::string classname;
            found = sensorXmlTag.find("class=\"");
            found2 = sensorXmlTag.substr(found + 7).find("\"");
            if ((found != std::string::npos) && (found2 != std::string::npos))
              classname = sensorXmlTag.substr(found + 7, found2);

            // To be able to create a class based on its name, one need to include its
            // classname on the following if/else chain
            if (classname == CpuFreq::ClassName)
              sensor = new CpuFreq(sensorXmlTag);
            else if (classname == CpuTemp::ClassName)
              sensor = new CpuTemp(sensorXmlTag);
            else if (classname == Network::ClassName)
              sensor = new Network(sensorXmlTag);
            else if (classname == RunningProcs::ClassName)
              sensor = new RunningProcs(sensorXmlTag);
            else if (classname == AcpiPowerMeter::ClassName)
              sensor = new AcpiPowerMeter(sensorXmlTag);
            else if (classname == RecsPowerMeter::ClassName)
              sensor = new RecsPowerMeter(sensorXmlTag);
            else if (classname == G5kPowerMeter::ClassName)
              sensor = new G5kPowerMeter(sensorXmlTag);
            else if (classname == CpuTime::ClassName)
              sensor = new CpuTime(sensorXmlTag);
          }

      }

    return sensor;
  }

  bool
  SensorController::storeXML(Sensor &sensor, const char* filepath, char type)
  {
    std::ofstream outfile;

    if (type == 'A')
      {
        if (access(filepath, W_OK) == -1)
          return false;
        outfile.open(filepath, std::ofstream::app);
      }
    else
      outfile.open(filepath, std::ofstream::trunc);

    outfile << sensor.toXml();

    outfile.flush();
    outfile.close();

    return true;
  }
}
