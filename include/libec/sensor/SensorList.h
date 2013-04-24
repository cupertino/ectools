/*
 * SensorList.h
 *
 *  Created on: 28 mars 2013
 *      Author: fontoura
 */

#ifndef SENSORLIST_H_
#define SENSORLIST_H_

#include <list>
#include "Sensor.h"

namespace cea
{
  /// A list of sensors.
  class SensorList : public std::list<Sensor*>
  {
  public:
    /// Constructor
    SensorList();
    /// Destructor.
    /// Deletes all added sensors.
    virtual
    ~SensorList();

    /// Adds sensor to the list.
    /// The sensor pointer will be deleted at the destructor,
    /// so it must not be deleted elsewhere. Code usage examples:
    /// \code
    /// SensorList sl;
    /// sl.add(new CpuTemp(1));
    /// \endcode
    bool
    add(Sensor &s);

  protected:
    void
    clean();

    void
    copy();
  };

} /* namespace cea */
#endif /* SENSORLIST_H_ */
