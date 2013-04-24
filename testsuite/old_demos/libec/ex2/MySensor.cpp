// demos/ex2/MySensor.cpp
#include "MySensor.h"

#include <libec/tools/Tools.h>
#include <fcntl.h>

MySensor::MySensor()
{
  int fd;

  _name = "MySensor User Time";
  _alias = "UT";
  _type = cea::U64;

  fd = open("/proc/stat", O_RDONLY, 0);
  _isActive = (fd != -1);
  close(fd);
}

cea::sensor_t
MySensor::getValue(pid_t pid)
{
  cea::sensor_t ret;

  update(pid);
  ret.U64 = _cvPIDMap[pid].U64 - _pvPIDMap[pid].U64;
  return ret;
}

void
MySensor::update(pid_t pid)
{
  int fd;
  static char path[1024];
  char sbuf[1024];

  unsigned long int putime, pstime;

  //dummy variables
  int d_int;
  char d_buf[200];
  char d_char;
  long d_long;
  unsigned long d_ulong;

  sprintf(path, "/proc/%d/stat", pid);

  fd = open(path, O_RDONLY, 0);
  if (fd == -1)
    return;
  read(fd, sbuf, 1023);
  close(fd);

  sscanf(sbuf, "%d (%s %c "
      "%d %d %d %d %d "
      "%lu %lu %lu %lu %lu "
      "%lu %lu ", /* utime stime */
  &d_int, &d_buf, &d_char, &d_int, &d_int, &d_int, &d_int, &d_int, &d_long,
      &d_long, &d_long, &d_long, &d_long, &putime, &pstime);

  _pvPIDMap[pid].U64 = _cvPIDMap[pid].U64;
  _cvPIDMap[pid].U64 = putime + pstime;
}
