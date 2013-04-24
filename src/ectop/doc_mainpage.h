///////////////////////////////////////////////////////////////////////////////
/// \file       doc_mainpage.h
/// \author     Leandro Fontoura Cupertino, Dorian Rodde
/// \version    0.1
/// \date       2012.08
/// \copyright  2012, IRIT, CoolEmAll (INFSO-ICT-288701)
/// \brief      File containing main page for Doxygen
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// @mainpage
///
/// @section welcome Welcome
/// Welcome to the official ectop documentation. Here you will find a detailed
/// view of all ectop can do.
///
/// This monitoring application is a top like monitor, where you can easily
/// add/remove sensors (physical measures or estimators).
///
/// @section dep Dependencies
/// - libec
/// - kernel source v2.6 (don't work with 3.0+) [<a href="http://www.kernel.org">download</a>]
/// - ncurses [<a href="http://www.gnu.org/software/ncurses/">download</a>]
///
/// \section lic License
/// Copyright (c) 2011-2014, IRIT, CoolEmAll (INFSO-ICT-288701)
/// by Leandro Fontoura Cupertino <fontoura@irit.fr>
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// \section example1 Adding/removing columns into the interface
/// This example show how to add or remove columns with sensors
/// into the main.cpp file.
///
/// <h3>XML monitor loaded</h3>
/// @code
/// void
/// addSensors(MonitorEctop* m)
/// {
///   m->addSensor(new SensorCpuUsage());
///   m->addSensor(new PerfCount(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES));
///   m->addSensor(new PerfCount(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES));
///   m->addSensor(new LinearRegression(110, 220, new SensorCpuUsage()));
///   m->addSensor(new LinearRegression());
/// }
/// @endcode
///////////////////////////////////////////////////////////////////////////////
