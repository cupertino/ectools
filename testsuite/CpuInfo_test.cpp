#include <iostream>
#include <libec/device/CpuInfo.h>

int
main()
{
  cea::CpuInfo cpuInfo;

  std::cout << "Number of cores: " << cpuInfo.getCpuCores() << std::endl;
  return 0;
}
