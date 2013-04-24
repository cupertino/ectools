// demos/ex1/MySensor.cpp
#include <libec/tools/Tools.h>
#include <libec/sensor/SensorPid.h>
#include <iostream>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <cmath>

#include <libec/tools.h>
#include <libec/sensor/SensorPid.h>
#include <libec/sensor/SensorPidStat.h>
#include <libec/device/SystemInfo.h>

class MySensorMap : public cea::PIDSensor
{
public:
  MySensorMap()
  {
    _name = "Map";
    _alias = "M";
    _type = cea::U64;
    _isActive = true;
  }

  cea::sensor_t
  getValue(pid_t pid)
  {
    update(pid);
    return _cvPIDMap[pid];
  }

  void
  update(pid_t pid)
  {
    _cvPIDMap[pid].U64 = pid * rand();
  }

  long
  getMem()
  {
    return _cvPIDMap.size();
  }
};

class MySensorVector : public cea::PIDSensor
{
public:
  MySensorVector()
  {
    _name = "Vector";
    _alias = "V";
    _type = cea::U64;
    _isActive = true;
    cea::sensor_t s;
    s.U64 = 0;

    _cvPIDVect.assign(cea::SystemInfo::getPidMax(), s);
//    _cvPIDVect = new std::vector<cea::sensor_t>(cea::SystemInfo::getPidMax(),
//        0);
  }

  cea::sensor_t
  getValue(pid_t pid)
  {
    update(pid);
    return _cvPIDVect[pid];
  }

  void
  update(pid_t pid)
  {
    _cvPIDVect[pid].U64 = pid * rand();
  }

  long
  getMem()
  {
    return _cvPIDVect.size();
  }

private:
  //  std::vector<cea::sensor_t> *_cvPIDVect;
  std::vector<cea::sensor_t> _cvPIDVect;

};

// to compile:
// [wp5-t5.2] g++ -Lbin/libec.a -Iinclude -I/usr/src/linux-cea/tools/perf/ -lm -O0 testsuite/SensorPID_test.cpp bin/libec.a

int
main()
{
  MySensorVector* sensor0;
  MySensorMap* sensor1;
  int pid;
  time_t start;
  struct timeval tvstart, tvend;
  int pid_max;
  int iter = 2000000;

  cea::SystemInfo::init();
  pid_max = cea::SystemInfo::getPidMax();
  sensor0 = new MySensorVector();
  if (sensor0->getStatus())
    {
      gettimeofday(&tvstart, NULL);
      start = time(NULL);
      srand(1);
      for (int i = 0; i < iter; i++)
        {
          pid = rand() % pid_max;
          //        std::cout << "main: pid choosed is " << pid << std::endl;
          sensor0->getValue(pid);
        }
      gettimeofday(&tvend, NULL);
      std::cout << "Vect time:    " << tvend.tv_sec - tvstart.tv_sec << "'";
      if (tvend.tv_usec > tvstart.tv_usec)
        std::cout << tvend.tv_usec - tvstart.tv_usec;
      else
        std::cout << tvstart.tv_usec - tvend.tv_usec;
      std::cout << std::endl;
      std::cout << "Vect memory:  " << sensor0->getMem() << std::endl;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  sensor1 = new MySensorMap();
  if (sensor1->getStatus())
    {
      gettimeofday(&tvstart, NULL);
      start = time(NULL);
      srand(1);
      for (int i = 0; i < iter; i++)
        {
          pid = rand() % pid_max;
//          std::cout << "main: pid choosed is " << pid << std::endl;
          sensor1->getValue(pid);
        }
      gettimeofday(&tvend, NULL);
      std::cout << "Vect time:    " << tvend.tv_sec - tvstart.tv_sec << "'";
      if (tvend.tv_usec > tvstart.tv_usec)
        std::cout << tvend.tv_usec - tvstart.tv_usec;
      else
        std::cout << tvstart.tv_usec - tvend.tv_usec;
      std::cout << std::endl;
//      std::cout << "Map time:     " << time(NULL) - start << std::endl;
      std::cout << "Map memory:   " << sensor1->getMem() << std::endl;
    }
  else
    std::cerr << "error: sensor could not be opened." << std::endl;

  return 0;
}
