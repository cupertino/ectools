#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <stdlib.h>
#include <sys/time.h>
#include <dirent.h>

// -----------------------------------------------------------------------------
// TOOLS - Function
// -----------------------------------------------------------------------------

namespace Tools
{

  /// Convert a number into a string
  template<typename T>
    std::string
    CStr(const T& data)
    {
      std::ostringstream oss;
      oss << data;
      return oss.str();
    }

  /// Convert number into a string
  template<typename T>
    void
    CStrTo(const std::string& data, T& to)
    {
      std::stringstream oss;
      oss << data;
      oss >> to;
    }

  /// Check if a c-string is a number
  bool
  isNumeric(const char* str)
  {
    /* Check if is number */
    while ((str != 0) && (*str != '\0'))
      {
        if ((*str < '0') || (*str > '9'))
          return false;
        str++;
      }
    /* Is a Number return true */
    return true;
  }

  /// Show the elapsed time
  void
  printElapsedTime(struct timeval& tick, unsigned long long int processRead)
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long int diff = (tv.tv_usec + 1000000 * tv.tv_sec)
        - (tick.tv_usec + 1000000 * tick.tv_sec);
    printf("%ld.%06ld s", diff / 1000000, diff % 1000000);

    /* FrameRate */
    std::cout << "Rate: " << (diff / processRead) << " us/p, ";
  }

}

// -----------------------------------------------------------------------------
// Process - Function
// -----------------------------------------------------------------------------
namespace Proc
{
  typedef void
  (*procInfoFunction)(pid_t, const std::string&);
  typedef void
  (*benchFunctor)(pid_t, procInfoFunction);

  inline void
  getStatName(pid_t pid, const std::string& data)
  {

    /* Get the name */
    std::string retString;
    unsigned int start = 0, max = data.length();
    /* Found the first '(' */
    for (; start < max; start++)
      {
        if (data[start] == '(')
          {
            start++;
            break;
          }
      }
    /* Found the last ')' */
    for (; max > start; max--)
      {
        if (data[max] == ')')
          break;
      }
    /* Return the name*/
    retString.assign(data, start, max - start);
  }

  inline void
  openStat(pid_t _pid, procInfoFunction fun)
  {
    /* Open the cmdline file */
    std::string file = "/proc/" + Tools::CStr(_pid) + "/stat";
    std::ifstream f(file.c_str());
    std::string data;

    /* If Opened extract Path */
    if (f.is_open())
      {

        /* Get the first line */
        if (f.good())
          std::getline(f, data);

        /* Close the file */
        f.close();

      }

    /* Call the retrieve data function */
    if (fun != 0)
      fun(_pid, data);
  }

  typedef std::map<pid_t, std::ifstream*> ProcStatList;
  ProcStatList storedStatFile;

  /// Close all the opened file
  inline void
  clearProcStatList()
  {
    for (ProcStatList::iterator it = storedStatFile.begin();
        it != storedStatFile.end(); ++it)
      {
        if (it->second != 0)
          {
            /* Close the file */
            if (it->second->is_open())
              {
                it->second->close();
              }
            delete it->second;
          }
      }
    storedStatFile.clear();
  }

  inline void
  openStatStayOpen(pid_t _pid, procInfoFunction fun)
  {

    /* Open the cmdline file */
    std::string file = "/proc/" + Tools::CStr(_pid) + "/stat";
    std::ifstream* f;

    /* Search the corresponding file if already open */
    ProcStatList::iterator it = storedStatFile.find(_pid);
    if (it == storedStatFile.end())
      {
        /* Open the file */
        f = new std::ifstream(file.c_str());
        if (f == 0)
          return;
        /* Add to map */
        storedStatFile.insert(std::pair<pid_t, std::ifstream*>(_pid, f));
      }
    else
      {
        f = it->second;
      }

    std::string data;

    /* If Opened extract Path */
    if (f->is_open())
      {

        /* Rewind */
        f->seekg(0, std::ios_base::beg);

        /* Get the first line */
        if (f->good())
          std::getline(*f, data);

      }

    /* Call the retrieve data function */
    if (fun != 0)
      fun(_pid, data);
  }

  /// List all process and retrieve info of a process
  unsigned int
  getAllProcess(benchFunctor f, procInfoFunction fInfo = 0)
  {
    struct dirent *dir;
    long pid;
    char* endptr;
    unsigned int processCount = 0;
    /* Open proc dir */
    DIR* proc = opendir("/proc");

    /* Get all the numeric dir (corresponding to the pid) */
    while ((dir = readdir(proc)) != NULL)
      {
        if (Tools::isNumeric(dir->d_name))
          {
            /* Get the process id */
            pid = strtol(dir->d_name, &endptr, 10);
            if (*endptr != '\0')
              continue;
            /* Search the Process */
            f(pid, fInfo);
            //std::cout << retrieveProcessPath(pid) << std::endl;

            /* Grow the process counter */
            processCount++;
          }
      }

    /* Close the proc dir */
    closedir(proc);

    return processCount;
  }

}

// -----------------------------------------------------------------------------
// Benchmark
// -----------------------------------------------------------------------------

int
main()
{
  // Initialization
  struct timeval tick;
  unsigned int maxLoop = 1000;
  unsigned long long int processRead;
  Proc::benchFunctor fun;
  Proc::procInfoFunction funInfo = &(Proc::getStatName);

  // ----------------------------------------------
  // Benchmark 1

  // Init
  processRead = 0;
  fun = &(Proc::openStat);
  gettimeofday(&tick, NULL);

  // List all process and retrieve infos (open/close stat file)
  for (unsigned int i = 0; i < maxLoop; i++)
    {
      processRead += Proc::getAllProcess(fun, funInfo);
    }

  // Show benchmark result
  std::cout << "Open/Close: Retrieve the name of ~" << processRead
      << " process in ";
  Tools::printElapsedTime(tick, processRead);
  std::cout << std::endl;

  // ----------------------------------------------
  // Benchmark 2

  // Init
  processRead = 0;
  fun = &(Proc::openStatStayOpen);
  gettimeofday(&tick, NULL);

  // List all process and retrieve infos (open/close stat file)
  for (unsigned int i = 0; i < maxLoop; i++)
    {
      processRead += Proc::getAllProcess(fun, funInfo);
    }

  // Show benchmark result
  std::cout << "Stay open stat file: Retrieve the name of ~" << processRead
      << " process in ";
  Tools::printElapsedTime(tick, processRead);
  std::cout << std::endl;

  return 0;
}
