#include <iostream>
#include <fstream>
#include <libec/device/SystemInfo.h>

using namespace cea;

int
main()
{
  SystemInfo::init();

  std::cout << "OS name:         " << SystemInfo::getName() << std::endl;
  std::cout << "OS release:      " << SystemInfo::getRelease() << std::endl;
  std::cout << "OS version:      " << SystemInfo::getVersion() << std::endl;
  std::cout << "OS architecture: " << SystemInfo::getArchitecture()
      << std::endl;

  std::cout << "Host name:       " << SystemInfo::getHostName() << std::endl;

  std::cout << "CPU total:       " << SystemInfo::getCpuCount() << std::endl;
  std::cout << "CPU dies:        " << SystemInfo::getCpuDies() << std::endl;
  std::cout << "CPU cores:       " << SystemInfo::getCpuCores() << std::endl;
//  std::cout << "CPU smt:         " << SystemInfo::getCpuCount() << std::endl;

  std::cout << "PID max:         " << SystemInfo::getPidMax() << std::endl;

  return 0;
}
