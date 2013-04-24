#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

//#include<libec/sensor/SensorPowerAcpi.h>
#include<libec/sensor/SensorPowerPlogg.h>
#include<libec/estimator/PEMinMaxCpu.h>
#include<libec/estimator/DPELinearRegression.h>
#include<libec/estimator/DPELRCpu.h>

int
main()
{
  std::ofstream ofs;
  unsigned t = 0;
//  cea::AcpiPowerMeter pm;
  cea::PloggPowerMeter pm;
  cea::MinMaxCpu mmcpe(18.75, 45);

  double dp[2] =
    { 18.75, 45 - 18.75 };
  cea::DPELRCpu dpe_lr(pm, dp);
//  cea::DPELinearRegression dpe_lr(pm, 1, dp);

  unsigned tstep = 1; // step time(in seconds) in which the output file will be updated
  unsigned tref = 30; // time in seconds  between two calibrations of the dynamic power estimator
  unsigned tcal = 10; // time in seconds to calibrate the dynamic power estimator
  time_t tstart;
  time_t tnow;

  std::cout << dpe_lr << std::endl;

  tstart = time(NULL) - 1; // to skip the first moment
  while (1)
    {
      tnow = time(NULL);
      t = tnow - tstart;

      pm.update();
      mmcpe.update();
      dpe_lr.update();

      std::stringstream ss;
      ss << static_cast<float>(t);
      ss << " " << pm.getValue().Float;
      ss << " " << mmcpe.getValue().Float;
      ss << " " << dpe_lr.getValue().Float;
      ss << std::endl;

      ofs.open("/tmp/fake_sensors");
      ofs << ss.str();
      ofs.close();

      if (t % tref == 0)
        {
          // need to be a separated thread
          dpe_lr.collectData(tcal);
          dpe_lr.calibrate();
          std::cout << dpe_lr << std::endl;
        }
      sleep(tstep);
    }

  return 0;
}
