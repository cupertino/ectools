/*
 * SensorWattsUpMeter.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: adminuser
 */

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/detail/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fcntl.h>
#include <libec/Globals.h>
#include <libec/sensor/Sensor.h>
#include <libec/sensor/SensorPowerWattsUp.h>
#include <libec/tools/DebugLog.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <vector>

static const int errorValue = -1;
static const int wu_read_max_retry = 2;
static const char* wu_result_delim = ",";
static const int power_value_index = 3;
static const int power_factor = 10; // the device reports on tenths of watts

static int
readFromWu(int dev_fd, char *buffer, int bufferSize)
{
  int ret;
  char tmpBuffer[bufferSize];
  memset(tmpBuffer, 0, sizeof(char) * bufferSize);
  for (int i = 0; i < wu_read_max_retry; i++)
  {
    ret = read(dev_fd, &tmpBuffer, bufferSize);
    if (ret == errorValue)
    {
      sleep(1);
    }
    else
    {
      break;
    }
  }

  string tmpString = string(tmpBuffer);
  boost::algorithm::trim(tmpString);
  int size = tmpString.size();
  memcpy(buffer, tmpString.c_str(), size);
  buffer[size] = 0;

  return ret;
}

int
writeToWu(int dev_fd, const char *buffer, int bufferSize)
{
  return write(dev_fd, buffer, bufferSize);
}

namespace cea
{

  WattsUpPowerMeter::WattsUpPowerMeter()
  {
    init("/dev/ttyUSB0");
  }

  WattsUpPowerMeter::WattsUpPowerMeter(string wu_device)
  {
    init(wu_device);
  }

  void
  WattsUpPowerMeter::init(string wu_device)
  {
    bufferSize = 256;
    buffer = new char[bufferSize];
    deviceFileDescriptor = -1;

    _name = "WATTS_UP_POWER_METER";
    _alias = "PM_WU";

    _type = Float;
    _cValue.Float = 0.0;
    _isActive = initWattsUp(wu_device);
  }

  WattsUpPowerMeter::~WattsUpPowerMeter()
  {
    delete[] buffer;
    close(deviceFileDescriptor);
  }

  bool
  WattsUpPowerMeter::initWattsUp(string wuDevice)
  {
    struct termios t;
    struct stat s;
    int ret;

    DebugLog::writeMsg(DebugLog::INFO, "WattsUpMeter", ("Checking device file: " + wuDevice));
    const char* wuDeviceCharArray = wuDevice.c_str();
    ret = stat(wuDeviceCharArray, &s);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", ("stat() device file"));
      return false;
    }
    if (!S_ISCHR(s.st_mode))
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", "File is not a character device");
      return false;
    }
    if (access(wuDeviceCharArray, R_OK | W_OK) != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", "Device not accessible");
      return false;
    }

    deviceFileDescriptor = open(wuDeviceCharArray, O_RDWR | O_NONBLOCK);
    if (deviceFileDescriptor < 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", "Failed to open device");
      return false;
    }

    ret = tcgetattr(deviceFileDescriptor, &t);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", "Failed to get device attributes");
    }

    cfmakeraw(&t);
    cfsetispeed(&t, B115200);
    cfsetospeed(&t, B115200);
    tcflush(deviceFileDescriptor, TCIFLUSH);

    t.c_iflag |= IGNPAR;
    t.c_cflag &= ~CSTOPB;
    ret = tcsetattr(deviceFileDescriptor, TCSANOW, &t);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", "Failed to set device attributes");
    }

    string errorMsg;
    WattsUpPowerMeter::WuCmdStatus status;

    status = setDeviceToLogPower(&errorMsg);
    if (status != NO_ERROR)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", errorMsg);
      return false;
    }

    status = WattsUpPowerMeter::setExternalLogging(1, &errorMsg);
    if (status != NO_ERROR)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", errorMsg);
      return false;
    }

    usleep(1000);

    DebugLog::writeMsg(DebugLog::INFO, "WattsUpMeter", "Device ready and logging power");

    return true;
  }

  sensor_t
  WattsUpPowerMeter::getValue()
  {
    return _cValue;
  }

  void
  WattsUpPowerMeter::update()
  {
    float power;
    string errorMsg;
    WuCmdStatus status = readPower(&power, &errorMsg);
    if (status != NO_ERROR)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", errorMsg);
      return;
    }
    _cValue.Float = power;
  }


  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::sendRequestToWu(string cmd)
  {
    if (writeToWu(deviceFileDescriptor, cmd.c_str(), cmd.length()) == -1)
    {
      return ERROR_REQUEST;
    }
    else
    {
      return NO_ERROR;
    }
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::sendRequestToWuAndReadReply(string cmd, string *reply)
  {
    int ret = writeToWu(deviceFileDescriptor, cmd.c_str(), cmd.length());
    if (ret == errorValue)
    {
      return ERROR_REQUEST;
    }

    ret = readFromWu(deviceFileDescriptor, buffer, bufferSize);
    if (ret == errorValue)
    {
      return ERROR_REPLY;
    }
    *reply = buffer;

    return NO_ERROR;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::sendRequestToWuAndReadReplies(string cmd, unsigned int replyCount, vector<string> *replies)
  {
    int ret = writeToWu(deviceFileDescriptor, cmd.c_str(), cmd.length());
    if (ret == errorValue)
    {
      return ERROR_REQUEST;
    }

    for (unsigned int i = 0; i < replyCount; i++)
    {
      ret = readFromWu(deviceFileDescriptor, buffer, bufferSize);
      if (ret == errorValue)
      {
        return ERROR_REPLY;
      }
      replies->push_back(string(buffer));
    }

    return NO_ERROR;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setSamplingInterval(int interval, string *errorMsg)
  {
    ostringstream sstm;
    sstm << "#S,W,2,_," << interval << ";";
    string setSamplingIntervalCmd = sstm.str();

    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(setSamplingIntervalCmd);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = "Failed to set sampling interval";
    }

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::getSamplingInterval(int *interval, string *errorMsg)
  {
    const char* failedToGetSamplingInterval = "Failed to get sampling interval";
    string getSamplingIntervalCmd = "#S,R,0;";
    string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(getSamplingIntervalCmd, &reply);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToGetSamplingInterval;
    }
    printf("Getting one value from reply\n");
    std::string intervalStr = getOneValueFromReply(reply, 4);
    printf("intervalString = %s\n", intervalStr.c_str());
    *interval = atoi(intervalStr.c_str());
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::getHeaderRecord(vector<string> headerRecord, string *errorMsg)
  {
    const char* failedToGetHeaderRecord = "Failed to get header record";
    string getHeaderRecordCmd = "#H,R,0;";
    string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(getHeaderRecordCmd, &reply);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToGetHeaderRecord;
    }
    int headerRecordIndexes[18] =
    { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    vector<int> positionstoGet(headerRecordIndexes,
        headerRecordIndexes + sizeof headerRecordIndexes / sizeof headerRecordIndexes[0]);
    vector<string> values = getMultipleValuesFromReply(reply, positionstoGet);
    headerRecord.insert(headerRecord.end(), values.begin(), values.end());

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setLoggedFileds(vector<int> loggedFields, string *errorMsg)
  {
    const char* failedToSetLoggedFilds = "Failed to set logged fields";
    ostringstream sstm;
    sstm << "#C,W,";
    sstm << loggedFields.size() << ",";
    for (size_t i = 0; i < loggedFields.size(); ++i)
    {
      sstm << loggedFields.at(i);
      sstm << ",";
    }
    string setLoggedFieldsCmd = sstm.str();
    setLoggedFieldsCmd.erase(setLoggedFieldsCmd.size() - 1);
    setLoggedFieldsCmd += ";";

    string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(setLoggedFieldsCmd, &reply);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToSetLoggedFilds;
    }
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::getLoggedFileds(vector<int> loggedFields, string *errorMsg)
  {
    const char* failedToGetLoggedFilds = "Failed to get logged fields";
    string getLoggedFieldsCmd = "#C,R,0;";
    string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(getLoggedFieldsCmd, &reply);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = failedToGetLoggedFilds;
    }
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::resetMemory(string *errorMsg)
  {
    const char* failedToResetMemory = "Failed to reset memory";
    string getHeaderRecordCmd = "#R,W,0;";
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(getHeaderRecordCmd);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = failedToResetMemory;
    }
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::softRestart(string *errorMsg)
  {
    const char* failedToSoftRestart = "Failed to do soft restart";
    string getHeaderRecordCmd = "#V,W,0;";
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(getHeaderRecordCmd);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = failedToSoftRestart;
    }
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::terminateTransaction(string *errorMsg)
  {
    const char* failedToResetMemory = "Failed to terminate transaction";
    string terminateTransactionCmd = "\x018";
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(terminateTransactionCmd);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = failedToResetMemory;
    }
    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setInternalLogging(int interval, string *errorMsg)
  {
    const char* failedToSetInternalLogging = "Failed to set internal logging";
    ostringstream sstm;
    sstm << "#L,W,3,I,0," << interval << ";";
    string setExternalLogging = sstm.str();

    string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(setExternalLogging, &reply);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToSetInternalLogging;
    }
    printf("REPLY TO SET INTERNAL LOGGING: '%s'\n", reply.c_str());

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setExternalLoggingAndCollectData(int interval, int replyCount, vector<string> *data,
      string *errorMsg)
  {
    const char* failedToSetExternalLogging = "Failed to set external logging";

    ostringstream sstm;
    sstm << "#L,W,3,E,0," << interval << ";";
    string setExternalLogging = sstm.str();

    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReplies(setExternalLogging, replyCount, data);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToSetExternalLogging;
    }

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setExternalLogging(int interval, string *errorMsg)
  {
    const char* failedToSetExternalLogging = "Failed to set external logging";

    ostringstream sstm;
    sstm << "#L,W,3,E,0," << interval << ";";
    string setExternalLogging = sstm.str();

    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(setExternalLogging);
    if (status != NO_ERROR)
    {
      *errorMsg = failedToSetExternalLogging;
    }

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setDeviceToLogPower(string *errorMsg)
  {
    int logPower[] =
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    vector<int> logPowerVector(logPower, logPower + sizeof logPower / sizeof logPower[0]);
    return WattsUpPowerMeter::setLoggedFileds(logPowerVector, errorMsg);
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::readPower(float *power, string *errorMsg)
  {
    const char* failedToReadPower = "Failed to read power";

    *power = -1;
    for (int i = 0; i < wu_read_max_retry; i++)
    {
      int ret = readFromWu(deviceFileDescriptor, buffer, bufferSize);
      if (ret == errorValue)
      {
        *errorMsg = failedToReadPower;
        return ERROR_READ;
      }
      string powerStr = getOneValueFromReply(string(buffer), power_value_index);
      *power = atof(powerStr.c_str());
      *power = *power / power_factor;

      if(*power > 0) {
        break;
      }
    }
    return NO_ERROR;
  }

  string
  WattsUpPowerMeter::getOneValueFromReply(string reply, int positionToGet)
  {
    vector<int> positionsToGet(1, positionToGet);
    vector<string> positions = getMultipleValuesFromReply(reply, positionsToGet);
    if (positions.size() > 0)
    {
      return positions.at(0);
    }
    else
    {
      return string("");
    }
  }

  vector<string>
  WattsUpPowerMeter::getMultipleValuesFromReply(string reply, vector<int> positionsToGet)
  {
    vector<string> positions;
    if (!reply.empty())
    {
      reply.erase(0, 1);
      reply.erase(reply.size() - 1, 1);
      vector<string> parts;
      boost::split(parts, reply, boost::is_any_of(wu_result_delim));
      for (vector<int>::size_type i = 0; i < positionsToGet.size(); i++)
      {
        int position = positionsToGet.at(i);
        positions.push_back(parts.at(position));
      }
    }

    return positions;
  }

}

