#include <dirent.h>
#include <stddef.h>
#include <fcntl.h>
#include <libec/tools.h>

//dummy variables
int d_int;
char d_buf[200];
char d_char;
long d_long;
unsigned long d_ulong;
unsigned long long d_ulonglong;

void
getPidStatCpuTime(unsigned long long &tcpu, unsigned long long &ttotal,
    unsigned long &tuser, unsigned long &tsys, unsigned long &tidle)
{
  struct dirent *dir;
  /* Open proc dir */
  DIR* proc = opendir("/proc");
  char path[1024], sbuf[1024];
  int fd;
  unsigned long int putime, pstime;
  unsigned long int pcutime, pcstime;

  tcpu = tsys = tuser = tidle = 0;

  // Get all the numeric dir (corresponding to the pid)
  while ((dir = readdir(proc)) != NULL)
    {
      if (cea::Tools::isNumeric(dir->d_name))
        {
          pid_t pid;
          std::stringstream ss;
          ss << dir->d_name;
          ss >> pid;
          sprintf(path, "/proc/%d/stat", pid);

          fd = open(path, O_RDONLY, 0);
          if (fd == -1)
            {
              std::cout << "error: file [" << path << "] could not be opened."
                  << std::endl;
              exit(-1);
            }
          read(fd, sbuf, 1023);
          close(fd);

          sscanf(sbuf,
              "%d (%s %c %d %d %d %d %d %lu %lu %lu %lu %lu "
                  "%lu %lu " // utime stime
                  "%ld %ld"// cutime cstime
              , &d_int, d_buf, &d_char, &d_int, &d_int, &d_int, &d_int, &d_int,
              &d_ulong, &d_ulong, &d_ulong, &d_ulong, &d_ulong, &putime,
              &pstime, &pcutime, &pcstime);

          tsys += pstime;
          tuser += putime;
          tidle += pcutime + pcstime;
        }
    }
  tcpu = tsys + tuser;
  ttotal = tcpu + tidle;
}

void
getCpuTime(unsigned long long &tcpu, unsigned long long &ttotal,
    unsigned long &tuser, unsigned long &tnice, unsigned long &tsys,
    unsigned long &tidle)
{
  char path[1024], sbuf[1024];
  int fd;

  sprintf(path, "/proc/stat");
  fd = open(path, O_RDONLY, 0);
  if (fd == -1)
    {
      std::cout << "error: file [" << path << "] could not be opened."
          << std::endl;
      exit(-1);
    }
  read(fd, sbuf, 1023);
  close(fd);

  sscanf(sbuf, "cpu %lu %lu %lu %lu", // %lu %lu %lu %lu %lu", /* utime stime */
      &tuser, &tnice, &tsys, &tidle);

  tcpu = tuser + tsys + tnice;
  ttotal = tuser + tsys + tnice + tidle;
}

int
main()
{
  unsigned long long tcpu = 0, ptcpu = 0;
  unsigned long long tcpu_pid = 0, ptcpu_pid = 0;
  unsigned long long ttotal = 0, pttotal = 0;
  unsigned long long ttotal_pid = 0, pttotal_pid = 0;
  unsigned long tuser = 0, ptuser = 0;
  unsigned long tuser_pid = 0, ptuser_pid = 0;
  unsigned long tnice = 0, ptnice = 0;
  unsigned long tsys = 0, ptsys = 0;
  unsigned long tsys_pid = 0, ptsys_pid = 0;
  unsigned long tidle = 0, ptidle = 0;
  unsigned long tidle_pid = 0, ptidle_pid = 0;

  getPidStatCpuTime(tcpu_pid, ttotal_pid, tuser_pid, tsys_pid, tidle_pid);
  getCpuTime(tcpu, ttotal, tuser, tnice, tsys, tidle);

  int wait_ms = 1000 / sysconf(_SC_CLK_TCK);

  std::cout << "kernel interruptions occurs every: " << wait_ms << "ms"
      << std::endl;

  std::cout << "          --[/proc/pid/stat]- ------[/proc/stat]-----"
      << std::endl;
  std::cout << "timestamp cpu ttl usr sys idl cpu ttl usr nic sys idl"
      << std::endl;

  while (1)
    {
      ptcpu_pid = tcpu_pid;
      ptuser_pid = tuser_pid;
      ptsys_pid = tsys_pid;
      ptcpu = tcpu;
      pttotal = ttotal;
      pttotal_pid = ttotal_pid;
      ptuser = tuser;
      ptnice = tnice;
      ptsys = tsys;
      ptidle = tidle;
      ptidle_pid = tidle_pid;

      sleep(1);

      getPidStatCpuTime(tcpu_pid, ttotal_pid, tuser_pid, tsys_pid, tidle_pid);
      getCpuTime(tcpu, ttotal, tuser, tnice, tsys, tidle);

      std::cout << time(NULL) << " ";
      std::cout << tcpu_pid - ptcpu_pid << " ";
      std::cout << ttotal_pid - pttotal_pid << " ";
      std::cout << tuser_pid - ptuser_pid << " ";
      std::cout << tsys_pid - ptsys_pid << " ";
      std::cout << tidle_pid - ptidle_pid << " ";
      std::cout << tcpu - ptcpu << " ";
      std::cout << ttotal - pttotal << " ";
      std::cout << tuser - ptuser << " ";
      std::cout << tnice - ptnice << " ";
      std::cout << tsys - ptsys << " ";
      std::cout << tidle - ptidle << std::endl;
    }
}
