///////////////////////////////////////////////////////////////////////////////
/// @file       doc_mainpage.h
/// @author     Dorian RODDE, Leandro Fontoura Cupertino
/// @version    0.1
/// @date       2012.07
/// @copyright  2012, CoolEmAll (INFSO-ICT-288701)
/// @brief      File containing main page for Doxygen 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @mainpage
///
/// @section welcome Welcome
/// This is to the official libec (Energy Consumption library) documentation.
/// Here you will find a detailed view of libec features.
///
/// @section desc Description
/// The libec library was developed to help the development of new energy
/// consumption sensors, implementing an easy way to create and monitor them.
/// It contains a set of already implemented sensors. The sensors are defined
/// as a class which returns a direct measure or an estimated value. A direct
/// measure is a physical measurement like temperature and power, while an
/// estimator aggregates a value to the measured data in order to estimate a
/// new property.
///
/// Currently, the sensors implemented in this library are mainly based on
/// Hardware Performance Counters and operational system information (from
/// /proc filesystem), but libec sensors can also collect different data
/// coming from any source. The sensors can be divided into two classes: node
/// and process related sensors.
///
/// Node related sensors cannot be defined for each process running on the
/// system, e.g. PUD power can only provide information related to the power
/// measured on the outlet, which concerns the entire node. This kind of
/// sensors can be implemented by inheritance of the abstract class
/// cea::Sensor.
///
/// On the other hand, process related sensors can retrieve information for a
/// specific process, e.g. Performance Counters can count the number of cache
/// misses occurred during the execution of a process. This sensor can be
/// implemented by inheritance of the abstract class cea::PIDSensor.
///
/// This library is mainly developed to be a set of generic class for
/// collecting energy consumption data. With this library you can also monitor,
/// log, show in many views, load any data.
///
/// \section dep Dependencies
/// - kernel source v2.6 (don't work with 3.0+) [<a href="http://www.kernel.org">download</a>]
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
/// @section demos Demos
/// Some demos can be found in the demos folder that comes with the
/// distributed code. Most of these demos are in fact testing projects of
/// libec. The cea::Monitor and cea::PIDSensor detailed description also
/// contains some examples.
///
///////////////////////////////////////////////////////////////////////////////
