/*
 * SensorCpuFrequency_test.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: Leandro
 */

#include <iostream>
#include <libec/tools/DebugLog.h> // if the debug log comes after the sensors.h we have compilations issues. need to check why.
#include <libec/sensors.h>
#include <libec/tools/MicroBenchmark.h>

int
main(int argc, char *argv[])
{
  std::cout << "microbenchmark\n";

  cea::MicroBenchmark::stressCpu(10);

  return 0;
}
