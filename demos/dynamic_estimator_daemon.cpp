#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#include<libec/tools/DebugLog.h>
//#include<libec/sensor/SensorPowerAcpi.h>
#include<libec/sensor/SensorPowerPlogg.h>
#include<libec/estimator/PEMinMaxCpu.h>

#include<libec/estimator/DPELRCpu.h>
#include<libec/estimator/DPELRCpuMem.h>

int
main()
{
  cea::DebugLog::create("demo.log");
  cea::DebugLog::clear();

  std::ofstream ofs;
  time_t t = 0;

//  cea::AcpiPowerMeter pm;
  cea::PloggPowerMeter pm;
  cea::MinMaxCpu mmcpe(18.75, 45);

  // initial values for the linear model
  double dp[2] =
    { 18.75, 45 - 18.75 };
  cea::DPELRCpu dpe_lr(pm, dp);
  dpe_lr.getLatency();

//  double dp[6] =
//    { 18.75, 45 - 18.75, 0, 0, 0, 0 };
//  cea::DPELRCpuMem dpe_lr(pm, dp);

  time_t tstep = 1; // step time(in seconds) in which the output file will be updated
  time_t tref = 30; // time in seconds  between two calibrations of the dynamic power estimator
  time_t tcal = 10; // time in seconds to calibrate the dynamic power estimator
  time_t tstart;
  time_t tnow;

//  unsigned lat = pm.getLatency();
//  tref = lat * .005;

  std::cout << tref << std::endl;

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

      ofs.open("/tmp/libec_daemon_demo");
      ofs << ss.str();
      ofs.close();

      if ((tref - t) < 0)
        {
          std::stringstream ss;
          // need to be a separated thread
          dpe_lr.collectData(tcal);
          cea::DebugLog::cout << "collect data: ok\n";
          dpe_lr.calibrate();
          cea::DebugLog::cout << "sensor calibration: ok\n";
          std::cout << dpe_lr << std::endl;
          tref += 30;
        }
      sleep(tstep);
    }

  return 0;
}
