/*
 * SensorPowerWattsUp.h
 *
 *  Created on: Jul 22, 2013
 *      Author: Miguel Ferreira (https://github.com/miguelaferreira)
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

    WattsUpPowerMeter(string wu_device);

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
      NO_ERROR, ERROR_REQUEST, ERROR_REPLY, ERROR_READ
    };

    int deviceFileDescriptor;
    int bufferSize;
    char* buffer;

    bool
    initWattsUp(string wu_device);

    WuCmdStatus
    sendRequestToWu(string cmd);

    WuCmdStatus
    sendRequestToWuAndReadReply(string cmd, std::string *reply);

    WuCmdStatus
    sendRequestToWuAndReadReplies(string cmd, unsigned int replyCount, vector<string> *replies);

    WuCmdStatus
    setSamplingInterval(int interval, string *errorMsg);

    WuCmdStatus
    getSamplingInterval(int *interval, string *errorMsg);

    WuCmdStatus
    getHeaderRecord(vector<string> headerRecord, string *errorMsg);

    WuCmdStatus
    setLoggedFileds(vector<int> loggedFields, string *errorMsg);

    WuCmdStatus
    getLoggedFileds(vector<int> loggedFields, string *errorMsg);

    WuCmdStatus
    resetMemory(string *errorMsg);

    WuCmdStatus
    softRestart(string *errorMsg);

    WuCmdStatus
    terminateTransaction(string *errorMsg);

    WuCmdStatus
    setInternalLogging(int interval, string *errorMsg);

    WuCmdStatus
    setExternalLoggingAndCollectData(int interval, int replyCount, vector<string> *data, string *errorMsg);

    WuCmdStatus
    setExternalLogging(int interval, string *errorMsg);

    WuCmdStatus
    setDeviceToLogPower(string *errorMsg);

    WuCmdStatus
    readPower(float *power, string *errorMsg);

    string
    getOneValueFromReply(string reply, int positionsToGet);

    vector<string>
    getMultipleValuesFromReply(string reply, vector<int> positionsToGet);
    void
    init(string wu_device);
  };
}

#endif /* SENSORPOWERWATTSUP_H_ */
