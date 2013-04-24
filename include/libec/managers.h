///////////////////////////////////////////////////////////////////////////////
/// @file		managers.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Managers for sensor and estimator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_MANAGERS_H__
#define LIBEC_MANAGERS_H__

#include <iostream>
#include <string>

#include "manager/Manager.h"
#include "manager/ElementCreator.h"

namespace cea
{

  /// @brief Contains all the managers provided by the library
  ///
  /// By defaults these manager stay empty either if Sensor or Estimator are
  /// created. \n \n
  ///
  /// To use these Managers the developper have to add manually the Sensor
  /// or Estimator created and remove it when the element is deleted.
  ///
  namespace managers
  {
    /// @brief Manager of Estimator
    Manager estimators;
    /// @brief Manager of Sensor
    Manager sensors;
  }

}

#endif

///////////////////////////////////////////////////////////////////////////////
/// @namespace cea::managers
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////

