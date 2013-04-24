#ifndef GOOGLECHART_H_
#define GOOGLECHART_H_

#include <libec/monitors.h>

class GoogleChart
{
public:
  GoogleChart(cea::GridModel& model);

protected:
  void
  createCSS();

  void
  createHTML();

  void
  createJSON();

  cea::GridModel* _model;
};

#endif
