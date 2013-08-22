#include <libec/tools/FileTools.h>
#include <fcntl.h>
#include <unistd.h>

namespace cea
{
  unsigned int
  FileTools::sysfsReadFile(const char *path, char *buf, size_t buflen)
  {
    int fd;
    ssize_t numread;

    fd = open(path, O_RDONLY);
    if (fd == -1)
      return 0;

    numread = read(fd, buf, buflen - 1);
    if (numread < 1)
      {
        close(fd);
        return 0;
      }

    buf[numread] = '\0';
    close(fd);

    return (unsigned int) numread;
  }
}
