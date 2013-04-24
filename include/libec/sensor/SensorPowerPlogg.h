/*
 * PloggPowerMeter.h
 *
 *  Created on: 16 avr. 2013
 *      Author: fontoura
 */

#ifndef PLOGGPOWERMETER_H_
#define PLOGGPOWERMETER_H_

#include "SensorPower.h"

namespace cea
{

  class PloggPowerMeter : public cea::PowerMeter
  {
  public:
    PloggPowerMeter();

    virtual
    ~PloggPowerMeter();

    void
    update();

    sensor_t
    getValue();

  protected:
    int fd;
    char mac[6]; // Plogg's MAC address

    unsigned int
    get_val(unsigned char* buf, unsigned int pos);

    bool
    initPlogg(const char* mac);

    float
    get_power();
  };

} /* namespace cea */
#endif /* PLOGGPOWERMETER_H_ */
