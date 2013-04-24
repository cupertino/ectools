//============================================================================
// Name        : DataAcquisition.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.27
// Copyright   : Your copyright notice
// Description : Performance Counter Sensor header
//============================================================================

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#include <libec/device/SystemInfo.h>
#include <libec/DataAcquisition.h>

namespace cea
{
  DataAcquisition::DataAcquisition(char* bench, unsigned int idleTime,
      float freq, std::string logfile)
  {
    _benchCmd = bench;
    setFrequency(freq);
    setIdleTime(idleTime);

    _logfile.set(FileLog(logfile, true), GnuplotFormat());
    _logfile.setFlushFrequency(5000);
  }

  DataAcquisition::~DataAcquisition()
  {
    for (std::list<cea::Sensor*>::iterator it = _sensors.begin();
        it != _sensors.end(); it++)
      delete (*it);

//    for (std::list<cea::Sensor*>::iterator it = _sensors.begin();
//        it != _sensors.end(); it++)
//      {
//        cea::Sensor* s = *it;
//        delete s;
//      }
  }

  void
  DataAcquisition::collectData()
  {
    pthread_t thread;
    time_t start_time;
    struct timeval tv;
    Sensor* s;

    if (_sensors.empty())
//      _sensors = getAvailableSensors();
      getAvailableSensors(_sensors);

    _logfile.addComment("");
    _logfile.addComment("Benchmark Data");

    logHeader(_sensors);

    // The first time a sensor is updated its value don't have meaning
    for (std::list<Sensor*>::iterator it = _sensors.begin();
        it != _sensors.end(); it++)
      {
        s = *it;
        s->update();
      }
    sleep(1);

    start_time = time(NULL);

    // Runs the benchmark in a new thread
    if (pthread_create(&thread, NULL, runBenchmark, this) == -1)
      {
        std::string msg = "error: The benchmark thread could not be created.";
        std::cerr << msg << std::endl;
        _logfile.openBlock("");
        _logfile.write(msg);
        _logfile.closeBlock("");
        _logfile.update();
        _logfile.flush();
        exit(1);
      }
    _isBenchRunning = true;

    // Keep collecting the data while benchmark is running
    // wait a while to start the experiment on the precise second
    gettimeofday(&tv, NULL);
    usleep(1000000 - tv.tv_usec);

    while (_isBenchRunning)
      {
        _logfile.openBlock("");
//        std::stringstream ss;
//        ss << time(NULL) << ".0";
//        _logfile.write(ss.str());
        _logfile.write(time(NULL));

        for (std::list<Sensor*>::iterator it = _sensors.begin();
            it != _sensors.end(); it++)
          {
            s = *it;
            if (s->getType() == U64)
              {
//                std::stringstream ss;
//                ss << s->getValue().U64 << ".0";
//                _logfile.write(ss.str());
                _logfile.write(s->getValue().U64);
              }
            else
              _logfile.write(s->getValue().Float);
          }
        _logfile.closeBlock("");
        _logfile.update();

        gettimeofday(&tv, NULL);
        usleep(_periodTime * 1000000 - tv.tv_usec);
      }

    // Log benchmark data
    _logfile.addComment("");
    _logfile.addComment("Benchmark summary");
    _ss.str("");
    _ss << "  Command line:      \t" << _benchCmd;
    _logfile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Start time:        \t" << _benchStart;
    _logfile.addComment(_ss.str());
    _ss.str("");
    _ss << "  End time:          \t" << _benchEnd;
    _logfile.addComment(_ss.str());
    _logfile.update();

    _logfile.flush();
  }

  void
  DataAcquisition::getAvailableSensors(std::list<cea::Sensor*> &sensors)
  {
//    std::list<cea::Sensor*> sensors;
    cea::Sensor* newSensor;
    int nPC = 0;
    int nNet = 0;
    int nPow = 0;
    int nPS = 0;

    _logfile.addComment("System config");
    struct utsname buf;
    if (uname(&buf) == 0)
      {
        _ss.str("");
        _ss << "  Node name:             \t" << buf.nodename;
        _logfile.addComment(_ss.str());
        _ss.str("");
        _ss << "  Domain name:           \t" << buf.domainname;
        _logfile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS name:               \t" << buf.sysname;
        _logfile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS release:            \t" << buf.release;
        _logfile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS version:            \t" << buf.version;
        _logfile.addComment(_ss.str());
        _ss.str("");
        _ss << "  Machine:               \t" << buf.machine;
        _logfile.addComment(_ss.str());
      }

    int numCPU = SystemInfo::getCpuCount();

    if (numCPU != -1)
      {
        _ss.str("");
        _ss << "  Number of CPUs:        \t" << numCPU;
        _logfile.addComment(_ss.str());
      }

    long int numFILES = sysconf(_SC_OPEN_MAX);
    if (numFILES != -1)
      {
        _ss.str("");
        _ss << "  Max files per process: \t" << numFILES;
        _logfile.addComment(_ss.str());
      }

    _logfile.addComment("");
    _logfile.addComment("Sensor's details");

    _logfile.update();

    // Performance Counter sensors per node
    for (int perf_hw_id = 0; perf_hw_id < PERF_COUNT_HW_MAX; perf_hw_id++)
      {
        newSensor = new PerfCount(PERF_TYPE_HARDWARE, perf_hw_id);
        nPC += addSensor(&sensors, newSensor);
      }
    for (int perf_sw_id = 0; perf_sw_id < PERF_COUNT_SW_MAX; perf_sw_id++)
      {
        newSensor = new PerfCount(PERF_TYPE_SOFTWARE, perf_sw_id);
        nPC += addSensor(&sensors, newSensor);
      }

    for (int perf_hw_cache_id = 0; perf_hw_cache_id < PERF_COUNT_HW_CACHE_MAX;
        perf_hw_cache_id++)
      {
        for (int perf_hw_cache_op_id = 0;
            perf_hw_cache_op_id < PERF_COUNT_HW_CACHE_OP_MAX;
            perf_hw_cache_op_id++)
          {
            for (int perf_hw_cache_op_result_id = 0;
                perf_hw_cache_op_result_id < PERF_COUNT_HW_CACHE_RESULT_MAX;
                perf_hw_cache_op_result_id++)
              {
                u64 config = (perf_hw_cache_id) | (perf_hw_cache_op_id << 8)
                    | (perf_hw_cache_op_result_id << 16);

                newSensor = new PerfCount(PERF_TYPE_HW_CACHE, config);
                nPC += addSensor(&sensors, newSensor);
              }
          }
      }

    /*
     // Performance Counter sensors per cpu
     for (int perf_hw_id = 0; perf_hw_id < PERF_COUNT_HW_MAX; perf_hw_id++)
     {
     for (int cpu = 0; cpu < numCPU; cpu++)
     {
     newSensor = new PerfCount(PERF_TYPE_HARDWARE, perf_hw_id, -1, cpu);
     if (addSensor(&sensors, newSensor) == 0)
     nPC++;
     }
     }
     for (int perf_sw_id = 0; perf_sw_id < PERF_COUNT_SW_MAX; perf_sw_id++)
     {
     for (int cpu = 0; cpu < numCPU; cpu++)
     {
     newSensor = new PerfCount(PERF_TYPE_SOFTWARE, perf_sw_id, -1, cpu);
     if (addSensor(&sensors, newSensor) == 0)
     nPC++;
     }
     }

     for (int perf_hw_cache_id = 0; perf_hw_cache_id < PERF_COUNT_HW_CACHE_MAX;
     perf_hw_cache_id++)
     {
     for (int perf_hw_cache_op_id = 0;
     perf_hw_cache_op_id < PERF_COUNT_HW_CACHE_OP_MAX;
     perf_hw_cache_op_id++)
     {
     for (int perf_hw_cache_op_result_id = 0;
     perf_hw_cache_op_result_id < PERF_COUNT_HW_CACHE_RESULT_MAX;
     perf_hw_cache_op_result_id++)
     {
     u64 config = (perf_hw_cache_id) | (perf_hw_cache_op_id << 8)
     | (perf_hw_cache_op_result_id << 16);

     for (int cpu = 0; cpu < numCPU; cpu++)
     {
     newSensor = new PerfCount(PERF_TYPE_HW_CACHE, config, -1,
     cpu);
     if (addSensor(&sensors, newSensor) == 0)
     nPC++;
     }
     }
     }
     }*/

    // Network sensors
    for (uint net_id = 0; net_id < Network::TYPE_MAX; net_id++)
      {
        newSensor = new Network((Network::TypeId) net_id);
        nNet += addSensor(&sensors, newSensor);
      }

    // Pidstat sensors
    for (uint ps_type = 0; ps_type < PidStat::TYPE_MAX; ps_type++)
      {
        newSensor = new PidStat((PidStat::TypeId) ps_type);
        nPS += addSensor(&sensors, newSensor);
      }

    /*
     for (uint ps_mem_id = 0; ps_mem_id < PS_MEM_MAX; ps_mem_id++)
     {
     newSensor = new PidStat(-1, PS_TYPE_MEMORY, ps_mem_id);
     if (addSensor(&sensors, newSensor) == 0)
     nPS++;
     }
     for (int ps_io_id = 0; ps_io_id < PS_IO_MAX; ps_io_id++)
     {
     newSensor = new PidStat(-1, PS_TYPE_IO, ps_io_id);
     if (addSensor(&sensors, newSensor) == 0)
     nPS++;
     }
     for (int ps_task_switch_id = 0; ps_task_switch_id < PS_TASK_SWITCH_MAX;
     ps_task_switch_id++)
     {
     newSensor = new PidStat(-1, PS_TYPE_TASK_SWITCH, ps_task_switch_id);
     if (addSensor(&sensors, newSensor) == 0)
     nPS++;
     }
     */

    // Power sensors
    newSensor = new G5kPowerMeter();
    nPow += addSensor(&sensors, newSensor);

    newSensor = new AcpiPowerMeter();
    nPow += addSensor(&sensors, newSensor);

    _logfile.addComment("");
    _logfile.addComment("Number of Available Sensors");
    _ss.str("");
    _ss << "  Performance Counter: \t" << nPC;
    _logfile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Networking:          \t" << nNet;
    _logfile.addComment(_ss.str());
    _ss.str("");
    _ss << "  PidStat:             \t" << nPS;
    _logfile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Power                \t" << nPow;
    _logfile.addComment(_ss.str());
    _nofSensors = nPC + nNet + nPS + nPow;
    _ss.str("");
    _ss << "  Total                \t" << _nofSensors;
    _logfile.addComment(_ss.str());
    _logfile.update();

//    return sensors;
  }

  void
  DataAcquisition::setBenchmark(char* bench)
  {
    _benchCmd = bench;
  }

  // Set the frequency in Hz
  void
  DataAcquisition::setFrequency(float freq)
  {
    _periodTime = 1 / freq;
  }

  void
  DataAcquisition::setIdleTime(unsigned int time)
  {
    _idleTime = time;
  }

  bool
  DataAcquisition::addSensor(Sensor *sensor)
  {
    return addSensor(&_sensors, sensor);
  }

  bool
  DataAcquisition::addSensor(std::list<Sensor*> *list, Sensor *sensor)
  {
    std::stringstream ss;

    if (sensor->getStatus())
      {
        ss << "  " << sensor->getName() << "\t[ok]";
        _logfile.addComment(ss.str());

        list->push_back(sensor);
        return 1;
      }

    ss << "  " << sensor->getName() << "\t[fail]";
    _logfile.addComment(ss.str());

    return 0;
  }

  void
  DataAcquisition::logHeader(std::list<Sensor*> sensor)
  {
    std::stringstream alias_ss;
    _ss.str("");

    _ss << "name time";
    alias_ss << "alias time";
    for (std::list<Sensor*>::iterator it = sensor.begin(); it != sensor.end();
        it++)
      {
        Sensor* sen = *it;
        _ss << "\t" + sen->getName();
        alias_ss << "\t" + sen->getAlias();
      }
    _logfile.addComment(_ss.str());
    _logfile.addComment(alias_ss.str());
  }

  void
  DataAcquisition::printHeader(std::list<Sensor*> sensor)
  {
    // Print sensors headers
    std::cerr << "#name time";
    for (std::list<Sensor*>::iterator it = sensor.begin(); it != sensor.end();
        it++)
      {
        Sensor* sen = *it;
        std::cerr << "\t" + sen->getName();
      }
    std::cerr << std::endl;

    std::cerr << "#alias time";
    for (std::list<Sensor*>::iterator it = sensor.begin(); it != sensor.end();
        it++)
      {
        Sensor* sen = *it;
        std::cerr << "\t" + sen->getAlias();
      }
    std::cerr << std::endl;
  }

  void
  DataAcquisition::printSensors(std::list<Sensor*> sensors)
  {
    std::cerr << time(NULL);

    for (std::list<Sensor*>::iterator it = sensors.begin(); it != sensors.end();
        it++)
      {
        Sensor* s = *it;

        if (s->getType() == U64)
          std::cerr << "\t" << ((sensor_t) s->getValue()).U64;
        else if (s->getType() == Float)
          {
//            std::cerr.precision(2);
            std::cerr << "\t" << ((sensor_t) s->getValue()).Float;
//            printf("\t%3.2f", ((sensor_t) s->getValue()).Float);
          }
      }
  }

  void
  DataAcquisition::updateSensors(std::list<Sensor*> sensors)
  {
    for (std::list<Sensor*>::iterator it = sensors.begin(); it != sensors.end();
        it++)
      {
        Sensor* s = *it;
        s->update();
      }
  }

  void*
  DataAcquisition::runBenchmark(void* data)
  {
    DataAcquisition* da = (DataAcquisition*) data;
    int error;

    sleep(da->_idleTime);
    time_t start = time(NULL);
//    std::cout << start << "\tBenchmark ... [start]" << std::endl;
    error = system(da->_benchCmd);
//    error = execv(da->_benchCmd, NULL);
    if (error == -1)
      {
        da->_ss.str("");
        da->_ss << "# " << time(NULL) << "\tBenchmark ... [error] cmd ["
            << da->_benchCmd << "]" << std::endl;
        da->_logfile.addComment(da->_ss.str());

        std::cerr << da->_ss;

        return (void*) -1;
      }
    time_t stop = time(NULL);
//    std::cout << stop << "\tBenchmark ... [stop: " << error << "]" << std::endl;
    sleep(da->_idleTime);
    da->_isBenchRunning = false;
    da->_benchStart = start;
    da->_benchEnd = stop;

    return (void*) 0;
  }

} /* namespace cea */
