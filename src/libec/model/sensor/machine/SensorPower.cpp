//============================================================================
// Name        : SensorPower.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
// Description : Power Sensor available on the G5K plataform
//============================================================================

#include <libec/sensor/SensorPower.h>
#include <libec/tools/Tools.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  PowerMeter::PowerMeter()
  {
    clean();
  }

  PowerMeter::PowerMeter(const std::string &xmlTag) :
      Sensor(xmlTag)
  {
  }

  unsigned
  PowerMeter::getLatency()
  {
    if (_latency == -1)
      {
        unsigned time;
        timeval tstart, tend; // Start and end times

        // wait 'till the first change on the power
        time = 0;
        waitUpdate();

        // keep track of the update time
        gettimeofday(&tstart, NULL);
        waitUpdate();
        gettimeofday(&tend, NULL);
        time = Tools::timevaldiff(tstart, tend);

        return time;
      }
    else
      return _latency;
  }

  void
  PowerMeter::waitUpdate()
  {
    float curr, prev;
    int count;

    update();
    curr = prev = getValue().Float;
    count = 0;
    while (curr == prev)
      {
        update();
        curr = getValue().Float;
        usleep(10000);
        count++;
        if (count > 12000)
          {
            DebugLog::writeMsg(DebugLog::ERROR, "PowerMeter::waitUpdate()",
                "The time spent waiting for a new value for the power "
                    "meter is greater than 2 minutes. Leaving the method..");
            return;
          }
      }
  }

  void
  PowerMeter::clean()
  {
    Sensor::clean();

    _latency = -1;
  }
}
