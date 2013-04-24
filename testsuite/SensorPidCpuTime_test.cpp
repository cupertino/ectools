#include <libec/tools.h>
#include <libec/sensors.h>
#include <libec/Globals.h>

#include <dirent.h>
#include <math.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))

int
main()
{
  cea::CpuTime ct;
  pid_t pid;

  std::cout << "Testing class:        " << ct.ClassName << std::endl;
  pid = getpid();

  int tmp = pid;
  for (int i = 1; i < 10000000; i++)
    tmp *= sqrt(rand());

  std::cout << "  Machine total time: " << ct.getTotalTime() << std::endl;
  std::cout << "  Machine CPU time:   " << ct.getValue().U64 << std::endl;
  std::cout << "  Process Id:         " << pid << std::endl;
  std::cout << "  Process CPU time:   " << ct.getValuePid(pid).U64 << std::endl;
  std::cout << "  Class size (bytes): " << sizeof(ct) << std::endl;

  std::cout << "-----" << std::endl;
  std::stringstream ss;
  ss
      << "echo '/proc/stat (cpu usr nice sys idl iowait irq softirq steal guest)';";
  ss << "cat /proc/stat;";
  ss << "echo '/proc/" << pid << "/stat (14 and 15th columns)';";
  ss << "cat /proc/" << pid << "/stat;";
  std::cout << cea::Tools::exec(ss.str());
  std::cout << "-----" << std::endl;

  if (tmp == pid)
    std::cout << tmp << std::endl;

  cea::SensorController::storeXML(ct, "/tmp/cputime.xml");

  cea::Sensor* s;
  s = cea::SensorController::loadXmlFile("/tmp/cputime.xml");
  if (s != NULL)
    std::cout << s->toXml() << std::endl;

  delete s;

//  cea::PIDSensor* sensor;
//
//  struct dirent *dir;
//  int proc_count;
//
//  cea::DebugLog::create();
//  cea::DebugLog::clear();
//
//  sensor = new cea::CpuTime();
//
//  if (sensor->getStatus())
//    {
//      std::stringstream ss;
//      cea::u64 sys, usr;
//
//      long long pid_target = 0, pid_output = 0, pid_error = 0;
//      long long pid_sum = 0;
//      long long node_target = 0, node_output = 0, node_error = 0;
//      long long node_pidsum_error = 0;
//      pid_t pid;
//
//      for (int it = 0; it < 1; it++)
//        {
//          proc_count = 0;
//          // Open proc dir
//          DIR* proc = opendir("/proc");
//
//          node_output = sensor->getValuePid(-1).U64;
//
//          // Get all the numeric dir (corresponding to the pid)
//          while ((dir = readdir(proc)) != NULL)
//            {
//              if (cea::Tools::isNumeric(dir->d_name))
//                {
//                  ss.clear();
//                  ss.str("");
//                  ss << dir->d_name;
//                  ss >> pid;
//                  pid_output = sensor->getValuePid(pid).U64;
//
//                  ss.clear();
//                  ss.str("");
//                  ss << "awk '{print $14;}' /proc/" << pid << "/stat";
//                  cea::Tools::CStrTo(cea::Tools::exec(ss.str().c_str()), usr);
//                  ss.clear();
//                  ss.str("");
//                  ss << "awk '{print $15;}' /proc/" << pid << "/stat";
//                  cea::Tools::CStrTo(cea::Tools::exec(ss.str().c_str()), sys);
//                  pid_target = sys + usr;
//
//                  pid_sum += pid_output;
//
//                  pid_error += ABS(pid_target - pid_output);
//                }
//            }
//
//          closedir(proc);
//
//          ss.clear();
//          ss.str("");
//          ss << "awk 'NR==1{print $2;}' /proc/stat";
//          cea::Tools::CStrTo(cea::Tools::exec(ss.str().c_str()), usr);
//          ss.clear();
//          ss.str("");
//          ss << "awk 'NR==1{print $4;}' /proc/stat";
//          cea::Tools::CStrTo(cea::Tools::exec(ss.str().c_str()), sys);
//          node_target = sys + usr;
//
//          node_error += ABS(node_target - node_output);
//          node_pidsum_error += ABS(node_target - pid_sum);
//
//          usleep(10000); // wait for 10 ms
//        }
//
//      if (pid_error > 0)
//        std::cerr << "Cpu Time Sensor test1: FAILED! error=" << pid_error
//            << std::endl;
//      else
//        std::cout << "Cpu Time Sensor test1: PASSED!" << std::endl;
//
//      if (node_error > 0)
//        std::cerr << "Cpu Time Sensor test2: FAILED! error=" << node_error
//            << std::endl;
//      else
//        std::cout << "Cpu Time Sensor test2: PASSED!" << std::endl;
//
//      if (node_pidsum_error > 0)
//        std::cerr << "Cpu Time Sensor test3: FAILED! error="
//            << node_pidsum_error << std::endl;
//      else
//        std::cout << "Cpu Time Sensor test3: PASSED!" << std::endl;
//
//    }
//  else
//    std::cerr << "error: sensor could not be opened." << std::endl;
//  delete sensor;

  return 0;
}
