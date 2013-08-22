#ifndef LIBEC_FILE_TOOLS_H__
#define LIBEC_FILE_TOOLS_H__

#include<stddef.h>

namespace cea
{
  /// @brief Class containing static file operation functions
  class FileTools
  {
  public:
    /// \brief Reads a sysfs File
    /// \param filename  File name
    static unsigned int
    sysfsReadFile(const char *path, char *buf, size_t buflen);
  };
}

#endif

