#include <libec/estimator/EstimatorObserver.h>
#include <libec/estimator/EstimatorObservable.h>

namespace cea
{

  /** Default Constructors */
  EstimatorObserver::EstimatorObserver() :
      _pidObserved(0), _lastValue(0)
  {
    ;
  }

  /** Constructor */
  EstimatorObserver::EstimatorObserver(pid_t processIdObserved) :
      _pidObserved(processIdObserved), _lastValue(0)
  {
    ;
  }

  /** Get Process Observed */
  pid_t
  EstimatorObserver::getProcessObserved()
  {
    return _pidObserved;
  }

  /** Set Process Observed */
  void
  EstimatorObserver::setProcessObserved(pid_t processIdObserved)
  {
    _pidObserved = processIdObserved;
    _lastValue = 0;
  }

  /** +notify(newValue : power_t) */
  void
  EstimatorObserver::notify(pid_t pid, power_t newValue)
  {
    /* Event only if different value */
    if (_lastValue != newValue)
      {
        /* Raise the event */
        onValueChanged(pid, _lastValue, newValue);
        /* Change the Last Value */
        _lastValue = newValue;
      }
  }

}
