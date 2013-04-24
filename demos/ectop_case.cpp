/*
 ectop - Terminal process monitoring tool.
 Copyright (C) 2012 by Leandro Fontoura Cupertino, IRIT
 Author: Leandro Fontoura Cupertino <fontoura@irit.fr>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//#define HTML
#undef HTML

#include "../src/ectop/headers/MonitorEctop.h"

#include <iostream>
#include <string>
#include <sys/sysinfo.h>
#include <libec/device/SystemInfo.h>
#include <libec/logs.h>
#include <libec/process.h>
#include <libec/sensors.h>
#include <libec/estimators.h>

struct timeval tv, tv_p;

using namespace cea;

void
addSensors(MonitorEctop &m)
{
  //monitoring overhead test
//  m.addSensor(new FakeSensor(U64));
//  m.addSensor(new FakeSensor(Float));

//real case monitoring
  m.addPowerMeter(new AcpiPowerMeter());
  m.addCpuSensor(new PidStat(PidStat::CPU_USAGE));

  m.addSensor(new CpuTime());
  m.addSensor(new CpuElapsedTime());

  m.addSensor(new PerfCount());
  m.addSensor(new PidStat(PidStat::CPU_USAGE));
  m.addSensor(new PidStat(PidStat::RESIDENT_SET_SIZE));
  m.addSensor(new MinMaxCpu(22, 55));
//  m.addSensor(new MinMaxCpu2(new CpuElapsedTime(), 22, 55));
//  m.addSensor(new InverseCpu(new AcpiPowerMeter(), new CpuElapsedTime()));
}

void
renderHelpScreen()
{
  Console::Size sz = Console::getSize();
  Console::clear();
  Console::drawText("ectop - HELP!!", 0, 0);

  int pos = 2;
  Console::drawText("  '1'..'9'         "
      "Sort (ascending/descending) by visible column position.", 0, pos++);
  Console::drawText("  =+'1'..'9'       "
      "Show accumulated values of visible column position.", 0, pos++);
  Console::drawText("  keyboard arrows  Pan view.", 0, pos++);
  Console::drawText("  d,D              Debug mode.", 0, pos++);
}

void
renderHeader(TermGridView &view, MonitorEctop &m)
{
  Console::Size sz = Console::getSize();
  Console::clear();
  std::stringstream ss;

  view.size(sz.width - view.getLeft(), sz.height - view.getTop());

  if (m.isFreezed)
    ss << "PAUSED - ";
  else
    ss << "ectop  - ";

  ss << Tools::nowToString();
  Console::drawText(ss.str(), 0, 0);

  ss.str("");
  ss << "Tasks: ";
  ss << view.getRowViewCount();
  if (m.pow != NULL)
    {
      ss << ", Power (W): ";
      ss << m.pow->getValue().Float;
    }
  if (m.cpu != NULL)
    {
      ss << ", CPU (%):";
      ss << m.cpu->getValuePid(-1).Float;
    }
  Console::drawText(ss.str(), 0, 1);

//  struct sysinfo info;

//  if (sysinfo(&info) == 0)
    {
      if (m.debugMode)
        {
          ss.str("");
          gettimeofday(&tv, NULL);
          ss << "debug>> pid: ";
          ss << getpid();
          ss << ", render time(s):";
          ss
              << (tv.tv_sec - tv_p.tv_sec)
                  + (float) ((tv.tv_usec - tv_p.tv_usec)) / 1000000.0f;
          ss << "   ";
          Console::drawText(ss.str(), 0, 2);
        }
    }

  view.render();
}

/* -------------------------------------------------------------------------- */
/* Main */
//const std::string strFilter = "PID > 100";
//unsigned int id = 10;
int
main()
{
  DebugLog::create("ectop.log");
  DebugLog::clear();

  SystemInfo::init();

  std::string path = "/var/www/html/ectop/";

  // Model
  MonitorEctop m;
  ProcessEnumerator pe;
  // Connect Process enumerator to monitor
  m.connectRowFeeder(&pe);

// View
  TermGridView view(m); // terminal
#ifdef HTML
      HTMLGridView table_view(m);
#endif

  // Style filter
//  const GridStyle style;
//  style.color = 255;
//  view.setStyle(id, style);
//  view.addStyleFilter(strFilter, id);

// Controller
  TermGridCtrl termCtrl(view);
#ifdef HTML
  TermGridCtrl htmlCtrl(table_view);
#endif

  // Application header
  renderHeader(view, m);

  // Place the gridView
  view.move(0, 3);
  //view.setAutoSize(GridView::AutoSize(0, 3)); // Set auto size with offset
#ifdef HTML
  table_view.move(0, 3);
#endif

  // Create a style for Sensor
  GridStyle s;
  s.setAlign(GridStyle::RIGHT);

  // Set style for sensor Column in the view
  view.setStyle(MonitorEctop::SENSOR_U64, s);
  view.setStyle(MonitorEctop::SENSOR_FLOAT, s);

  // Log
  //view.setLog(FileLog("out.log"),XMLFormat(true));

  // Add Sensors
  addSensors(m);

  int cols = view.getColumnViewCount();
  if (cols > 2)
    {
      for (int c = 2; c < cols; c++)
        view.setColumnSum(c, true);

      view.sort(2, GridView::DESCENDING);
    }

  // Main Loop
  bool isRunning = true, displayHelp = false;

  while (isRunning)
    {
      gettimeofday(&tv_p, NULL);

      // Update Monitor
      if ((!m.isFreezed) && (!displayHelp))
        {
          m.update();
          pe.update();
        }

      // Event
      int c = Console::keyHit();
      switch (c)
        {
      case 0:
        break;
      case 'd':
      case 'D':
        m.debugMode = !m.debugMode;
        break;
      case 'h':
      case 'H':
        displayHelp = !displayHelp;
        if (displayHelp)
          renderHelpScreen();
        break;
      case 'q':
      case 'Q':
        if (displayHelp)
          {
            displayHelp = m.isFreezed = false;
          }
        else
          isRunning = false;
        break;
      case 'p':
      case 'P':
        m.isFreezed = (!m.isFreezed);
        view.forceRender();
        break;
      case 'f':
      case 'F':
        view.fixColumn(1, false, 0);
//        m.setFilter(filter_str);
        break;
      case 'u':
      case 'U':
        view.unfixColumn(1);
        break;
      default:
        termCtrl.handleTerminalEvent(c);
#ifdef HTML
        htmlCtrl.handleTerminalEvent(c);
#endif
        break;
        }

      /* Render */
      if (view.needRender())
        renderHeader(view, m);

#ifdef HTML
      if (table_view.needRender())
        {
          table_view.render();
          table_view.saveToFile(path + "/data.html");
        }
#endif

      Tools::sleep_ms(100);
    };

  return 0;
}
