/*
 * PloggPowerMeter.cpp
 *
 *  Created on: 16 avr. 2013
 *      Author: fontoura
 */

#define PLOGGDEV "/dev/rfcomm0"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

#include <libec/sensor/SensorPowerPlogg.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>

namespace cea
{

  PloggPowerMeter::PloggPowerMeter()
  {
    _type = Float;

    _isActive = initPlogg("00:80:98:E9:20:4E");
  }

  PloggPowerMeter::~PloggPowerMeter()
  {
    system("killall rfcomm");

    close(fd);
  }

  sensor_t
  PloggPowerMeter::getValue()
  {
    return _cValue;
  }

  void
  PloggPowerMeter::update()
  {
    unsigned char buf[56];
    int nb_octets;
    lseek(fd, 0, SEEK_END);
    nb_octets = write(fd, "yv\n", 3);
    int reste = 55;

    do
      {
        if ((nb_octets = read(fd, buf + 55 - reste, reste)) < 0)
          {
            DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter",
                "Access error. Check file read/write permission, "
                    " if your Plogg wattmeter on and that the"
                    " bluetooth connection is available through"
                    " 'rfcomm connect 0 <MAC_ADDRESS> 1'."
                    " The MAC_ADDRESS can be found with the 'hcitool scan' command.");
            return;
          }
        reste -= nb_octets;
      }
    while (reste != 0);

    // Verified
    float power = ((float) get_val(buf, 7)) / 1000;

    _cValue.Float = power;
  }

  unsigned int
  PloggPowerMeter::get_val(unsigned char* buf, unsigned int pos)
  {
    return buf[pos] << 24 | buf[pos + 1] << 16 | buf[pos + 2] << 8
        | buf[pos + 3];
  }

  bool
  PloggPowerMeter::initPlogg(const char* mac)
  {
    switch (fork())
      {
    case -1:
      DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter", "fork() error.");
      break;
    case 0:
      execlp("rfcomm", "rfcomm", "connect", "0", mac, "1", NULL);
      DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter", "execlp() error.");
      exit(1);
    default:
      {
//        int CR;
//        wait(&CR);
//        if (CR)
//          DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter",
//              "rfcomm() error.");
        sleep(10);
      }
      break;
      }
    system("stty 115200 -F /dev/rfcomm0");
    system("stty -crtscts -F /dev/rfcomm0");
    system("stty -hupcl clocal -F /dev/rfcomm0");
    system(
        "stty ignbrk ignpar -icrnl -ixon -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke -F /dev/rfcomm0");

    if ((fd = open(PLOGGDEV, O_RDWR)) == -1)
      {
        if (access(PLOGGDEV, F_OK) != 0)
          DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter", "Access error."
              " Check if your Plogg wattmeter is on and that the"
              " bluetooth connection is available through"
              " 'rfcomm connect 0 <MAC_ADDRESS> 1'."
              " The MAC_ADDRESS can be found with the 'hcitool scan' command.");
        else
          {
            if (access(PLOGGDEV, W_OK) != 0)
              DebugLog::writeMsg(DebugLog::ERROR, "PloggPowerMeter",
                  "Access error."
                      " Check if you have writing permissions granted on"
                      " the " PLOGGDEV " file.");
          }
        return false;
      }
    return true;
  }

}
/* namespace cea */
