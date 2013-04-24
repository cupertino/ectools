///////////////////////////////////////////////////////////////////////////////
/// @file		EstimatorObservable.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Implementation of Observable pattern for Estimator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_ESTIMATOROBSERVABLE_H__
#define LIBEC_ESTIMATOROBSERVABLE_H__

#include <iostream>
#include <list>

#include "../Globals.h"
#include "EstimatorObserver.h"

namespace cea
{

  /// @brief Implementation of Observable pattern for Estimator
  class EstimatorObservable
  {
  public:

    /* Function */

    /// @brief Add an estimator observer
    /// @param obs Observer to add
    void
    addObserver(EstimatorObserver* obs);

    /// @brief Remove an estimator observer
    /// @param obs Observer to remove
    void
    removeObserver(EstimatorObserver* obs);

    /// @brief Clear all observer
    void
    clearObserver();

    /// @brief Notify all concerned observer
    /// @param pid Process Identificator considered
    /// @param value Value notified
    void
    notifyObservers(pid_t pid, power_t value);

  protected:

    /* Members */
    /// @brief List of estimator observer
    std::list<EstimatorObserver*> _observerList;

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::EstimatorObservable
///	@ingroup estimator
///
///////////////////////////////////////////////////////////////////////////////
