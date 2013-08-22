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
      float freq, std::string logfilename, std::string outfilename)
  {
    _benchCmd = bench;
    setFrequency(freq);
    setIdleTime(idleTime);

    _logFile.clear();
    _logFile.set(FileLog(logfilename, true), GnuplotFormat());
    _logFile.setFlushFrequency(5000);

    _outFile.clear();
    _outFile.set(FileLog(outfilename, true), CSVFormat());
    _outFile.setFlushFrequency(5000);
  }

  DataAcquisition::~DataAcquisition()
  {
    for (std::list<cea::Sensor*>::iterator it = _sensors.begin();
        it != _sensors.end(); it++)
      delete (*it);
  }

  void
  DataAcquisition::collectData()
  {
    pthread_t thread;
    time_t start_time;
    struct timeval tv;
    Sensor* s;

    if (_sensors.empty())
      getAvailableSensors(_sensors);

    logHeader(_sensors);

    // The first time a sensor is updated its value don't have meaning
    for (std::list<Sensor*>::iterator it = _sensors.begin();
        it != _sensors.end(); it++)
      {
        s = *it;
        s->update();
      }
//    sleep(1);

    start_time = time(NULL);

    if (_benchCmd != NULL)
      {
        // Runs the benchmark in a new thread
        if (pthread_create(&thread, NULL, runBenchmark, this) == -1)
          {
            std::string msg =
                "error: The benchmark thread could not be created.";
            std::cerr << msg << std::endl;
            _logFile.openBlock("");
            _logFile.write(msg);
            _logFile.closeBlock("");
            _logFile.update();
            _logFile.flush();
            exit(1);
          }
      }
    _isBenchRunning = true;

    // Keep collecting the data while benchmark is running
    // wait a while to start the experiment on the precise second
    gettimeofday(&tv, NULL);
    usleep(1000000 - tv.tv_usec);

    while (_isBenchRunning)
      {
        _outFile.openBlock("");
        _outFile.write(time(NULL));

        for (std::list<Sensor*>::iterator it = _sensors.begin();
            it != _sensors.end(); it++)
          {
            s = *it;
            s->update();

            if (s->getType() == U64)
              _outFile.write(s->getValue().U64);
            else
              _outFile.write(s->getValue().Float);
          }
        _outFile.closeBlock("");
        _outFile.update();

        gettimeofday(&tv, NULL);
        usleep(_periodTime * 1000000 - tv.tv_usec);
      }

    // Log benchmark data
    _logFile.addComment("");
    _logFile.addComment("Benchmark summary");
    _ss.str("");
    _ss << "  Command line:      \t" << _benchCmd;
    _logFile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Start time:        \t" << _benchStart;
    _logFile.addComment(_ss.str());
    _ss.str("");
    _ss << "  End time:          \t" << _benchEnd;
    _logFile.addComment(_ss.str());
    _logFile.update();

    _outFile.flush();
    _logFile.flush();
  }

  void
  DataAcquisition::loadAvailableSensors()
  {
    getAvailableSensors(_sensors);
  }

  void
  DataAcquisition::getAvailableSensors(std::list<cea::Sensor*> &sensors)
  {
    cea::Sensor* newSensor;
    int nPC = 0;
    int nNet = 0;
    int nPow = 0;
    int nPS = 0;
    int nCPU = 0;

    _logFile.addComment("System config");
    struct utsname buf;
    if (uname(&buf) == 0)
      {
        _ss.str("");
        _ss << "  Node name:             \t" << buf.nodename;
        _logFile.addComment(_ss.str());
        _ss.str("");
        _ss << "  Domain name:           \t" << buf.domainname;
        _logFile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS name:               \t" << buf.sysname;
        _logFile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS release:            \t" << buf.release;
        _logFile.addComment(_ss.str());
        _ss.str("");
        _ss << "  OS version:            \t" << buf.version;
        _logFile.addComment(_ss.str());
        _ss.str("");
        _ss << "  Machine:               \t" << buf.machine;
        _logFile.addComment(_ss.str());
      }

    int numCPU = SystemInfo::getCpuCount();

    if (numCPU != -1)
      {
        _ss.str("");
        _ss << "  Number of CPUs:        \t" << numCPU;
        _logFile.addComment(_ss.str());
      }

    long int numFILES = sysconf(_SC_OPEN_MAX);
    if (numFILES != -1)
      {
        _ss.str("");
        _ss << "  Max files per process: \t" << numFILES;
        _logFile.addComment(_ss.str());
      }

    _logFile.addComment("");
    _logFile.addComment("Sensor's details");

    _logFile.update();

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

    // Network sensors
    for (uint net_id = 0; net_id < Network::TYPE_MAX; net_id++)
      {
        newSensor = new Network((Network::TypeId) net_id);
        nNet += addSensor(&sensors, newSensor);
      }

    newSensor = new CpuTimeUsage;
    nCPU += addSensor(&sensors, newSensor);

    int ncpus = SystemInfo::getCpuCount();
    for (int c = 0; c < ncpus; c++)
      {
//        newSensor = new CpuTimeUsage(i);
//        nCPU += addSensor(&sensors, newSensor);

        newSensor = new CpuFreq(c);
        nCPU += addSensor(&sensors, newSensor);

        newSensor = new CpuFreqMsr(c);
        nCPU += addSensor(&sensors, newSensor);

        newSensor = new CpuTemp(c);
        nCPU += addSensor(&sensors, newSensor);

        newSensor = new CpuStateMsr(c);
        nCPU += addSensor(&sensors, newSensor);

        int nstates = CpuStateTime::getNumberofStates();
        for (int s = 0; s < nstates; s++)
          {
            newSensor = new CpuStateTimeElapsed(s, c);
            nCPU += addSensor(&sensors, newSensor);
          }
      }

    newSensor = new MemRss;
    nCPU += addSensor(&sensors, newSensor);

    newSensor = new MemUsage;
    nCPU += addSensor(&sensors, newSensor);

    // Power sensors
    newSensor = new G5kPowerMeter();
    nPow += addSensor(&sensors, newSensor);

    newSensor = new AcpiPowerMeter();
    nPow += addSensor(&sensors, newSensor);

//    newSensor = new RecsPowerMeter("192.168.0.250", 10001, 13);
//    nPow += addSensor(&sensors, newSensor);

    _logFile.addComment("");
    _logFile.addComment("Number of Available Sensors");
    _ss.str("");
    _ss << "  Performance Counter: \t" << nPC;
    _logFile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Networking:          \t" << nNet;
    _logFile.addComment(_ss.str());
    _ss.str("");
    _ss << "  PidStat:             \t" << nPS;
    _logFile.addComment(_ss.str());
    _ss.str("");
    _ss << "  Power                \t" << nPow;
    _logFile.addComment(_ss.str());
    _nofSensors = nPC + nNet + nPS + nPow;
    _ss.str("");
    _ss << "  Total                \t" << _nofSensors;
    _logFile.addComment(_ss.str());
    _logFile.update();
    _logFile.addComment("");

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

    ss << "  " << sensor->getName() << " [" << sensor->getAlias() << "]";
    if (sensor->getStatus())
      {
        ss << "\t[ok]";
        _logFile.addComment(ss.str());

        list->push_back(sensor);
        return 1;
      }

    ss << "\t[fail]";
    _logFile.addComment(ss.str());

    return 0;
  }

  void
  DataAcquisition::logHeader(std::list<Sensor*> sensor)
  {
    _outFile.openBlock("");
    _outFile.write("TS");

    for (std::list<Sensor*>::iterator it = sensor.begin(); it != sensor.end();
        it++)
      {
        Sensor* s = *it;
        _outFile.write(s->getAlias());
      }
    _outFile.closeBlock("");
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
        da->_logFile.addComment(da->_ss.str());

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
