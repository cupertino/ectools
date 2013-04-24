#include<libec/sensors.h>
#include<libec/sensor/SensorList.h>

int
main()
{
  cea::SensorList sl;
  cea::Sensor *ptr = new cea::CpuTime();

  if (sl.add(*ptr))
    {
      std::cout << "The sensor was added to the list." << std::endl;
      std::cout << (*sl.begin())->getAlias();
      std::cout << std::endl;
    }
  else
    {
      delete ptr;
      std::cout << "The sensor was not added." << std::endl;
    }

//  std::cout << time(NULL) << std::endl;
//
//  // Works properly
//  std::list<cea::Sensor*> list;
//  cea::AcpiPowerMeter* apm = new cea::AcpiPowerMeter();
//  cea::SensorCpuUsage* cu = new cea::SensorCpuUsage();
//
//  list.push_back(apm);
////  list.push_back(new cea::RecsPowerMeter());
//  list.push_back(cu);
//
////  for (std::list<cea::Sensor*>::iterator it = list.begin(); it != list.end();
////      it++)
////    {
////      std::cout << (*it)->getClassName() << std::endl;
////      delete (*it);
////    }
//
//  delete apm;
//  delete cu;
//
//  std::cout << time(NULL) << std::endl;

  return 0;
}
