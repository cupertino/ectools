#ifndef LIBEC_LINEAR_REGRESSION_H__
#define LIBEC_LINEAR_REGRESSION_H__

#include <libec/machine-learning/Calibrator.h>
#include <boost/numeric/ublas/matrix.hpp>

namespace cea
{
  class LinearRegression : public Calibrator
  {
  public:
    LinearRegression(int params, int outputs = 1, int buffSize = 10);
    ~LinearRegression();

    /// Adds a new pattern to the history database
    void
    addPattern(double* input, double *target);

    /// Set the maximum number of historical data to be fitted
    /// \param size  Maximum number of data
    void
    setBuffer(int size);

    /// Calculate the parameters of the linear regression
    bool
    solve(double *weights);

    template<class T>
      bool
      invertMatrixSize2(const boost::numeric::ublas::matrix<T> &input,
          boost::numeric::ublas::matrix<double> &inverse);

    /// Calculates the standard deviation for the last regression
    double
    std();

    /// Calculates the correlation factor of the last regression
    double
    r2();

    int
    countSamples();

    std::string toJson() const;
    std::string toXml() const;

    friend std::ostream&
    operator<<(std::ostream& os, const LinearRegression &lr);

  protected:
    int _buffSize;
    unsigned _row;
    unsigned _outputs;
    int _params;
    boost::numeric::ublas::matrix<double> _input;
    boost::numeric::ublas::matrix<double> _target;
  };
}
#endif
