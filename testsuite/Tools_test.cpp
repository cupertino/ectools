#include <iostream>
#include <fstream>
#include <libec/tools.h>

using namespace cea;

int
main()
{
  struct timeval tv1, tv2;

  gettimeofday(&tv1, NULL);
  usleep(2500);
  gettimeofday(&tv2, NULL);

  std::cout << "tv1: " << tv1.tv_sec << "(s) " << tv1.tv_usec << "(us)\n";
  std::cout << "tv2: " << tv2.tv_sec << "(s) " << tv2.tv_usec << "(us)\n";

  std::cout << "tv1-tv2: " << Tools::timevaldiff(&tv1, &tv2) << "(ms)\n";
  std::cout << "tv2-tv1: " << Tools::timevaldiff(&tv2, &tv1) << "(ms)\n";
  std::cout << "tv1-tv1: " << Tools::timevaldiff(&tv1, &tv1) << "(ms)\n";

  std::string str;

  str = "1.602";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = ".602";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "1.602e+12";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "1602";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "1602-";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "-1602";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "-160.2";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "+160.2";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  str = "e12";
  if (Tools::isNumeric(str))
    std::cout << "string: " << str << ", is numeric.\n";
  else
    std::cout << "string: " << str << ", is not a number.\n";

  return 0;
}
