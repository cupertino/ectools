#ifndef LIBEC_GNUPLOT_FORMAT_H__
#define LIBEC_GNUPLOT_FORMAT_H__

#include <string>
#include "../base/BaseFormat.h"

namespace cea
{

  class GnuplotFormatter;

  class GnuplotFormat : public BaseFormat
  {
  public:

    /** Constructor */
    GnuplotFormat();
    GnuplotFormat(const std::string& separator);
    GnuplotFormat(const GnuplotFormat& fmtOpt);

    /** Create the Formatter */
    BaseFormatter*
    createFormatter() const;

    /** Friend class */
    friend class GnuplotFormatter;

  public:

    /** Option separator */
    std::string separator;

  };

}

#endif
