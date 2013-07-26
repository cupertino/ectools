/*
 * SensorPowerWattsUp.h
 *
 *  Created on: Jul 22, 2013
 *      Author: adminuser
 */

#ifndef SENSORPOWERWATTSUP_H_
#define SENSORPOWERWATTSUP_H_

#include "SensorPower.h"
#include <vector>
#include<string>

using namespace std;

namespace cea
{
  class WattsUpPowerMeter : public cea::PowerMeter
  {

  public:
    WattsUpPowerMeter();

    virtual
    ~WattsUpPowerMeter();

    sensor_t
    getValue();

    void
    update();

    void
    experiment();

  private:
    enum WuCmdStatus
    {
      NONE, ERROR_REQUEST, ERROR_REPLY
    };

    int dev_fd;
    int bufferSize;
    char* buffer;

    WuCmdStatus
    sendRequestToWu(string cmd);

    WuCmdStatus
    sendRequestToWuAndReadReply(string cmd,
        std::string *reply);

    WuCmdStatus
    setSamplingInterval(int interval, string *errorMsg);

    WuCmdStatus
    getSamplingInterval(int *interval, string *errorMsg);

    string getOneValueFromReply(string reply, int positionsToGet);

    vector<string> getMultipleValuesFromReply(string reply, vector<int> positionsToGet);
  };
}

#endif /* SENSORPOWERWATTSUP_H_ */
