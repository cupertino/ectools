#include <libec/formatter/gnuplot/GnuplotFormat.h>
#include <libec/formatter/gnuplot/GnuplotFormatter.h>

namespace cea
{

  /** +Constructor */
  GnuplotFormat::GnuplotFormat() :
      BaseFormat(Gnuplot), separator("\t")
  {
    ;
  }
  GnuplotFormat::GnuplotFormat(const std::string& separator) :
      BaseFormat(Gnuplot), separator(separator)
  {
    ;
  }
  GnuplotFormat::GnuplotFormat(const GnuplotFormat& fmtOpt) :
      BaseFormat(Gnuplot), separator(fmtOpt.separator)
  {
    ;
  }

  /** Create the Formatter */
  BaseFormatter*
  GnuplotFormat::createFormatter() const
  {
    return new GnuplotFormatter();
  }

}
