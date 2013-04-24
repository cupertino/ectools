///////////////////////////////////////////////////////////////////////////////
/// @file		EstimatorObserver.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class to observe an estimator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_ESTIMATOROBSERVER_H__
#define LIBEC_ESTIMATOROBSERVER_H__

#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "../Globals.h"

namespace cea
{
  /* Prototype */
  class Estimator;
  class EstimatorObservable;

  /// @brief Abstract class to observe an estimator
  class EstimatorObserver
  {
  public:

    /* Constructor */
    /// @brief Default Constructor observe all Estimator change
    EstimatorObserver();

    /// @brief Observe a specific process by an Estimator
    /// @param processIdObserved Process Identificator to observe
    EstimatorObserver(pid_t processIdObserved);

    virtual
    ~EstimatorObserver();

    /* Property */
    /// @brief Get Process Identificator observed
    /// @return Process identificator observed. \n
    ///         Or 0 if no specific process observed
    pid_t
    getProcessObserved();

    /// @brief Observe a specific process
    /// @param processIdObserved Process Identificator to observe
    void
    setProcessObserved(pid_t processIdObserved);

    /// @brief Notify event triggered by EstimatorObservable
    ///
    /// This function call onValueChanged only if value have changed.
    ///
    /// @param pid Process computed Or 0 if global value
    /// @param newValue New value of the process computed
    void
    notify(pid_t pid, power_t newValue);

    /* Friend */
    friend class EstimatorObservable;

  protected:

    /// @brief Event triggered after the value of the observed Estimator
    ///        have changed
    ///
    /// This function need to be overridden.
    ///
    /// @param pid Process computed Or 0 if global value
    /// @param lastValue Previous value computed
    /// @param newValue New value computed
    virtual void
    onValueChanged(pid_t pid, power_t lastValue, power_t newValue) = 0;

    /* Members */
    pid_t _pidObserved; ///< Pid observed 0 = all pid

    power_t _lastValue; ///< Last value computed of the observed Estimator

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::EstimatorObserver
///	@ingroup estimator
///
///////////////////////////////////////////////////////////////////////////////
