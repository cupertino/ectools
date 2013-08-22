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
  cea::MinMaxCpu mmcpe(33, 57);

  time_t tstep = 1; // step time(in seconds) in which the output file will be updated
  time_t tstart;
  time_t tnow;

  tstart = time(NULL) - 1; // to skip the first moment
  while (1)
    {
      tnow = time(NULL);
      t = tnow - tstart;

      pm.update();
      mmcpe.update();

      std::stringstream ss;
      ss << static_cast<float>(t);
      ss << " " << pm.getValue().Float;
      ss << " " << mmcpe.getValue().Float;
      ss << std::endl;

      ofs.open("/tmp/libec_daemon_demo");
      ofs << ss.str();
      ofs.close();

      sleep(tstep);
    }

  return 0;
}
