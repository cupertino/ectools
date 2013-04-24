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

#include "../headers/MonitorEctop.h"
#include <libec/tools.h>

namespace cea
{
  // Constructor
  MonitorEctop::MonitorEctop() :
      debugMode(false), isFreezed(false), pow(NULL), cpu(NULL)
  {
    addColumn("PID", Value::INT, PID);
    getColumn(PID).setFixed(0, false);
    addColumn("COMMAND", Value::STRING, COMMAND, 20, 30);
  }

  MonitorEctop::~MonitorEctop()
  {
    for (std::list<PIDSensor*>::iterator it = _sensors.begin();
       it != _sensors.end(); it++)
      delete *it;

    if (pow != NULL)
      delete pow;

    if (cpu != NULL)
      delete cpu;
  }

  // Add a PID Sensor to a column
  void
  MonitorEctop::addSensor(PIDSensor* s)
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
        _sensors.push_back(s);
      }
    else
      delete s;
  }

  void
  MonitorEctop::addPowerMeter(PowerMeter* p)
  {
    if (p->getStatus())
      {
        this->pow = p;
      }
    else
      delete p;
  }

  void
  MonitorEctop::addCpuSensor(PidStat* s)
  {
    if (s->getStatus())
      {
        this->cpu = s;
      }
    else
      delete s;
  }

  // Event launched after a row beeing created
  void
  MonitorEctop::onRowCreate(Row& r)
  {
    //    if (r.tag == PROCESS)
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        Process& p = cast<Process>(r);

        setValue(r, COMMAND, p.getName());
        setValue(r, PID, p.getPid());

        //add the pid into all PIDSensors
        for (ColumnMap::iterator it = mappedColumns.begin();
            it != mappedColumns.end(); it++)
          {
            PIDSensor* s = (PIDSensor*) (*it).first;
            s->add(p.getPid());
          }
      }
  }

  // Event launched when a row need to be updated
  void
  MonitorEctop::onRowUpdate(Row& r)
  {
  }

  void
  MonitorEctop::onUpdate()
  {
    for (RowList::iterator r = rows.begin(); r != rows.end(); ++r)
      {
        if (_filter.applyFilter(*(*r)))
          {
            if ((*(*r)).tag == FEEDER_PROCESS_ITEM)
              {
                Process& p = cast<Process>(*(*r));

                for (ColumnList::iterator c = columns.begin();
                    c != columns.end(); ++c)
                  {
                    if ((*(*c)).tag == SENSOR_U64)
                      {
                        PIDSensor& s = cast<PIDSensor>(*(*c));
                        unsigned long long val = s.getValuePid(p.getPid()).U64;
                        setValue(*(*r), *(*c), val);
                      }
                    else if ((*(*c)).tag == SENSOR_FLOAT)
                      {
                        PIDSensor& s = cast<PIDSensor>(*(*c));
                        float val = s.getValuePid(p.getPid()).Float;
                        setValue(*(*r), *(*c), val);
                      }
                  }
              }
          }
      }
  }

  void
  MonitorEctop::beforeRenderViews()
  {
  }

  void
  MonitorEctop::afterRenderViews()
  {
  }

  void
  MonitorEctop::onRowDelete(Row& r)
  {
    //    if (r.tag == PROCESS)
    if (r.tag == FEEDER_PROCESS_ITEM)
      {
        //remove the pid from all PIDSensors
        for (ColumnMap::iterator it = mappedColumns.begin();
            it != mappedColumns.end(); it++)
          {
            PIDSensor* s = (PIDSensor*) (*it).first;
            s->remove(cast<Process>(r)->getPid());
          }
      }
  }

  TermGridView*
  getView()
  {
//    return views[0];
    return NULL;
  }

  void
  MonitorEctop::saveToJsonFile(std::string path)
  {
    std::ofstream ofs;

    path.append("/data.json");
    ofs.open(path.c_str());

    ofs << "[ ";

    ColumnList::iterator c = columns.begin();
    while (c != columns.end())
      {
        if (((*(*c)).tag == MonitorEctop::SENSOR_U64)
            || ((*(*c)).tag == MonitorEctop::SENSOR_FLOAT))
          {
            PIDSensor& s = cast<PIDSensor>(*(*c));
            ofs << "{ \"key\": \"" << s.getAlias() << "\", \"values\": [ ";
            //escreve sensor
            MonitorEctop::RowList::iterator r = rows.begin();
            while (r != rows.end())
              {
                Process& p = cast<Process>(*(*r));
                //escreve processo
                ofs << "{ \"label\" : \"" << p.getName() << "\" , \"value\" : "
                    << getValue(*(*r), *(*c)) << " }";
                r++;
                if (r != rows.end())
                  ofs << ", ";
              }
            ofs << " ] }";

            c++;
            if (c != columns.end())
              {
                ofs << ", ";
              }

          }
        else
          c++;
      }
    ofs << " ]";
    ofs.close();
  }
} /* namespace cea */
