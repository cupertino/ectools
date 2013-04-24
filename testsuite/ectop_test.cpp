//============================================================================
// Name        : daq.cpp
// Author      : Dorian Rodde
// Version     : 0
// Date        : 2012.06.01
// Copyright   : Your copyright notice
// Description : A 'ps' like tool for energy consumption of processes.
// Requirement : Ncurses
//============================================================================

#include <iostream>
#include <string>

#include <libec/monitors.h>
#include <libec/process.h>
#include <libec/sensors.h>
#include <libec/estimators.h>

//#ifdef __FD_SETSIZE
//#undef __FD_SETSIZE
//#endif
//#define __FD_SETSIZE  65535

using namespace cea;

/* -------------------------------------------------------------------------- */
/* Monitor */

class EctopMonitor : public Monitor
{
public:
  // Column tags
  static const int PID = 0;
  static const int NAME = 1;
  static const int SENSOR_U64 = 2;
  static const int SENSOR_FLOAT = 3;
  static const int REALVALUE_ROW = 50;

  // Constructor
  EctopMonitor()
  {
    addColumn("PID", Value::INT, PID);
    this->getColumn(PID).setFixed(0, false);
    addColumn("COMMAND", Value::STRING, NAME, 30, 30);

//    addRow(REALVALUE_ROW).setSum(false).setValue(1, 58080808).setFixed(0, true);
  }

  // Add a PID Sensor to a column
  void
  addSensor(PIDSensor* s)
  {
    if (s->getStatus())
      {
        std::string name = s->getAlias();
        if (s->getType() == U64)
          {
            addMappedColumn(s, name, Value::INT, SENSOR_U64);
          }
        else
          {
            addMappedColumn(s, name, Value::DOUBLE, SENSOR_FLOAT);
          }
      }
  }

  // Event launched after a row beeing created
  void
  onRowCreate(Row& r)
  {
//    if (r.tag == PROCESS)
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        setValue(r, NAME, cast<Process>(r)->getName());
        setValue(r, PID, cast<Process>(r)->getPid());
      }
  }

  // Event launched when a row need to be updated
  void
  onRowUpdate(Row& r)
  {
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        Process& p = cast<Process>(r);
        // Update Sensor U64
        for (ColumnIterator<SENSOR_U64, PIDSensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValuePid(p.getPid()).U64);
          }
        // Update Sensor Float
        for (ColumnIterator<SENSOR_FLOAT, PIDSensor> s = this; s != 0; ++s)
          {
            setValue(r, s, s->getValuePid(p.getPid()).Float);
          }
      }
  }

  void
  onRowDelete(Row& r)
  {
    /*
     //TODO: remove pid from all pid sensors.
     if (r.tag == PROCESS)
     {
     Process& p = cast<Process>(r);

     for (ColumnMap::iterator it = mappedColumns.begin();
     it != mappedColumns.end(); it++)
     {
     cea::PIDSensor* s;
     s = cast<cea::PIDSensor>((*it).second);
     s->remove(p.getPid());
     }
     }*/
  }
};

/* -------------------------------------------------------------------------- */
/* Main */

int
main()
{
  // Model
  EctopMonitor m;
  ProcessEnumerator pe;
  // Connect Process enumerator to monitor
  m.connectRowFeeder(&pe);

  // View
  TermGridView view(m);

  // Controller
  TermGridCtrl termCtrl(view);

  // Application header
  Console::drawText("ectop  - " + Tools::nowToString(), 0, 0);
  Console::drawText(
      "\t(use '1'..'9' to sort, '='+'1'..'9' to sum, 'c' to config)", 0, 1);

  // Place the gridView
  view.move(0, 3);
  // Set auto size with offset
  //  view.setAutoSize(GridView::AutoSize(0, 3));

  // Create a style for Sensor
  GridStyle s;
  s.setAlign(GridStyle::RIGHT);

  // Set style for sensor Column in the view
  view.setStyle(EctopMonitor::SENSOR_U64, s);
  view.setStyle(EctopMonitor::SENSOR_FLOAT, s);

  // Log
  //view.setLog(FileLog("out.log"),XMLFormat(true));

  // Add Sensor - here
  m.addSensor(
      new InverseCpu(new AcpiPowerMeter(), new CpuElapsedTime()));

//  m.addSensor(new StaticAverage());

  /*
   // FIXME: the number of max sensors varies according to the number
   // of process due to the maximum number of file descriptors allowed (1024)
   // what should we do to fix it? The performance counters cannot be closed
   // otherwise they stop counting ..
   PerfCount* pc;
   //  for (int i = 0; i < PERF_COUNT_HW_MAX; i++)
   for (int i = 0; i < 5; i++)
   {
   m.addSensor(new PerfCount(0, i));
   }
   */
  struct timeval tv, tv_p;

  bool isRunning = true, isFreezed = false;

  // Main Loop
  while (isRunning)
    {
      gettimeofday(&tv_p, NULL);

      // Update Monitor
      if (!isFreezed)
        {
          m.update();
          pe.update();
        }

      /* Event */
      int c = Console::keyHit();
      switch (c)
        {
      case 0:
        break;
      case 'q':
      case 'Q':
        isRunning = false;
        break;
      case 'p':
      case 'P':
        isFreezed = (!isFreezed);
        view.forceRender();
        break;
      case 'f':
        view.fixColumn(1, false, 0);
        break;
      case 'u':
        view.unfixColumn(1);
        break;
      default:
        termCtrl.handleTerminalEvent(c);
        break;
        }

      /* Render */
      if (view.needRender())
        {

          Console::Size sz = Console::getSize();
          std::string freeze;
          if (isFreezed)
            {
              Console::drawText("PAUSED - " + Tools::nowToString(), 0, 0);
            }
          else
            {
              Console::drawText("ectop  - " + Tools::nowToString(), 0, 0);
            }

          view.size(sz.width - view.getLeft(), sz.height - view.getTop());
          view.render();

          gettimeofday(&tv, NULL);
          Console::drawText(
              "debug>> pid: " + Tools::CStr(getpid()) + ", tasks:"
                  + Tools::CStr(view.getRowViewCount()) + ", render time(s):"
                  + Tools::CStr(
                      (tv.tv_sec - tv_p.tv_sec)
                          + (float) ((tv.tv_usec - tv_p.tv_usec)) / 1000000)
                  + "   ", 0, 2, Console::DrawStyle(Color::Yellow));
        }

      Tools::sleep_ms(50);

    };

  return 0;

}
