#include <libec/Globals.h>
#include <libec/machine-learning/MatrixInvese.h>
#include <libec/machine-learning/LinearRegression.h>

#include <boost/numeric/ublas/io.hpp>

#if DEBUG
#include <libec/tools/DebugLog.h>
#endif

namespace cea
{
  LinearRegression::LinearRegression(int params, int outputs, int buffSize)
  {
    _row = 0;
    _buffSize = buffSize;
    _params = params;
    _outputs = outputs;

    // serialization
    std::stringstream s;
    s << _input;
    s >> _target;
  }

  LinearRegression::~LinearRegression()
  {
  }

  template<class T>
    bool
    LinearRegression::invertMatrixSize2(
        const boost::numeric::ublas::matrix<T> &input,
        boost::numeric::ublas::matrix<double> &inverse)
    {
      if ((input.size1() == 2) && (input.size2() == 2))
        {
          double det;
          det = input(0, 0) * input(1, 1) - input(0, 1) * input(1, 0);
          if (det > 1e-15)
            {
              det = 1 / det;
              inverse(0, 0) = det * input(1, 1);
              inverse(0, 1) = -1 * det * input(0, 1);
              inverse(1, 0) = -1 * det * input(1, 0);
              inverse(1, 1) = det * input(0, 0);

              return true;
            }
        }

      return false;
    }

  int LinearRegression::countSamples()
  {
    return ((int)_input.size1());
  }

  bool
  LinearRegression::solve(double *weights)
  {
#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO,
        "LinearRegression::solve(double *weights)", "-- in");
#endif



    int size = _input.size1();
    boost::numeric::ublas::matrix<double> Xtrans(_params + 1, size);
    boost::numeric::ublas::matrix<double> XtransX(_params + 1, _params + 1);
    boost::numeric::ublas::matrix<double> XtransXinv(_params + 1, _params + 1);
    boost::numeric::ublas::matrix<double> XtransXinvXtrans(_params + 1, size);

    Xtrans = boost::numeric::ublas::trans(_input);

#if DEBUG
    DebugLog::cout << "  inputs (X): " << _input << DebugLog::endl;
    DebugLog::cout << "  targets (y): " << _target << DebugLog::endl;
    DebugLog::cout << "  X': " << Xtrans << DebugLog::endl;
#endif

    XtransX = boost::numeric::ublas::prod(Xtrans, _input);

#if DEBUG
    DebugLog::cout << "  X'X: " << XtransX << DebugLog::endl;
#endif

    if (_params == 1)
      {
#if DEBUG
        DebugLog::cout
            << "  inv(X'X): -- in   invertMatrixSize2(XtransX, XtransXinv)"
            << DebugLog::endl;
#endif
        if (!invertMatrixSize2(XtransX, XtransXinv))
          return false;
#if DEBUG
        DebugLog::cout
            << "  inv(X'X): -- out  invertMatrixSize2(XtransX, XtransXinv)"
            << DebugLog::endl;
#endif
      }
    else
      {
#if DEBUG
        DebugLog::cout
            << "  inv(X'X): -- in   InvertMatrix(XtransX, XtransXinv)"
            << DebugLog::endl;
#endif
        if (!InvertMatrix(XtransX, XtransXinv))
          return false;
#if DEBUG
        DebugLog::cout
            << "  inv(X'X): -- out  InvertMatrix(XtransX, XtransXinv)"
            << DebugLog::endl;
#endif
      }

#if DEBUG
    DebugLog::cout << "  inv(X'X): " << XtransXinv << DebugLog::endl;
#endif

    XtransXinvXtrans = boost::numeric::ublas::prod(XtransXinv, Xtrans);

#if DEBUG
    DebugLog::cout << "  inv(X'X)X': " << XtransXinvXtrans << DebugLog::endl;
#endif

    boost::numeric::ublas::matrix<double> w(_params + 1, 1);
    w = boost::numeric::ublas::prod(XtransXinvXtrans, _target);

#if DEBUG
    DebugLog::cout << "  weights: " << w << DebugLog::endl;
#endif

    for (int i = 0; i < _params + 1; i++)
      weights[i] = w(i, 0);

#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO,
        "LinearRegression::solve(double *weights)", "-- out");
#endif
    return true;
  }

  void
  LinearRegression::addPattern(double *input, double *target)
  {
    int rows = _input.size1();

    if (rows < _buffSize)
      {
        _input.resize(rows + 1, _params + 1);
        _target.resize(rows + 1, _outputs);
      }

    _input(_row, 0) = 1;
    for (int i = 0; i < _params; i++)
      _input(_row, i + 1) = input[i];

    for (unsigned i = 0; i < _outputs; i++)
      _target(_row, i) = target[i];

    _row = (_row + 1) % _buffSize;
  }

  std::ostream&
  operator<<(std::ostream& os, const LinearRegression &lr)
  {
    os << "{";
    os << " \"params\": " << lr._params << ",";
    os << " \"buffer size\": " << lr._buffSize << ",";
    os << " \"input\": " << lr._input << ",";
    os << " \"target\": " << lr._target << "";
    os << " }";

    return os;
  }

}
