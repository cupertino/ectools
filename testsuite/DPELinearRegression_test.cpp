/*
 * DPECpuTime_test.cpp
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#include <libec/estimator/DPELinearRegression.h>
#include <libec/sensor/SensorPowerAcpi.h>

using namespace cea;

int
main()
{
  AcpiPowerMeter pm;
  double* dp;
  dp = new double[2];
  dp[0] = 0;
  dp[1] = 1;
  DPELinearRegression dpe_lr(1, dp, &pm);


  std::cout << dpe_lr << std::endl;
  std::cout << "Estimated:  " << dpe_lr.getValue().Float << std::endl;
  std::cout << "PowerMeter: " << pm.getValue().Float << std::endl;

  std::cout << "---> collecting data ..." << std::endl;
  dpe_lr.collectData(120);
  std::cout << "---> calibrating ..." << std::endl;
  dpe_lr.calibrate();
  std::cout << dpe_lr << std::endl;

  std::cout << "Estimated:  " << dpe_lr.getValue().Float << std::endl;
  std::cout << "PowerMeter: " << pm.getValue().Float << std::endl;

  delete dp;

  return 0;
}
