#include <libec/estimator/EstimatorObservable.h>

namespace cea
{

  /** +addObserver */
  void
  EstimatorObservable::addObserver(EstimatorObserver* obs)
  {
    /* Avoid add Empty observer */
    if (obs == 0)
      return;
    /* Check if observer not already present */
    for (std::list<EstimatorObserver*>::iterator it = _observerList.begin();
        it != _observerList.end(); ++it)
      {
        /* If observer founded then exit the function */
        if ((*it) == obs)
          return;
      }
    /* Add the Observer */
    _observerList.push_back(obs);
  }

  /** +removeObserver */
  void
  EstimatorObservable::removeObserver(EstimatorObserver* obs)
  {
    /* Search and remove the corresponding Observer */
    for (std::list<EstimatorObserver*>::iterator it = _observerList.begin();
        it != _observerList.end(); ++it)
      {
        if ((*it) == obs)
          {
            /* Remove and Exit the function */
            _observerList.erase(it);
            return;
          }
      }
  }

  /** +clearObserver */
  void
  EstimatorObservable::clearObserver()
  {
    _observerList.clear();
  }

  /** +notifyObservers */
  void
  EstimatorObservable::notifyObservers(pid_t pid, power_t value)
  {
    /* Notify all the ModelObserver concerned */
    for (std::list<EstimatorObserver*>::iterator it = _observerList.begin();
        it != _observerList.end(); ++it)
      {
        /* Check if the observer waiting this pid or not */
        if ((((*it)->_pidObserved == 0) || ((*it)->_pidObserved == pid))
            && ((*it)->_lastValue != value))
          {
            (*it)->notify(pid, value);
          }
      }
  }

}
