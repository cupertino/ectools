/*
 * SensorWattsUpMeter.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: adminuser
 */

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
static const char wu_dev[] = "/dev/ttyUSB0";
static const int wu_read_max_retry = 2;
static const string wu_result_delim = ",";

static size_t
trimwhitespace(char *out, size_t len, const char *str)
{
  if (len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while (isspace(*str))
    str++;

  if (*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace(*end))
    end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len - 1 ? (end - str) : len - 1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}

static int
readFromWu(int dev_fd, char *buffer, int bufferSize)
{
  int ret;
  char tmpBuffer[bufferSize];
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

  trimwhitespace(buffer, ret, tmpBuffer);

  return ret;
}

int
writeToWu(int dev_fd, const char *buffer, int bufferSize)
{
  return write(dev_fd, buffer, bufferSize);
}

namespace cea
{

  WattsUpPowerMeter::WattsUpPowerMeter() :
      bufferSize(256), buffer(new char[bufferSize])
  {
    _type = Float;

    _isActive = true;

  }

  WattsUpPowerMeter::~WattsUpPowerMeter()
  {
    delete[] buffer;
  }

  sensor_t
  WattsUpPowerMeter::getValue()
  {
    return _cValue;
  }

  void
  WattsUpPowerMeter::update()
  {

  }

  void
  WattsUpPowerMeter::experiment()
  {
    struct termios t;
    struct stat s;
    int ret;

    DebugLog::writeMsg(DebugLog::INFO, "WattsUpMeter",
        ("Checking device file: " + std::string(wu_dev)));

    ret = stat(wu_dev, &s);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          ("stat() device file: " + std::string(wu_dev)));
      return;
    }
    if (!S_ISCHR(s.st_mode))
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          "File is not a character device");
      return;
    }
    if (access(wu_dev, R_OK | W_OK) != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          "Device not accessible");
      return;
    }

    dev_fd = open(wu_dev, O_RDWR | O_NONBLOCK);
    if (dev_fd < 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          "Failed to open device");
      return;
    }

    ret = tcgetattr(dev_fd, &t);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          "Failed to get device attributes");
    }

    cfmakeraw(&t);
    cfsetispeed(&t, B115200);
    cfsetospeed(&t, B115200);
    tcflush(dev_fd, TCIFLUSH);

    t.c_iflag |= IGNPAR;
    t.c_cflag &= ~CSTOPB;
    ret = tcsetattr(dev_fd, TCSANOW, &t);
    if (ret != 0)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter",
          "Failed to set device attributes");
    }

    DebugLog::writeMsg(DebugLog::INFO, "WattsUpMeter", "Attributes set");

    std::string errorMsg;
    int samplingInterval = 1;
    WattsUpPowerMeter::WuCmdStatus status =
        WattsUpPowerMeter::setSamplingInterval(samplingInterval, &errorMsg);
    if (status == ERROR_REQUEST || status == ERROR_REPLY)
    {
      DebugLog::writeMsg(DebugLog::ERROR, "WattsUpMeter", errorMsg);
      return;
    }

    status = getSamplingInterval(&samplingInterval, &errorMsg);
    printf("Sampling interval = %d\n", samplingInterval);
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::sendRequestToWu(std::string cmd)
  {
    if (writeToWu(dev_fd, cmd.c_str(), cmd.length()) == -1)
    {
      return ERROR_REQUEST;
    }
    else
    {
      return NONE;
    }
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::sendRequestToWuAndReadReply(std::string cmd,
      std::string *reply)
  {
    int ret = writeToWu(dev_fd, cmd.c_str(), cmd.length());
    if (ret == errorValue)
    {
      return ERROR_REQUEST;
    }

    ret = readFromWu(dev_fd, buffer, bufferSize);
    if (ret == errorValue)
    {
      return ERROR_REPLY;
    }
    *reply = buffer;

    return NONE;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::setSamplingInterval(int interval, std::string *errorMsg)
  {
    std::stringstream sstm;
    sstm << "#S,W,2,_," << interval << ";";
    std::string setSamplingIntervalCmd = sstm.str();

    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWu(
        setSamplingIntervalCmd);
    if (status == ERROR_REQUEST)
    {
      *errorMsg = "Failed to set sampling interval";
    }

    return status;
  }

  WattsUpPowerMeter::WuCmdStatus
  WattsUpPowerMeter::getSamplingInterval(int *interval, std::string *errorMsg)
  {
    const char* failedToGetSamplingInterval = "Failed to get sampling interval";
    std::string getSamplingIntervalCmd = "#S,R,0;";
    std::string reply;
    WattsUpPowerMeter::WuCmdStatus status = sendRequestToWuAndReadReply(
        getSamplingIntervalCmd, &reply);
    std::cout << reply << std::endl;
    if (status == ERROR_REQUEST || status == ERROR_REPLY)
    {
      *errorMsg = failedToGetSamplingInterval;
    }
    *interval = atoi(getOneValueFromReply(reply, 5).c_str());
    return status;
  }

  string
  WattsUpPowerMeter::getOneValueFromReply(string reply, int positionToGet)
  {
    vector<int> positionsToGet(1, positionToGet);
    return getMultipleValuesFromReply(reply, positionsToGet).at(0);
  }

  vector<string>
  WattsUpPowerMeter::getMultipleValuesFromReply(string reply, vector<int> positionsToGet)
  {
    reply.erase(0, 1);
    reply.erase(reply.size() - 1, 1);
    vector<string> parts;
    boost::split(parts, reply, boost::is_any_of(wu_result_delim));
    vector<string> positions;
    for (vector<int>::size_type i = 0; i < positionsToGet.size(); i++)
    {
      int position = positionsToGet.at(i);
      positions.push_back(parts.at(position));
    }

    return positions;
  }
}

