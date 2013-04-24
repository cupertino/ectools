#include <libec/tools/Color.h>

namespace cea
{

  /** Get a int from RGB */
  int
  Color::rgb(unsigned char r, unsigned char g, unsigned char b)
  {
    return (((b) << 16) | ((g) << 8) | (r));
  }

}
