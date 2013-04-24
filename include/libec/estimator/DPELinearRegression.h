/*
 * DPELinearRegression.h
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#ifndef DPELINEARREGRESSION_H_
#define DPELINEARREGRESSION_H_

#include "DynamicPowerEstimator.h"
#include "../machine-learning/LinearRegression.h"

namespace cea
{

  class DPELinearRegression : public cea::DynamicPowerEstimator
  {
  public:
    DPELinearRegression(PowerMeter &pm, int params, double *weights = NULL);
    virtual
    ~DPELinearRegression();

    void
    calibrate();

    /// Collects history data during the a specific time
    /// \param secs Time in seconds
    void
    collectData(int secs);

    unsigned
    getLatency();

    sensor_t
    getValue();

    /// Overloads output operator<<
    friend std::ostream&
    operator<<(std::ostream &out, DPELinearRegression &cPoint);

    friend std::istream&
    operator>>(std::istream &in, DPELinearRegression &cPoint);

    void
    update();

    /// \brief Updates sensor's state.
    /// \param pid Process id. (-1 for all processes)
    void
    updatePid(pid_t pid);

  protected:
    void
    clean();

    void
    copy();

    LinearRegression _lr;
    double *_weights;
    int _params;
    long pm_latency;
  };

} /* namespace cea */
#endif /* DPELINEARREGRESSION_H_ */
