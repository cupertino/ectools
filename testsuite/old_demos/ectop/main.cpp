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

#undef HTML

#include "headers/MonitorEctop.h"
#include "headers/SensorFake.h"

#include <iostream>
#include <string>
#include <sys/sysinfo.h>
#include <libec/logs.h>
#include <libec/process.h>
#include <libec/sensors.h>
#include <libec/estimators.h>

struct timeval tv, tv_p;

using namespace cea;

void
addSensors(MonitorEctop* m)
{
  //monitoring overhead test
  m->addSensor(new FakeSensor(U64));
  m->addSensor(new FakeSensor(U64));
  m->addSensor(new FakeSensor(U64));
  m->addSensor(new FakeSensor(U64));
  m->addSensor(new FakeSensor(Float));
  m->addSensor(new FakeSensor(Float));
  m->addSensor(new FakeSensor(Float));
  m->addSensor(new FakeSensor(Float));
}

void
renderHelpScreen()
{
  Console::Size sz = Console::getSize();
  Console::clear();
  Console::drawText("HELP!!", 0, 0);

  int pos = 2;
  Console::drawText("  '1'..'9'         "
      "Sort (ascending/descending) by visible column position.", 0, pos++);
  Console::drawText("  =+'1'..'9'       "
      "Show accumulated values of visible column position.", 0, pos++);
  Console::drawText("  keyboard arrows  Pan view.", 0, pos++);
  Console::drawText("  d,D              Debug mode.", 0, pos++);
}

void
renderHeader(TermGridView* view, MonitorEctop* m)
{
  Console::Size sz = Console::getSize();
  Console::clear();

  std::string freeze;
  if (m->isFreezed)
    {
      Console::drawText("PAUSED - " + Tools::nowToString(), 0, 0);
    }
  else
    {
      Console::drawText("ectop  - " + Tools::nowToString(), 0, 0);
    }

  view->size(sz.width - view->getLeft(), sz.height - view->getTop());

  if (m->pow != NULL)
    {
      if (m->pow->getStatus())
        {
          Console::drawText(
              "Tasks: " + Tools::CStr(view->getRowViewCount()) + ", Power (W): "
                  + Tools::CStr(m->pow->getValue().Float) + ", CPU (%):"
                  + Tools::CStr(m->cpu->getValue(-1).Float), 0, 1);
        }
    }
  else
    Console::drawText("Tasks: " + Tools::CStr(view->getRowViewCount()), 0, 1);

//  struct sysinfo info;

//  if (sysinfo(&info) == 0)
    {
      if (m->debugMode)
        {
          gettimeofday(&tv, NULL);
          Console::drawText(
              "debug>> pid: " + Tools::CStr(getpid()) + ", render time(s):"
                  + Tools::CStr(
                      (tv.tv_sec - tv_p.tv_sec)
                          + (float) ((tv.tv_usec - tv_p.tv_usec)) / 1000000.0f)
                  + "   ", 0, 2);
        }
    }

  view->render();
}

/* -------------------------------------------------------------------------- */
/* Main */
//const std::string strFilter = "PID > 100";
//unsigned int id = 10;
int
main()
{
  DebugLog::create();
  DebugLog::clear();

  std::string path = "/var/www/html/ectop/";

  // Model
  MonitorEctop m;
  ProcessEnumerator pe;
  // Connect Process enumerator to monitor
  m.connectRowFeeder(&pe);

// View
  TermGridView view(m); // terminal

  // Style filter
//  const GridStyle style;
//  style.color = 255;
//  view.setStyle(id, style);
//  view.addStyleFilter(strFilter, id);

// Controller
  TermGridCtrl termCtrl(view);

  // Application header
  renderHeader(&view, &m);

  // Place the gridView
  view.move(0, 3);
  //view.setAutoSize(GridView::AutoSize(0, 3)); // Set auto size with offset

  // Create a style for Sensor
  GridStyle s;
  s.setAlign(GridStyle::RIGHT);

  // Set style for sensor Column in the view
  view.setStyle(MonitorEctop::SENSOR_U64, s);
  view.setStyle(MonitorEctop::SENSOR_FLOAT, s);

  // Log
  //view.setLog(FileLog("out.log"),XMLFormat(true));

  // Add Sensors
  addSensors(&m);

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
        m.debugMode = true;
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
        break;
        }

      /* Render */
      if (view.needRender())
        renderHeader(&view, &m);

      Tools::sleep_ms(100);
    };

  return 0;
}
