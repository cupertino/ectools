#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>

#include <libec/machine-learning/MatrixInvese.h>
#include <libec/machine-learning/LinearRegression.h>

int
main()
{
  int size = 1; // size tem que ser maior que params .. avaliar o porque!
  int params = 2;

  double* in = new double[params];
  double* out = new double[1];
  double* w = new double[params + 1];
  cea::LinearRegression lr(params, 1, size);

  srand(time(NULL));
  for (int i = 0; i < size; i++)
    {
      out[0] = 10.5;
      for (int j = 0; j < params; j++)
        {
          in[j] = rand() % 100;
          out[0] += in[j] * j;
        }
      lr.addPattern(in, out);
    }

  // print input matrices
  std::cout << lr;
  std::cout << std::endl;

  if (lr.solve(w))
    {
      std::cout << "w =";
      for (int i = 0; i < params + 1; i++)
        std::cout << " " << w[i];
      std::cout << std::endl;
    }
  exit(EXIT_SUCCESS);
}
