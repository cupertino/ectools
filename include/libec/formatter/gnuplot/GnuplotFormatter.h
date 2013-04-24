#ifndef LIBEC_GNUPLOT_FORMATTER_H__
#define LIBEC_GNUPLOT_FORMATTER_H__

#include <iostream>
#include <string>

#include "../base/BaseFormatter.h"
#include "GnuplotFormat.h"

namespace cea
{

  class GnuplotFormatter : public BaseFormatter
  {
  public:

    /** Constructor */
    GnuplotFormatter();
    GnuplotFormatter(const GnuplotFormat& fmtOpt);

    /** Option Property */
    const GnuplotFormat&
    getOption() const;
    void
    setOption(const BaseFormat& fmtOpt);

    /** Begin new block */
    virtual void
    openBlock(const std::string& name);

    /** Format anything */
    virtual void
    format(const Value& data);

    /** Format with property name */
    virtual void
    format(const std::string& propertyName, const Value& data);

    /** End new block */
    virtual void
    closeBlock(const std::string& name);

    /** Peek formatted value */
    virtual void
    peek(std::string& resultFormattedString);

    /** Format a Comment */
    virtual void
    formatComment(const std::string& comment);

    /** Clear the formatted data */
    virtual void
    clear();

    /** Format string */
    void
    strToGnuplot(std::string& data);

  protected:

    /** Option of the formatter */
    GnuplotFormat _fmtOpt;

    /** Store the formatted value */
    std::string _formattedData;

  };

}

#endif
