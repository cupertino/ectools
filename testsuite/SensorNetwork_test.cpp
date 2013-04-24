#include <iostream>

#include <libec/tools.h>
#include <libec/sensor/SensorController.h>
#include <libec/sensor/SensorNetwork.h>

int
main()
{
  cea::DebugLog::create();
  cea::DebugLog::clear();

  cea::Network* sensor[4];

  std::cout
      << "Test 1: class initialization and data verification from file.\n";
  sensor[0] = new cea::Network(cea::Network::SendBytes);
  sensor[1] = new cea::Network(cea::Network::SendPkt);
  sensor[2] = new cea::Network(cea::Network::ReceiveBytes);
  sensor[3] = new cea::Network(cea::Network::ReceivePkt);

  std::cout << "Sent bytes: " << sensor[0]->getValue().U64 << std::endl;
  std::cout << "Sent pkts:  " << sensor[1]->getValue().U64 << std::endl;
  std::cout << "Rcvd bytes: " << sensor[2]->getValue().U64 << std::endl;
  std::cout << "Rcvd pkts:  " << sensor[3]->getValue().U64 << std::endl;

  system("cat /proc/net/dev");

  std::cout << "\nTest 2: data variation.\n";
//  for (int i = 0; i < 10; i++)
    {
      sleep(1);
      std::cout << "Sent bytes: " << sensor[0]->getValue().U64 << std::endl;
      std::cout << "Sent pkts:  " << sensor[1]->getValue().U64 << std::endl;
      std::cout << "Rcvd bytes: " << sensor[2]->getValue().U64 << std::endl;
      std::cout << "Rcvd pkts:  " << sensor[3]->getValue().U64 << std::endl;
    }

  cea::SensorController::storeXML(*sensor[3], "/tmp/network.xml");
  cea::SensorController::storeXML(*sensor[1], "/tmp/network.xml", 'A');

  delete sensor[0];
  delete sensor[1];
  delete sensor[2];
  delete sensor[3];

  cea::Sensor *s;
  s = cea::SensorController::loadXmlFile("/tmp/network.xml");
  if (s != NULL)
    {
      std::cout << s->toXml();
    }
  delete s;

  return 0;
}
