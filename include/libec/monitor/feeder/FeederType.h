///////////////////////////////////////////////////////////////////////////////
/// @file		FeederType.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Type of monitor Feeder
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_FEEDERTYPE_H__
#define LIBEC_FEEDERTYPE_H__

#include <iostream>

namespace cea
{

  /// @brief Type of monitor Feeder
  ///
  /// Each time a new kind of monitor Feeder is create the FeederType enum
  /// need to be completed in order to indicate what a generic pointer
  /// corresponding to.
  ///
  enum FeederType
  {
    BASE_FEEDER_TYPE = 100000, ///< Base of feeder type

    /* List of type of Feeder item */
    FEEDER_PROCESS_ITEM, ///< Process feeder element
    PROCESS = FEEDER_PROCESS_ITEM, ///< Alias of process feeder element
    FEEDER_SENSOR_ITEM, ///< Sensor feeder element
    FEEDER_ESTIMATOR_ITEM ///< Estimator feeder element
  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@enum cea::FeederType
///	@ingroup monitor
///
///
///
///////////////////////////////////////////////////////////////////////////////
