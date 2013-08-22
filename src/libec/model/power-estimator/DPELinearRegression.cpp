/*
 * DPELinearRegression.cpp
 *
 *  Created on: 2 avr. 2013
 *      Author: fontoura
 */

#include <iostream>
#include <string>

#include <libec/estimator/DPELinearRegression.h>
#include <libec/tools/Tools.h>

#include <libec/tools/DebugLog.h>

namespace cea
{
  DPELinearRegression::DPELinearRegression(int params, double *weights,
      PowerMeter *pm) :
      _lr(params, 1, 100)
  {
    _weights = NULL;
    clean();

    _pm = pm;
    _weights = new double[params + 1];
    _params = params;

    if (params < 1)
      DebugLog::writeMsg(DebugLog::ERROR, "DPELinearRegression::constructor()",
          "Invalid number of parameters");

    if (weights != NULL)
      {
        for (int i = 0; i < params + 1; i++)
          _weights[i] = weights[i];
      }
  }

  DPELinearRegression::~DPELinearRegression()
  {
    clean();
  }

  void
  DPELinearRegression::calibrate()
  {
#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::calibrate()",
        "-- in");
#endif

    if (_lr.countSamples() > 5)
      _lr.solve(_weights);
    else
      DebugLog::writeMsg(DebugLog::WARNING, "DPELinearRegression::calibrate()",
          "The number of samples (%d) is too low. The weights will not be calibrated.",
          _lr.countSamples());

#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::calibrate()",
        "-- out");
#endif
  }

  void
  DPELinearRegression::collectData(int secs)
  {
    timeval tstart; // Start time
    timeval istart; // Iteration's start time
    timeval tnow; // Current times
    int elapsed = 0;
    int ielapsed = 0; // Elapsed time during one iteration
    int total = secs * 1000;

    if (_pm == NULL)
      {
        DebugLog::writeMsg(DebugLog::ERROR,
            "DPELinearRegression::collectData()", "No power meter available");
        return;
      }

    gettimeofday(&tstart, NULL);
#if DEBUG
    std::stringstream ss;
    ss << "power meter latency: ";
    ss << pm_latency;
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::collectData",
        ss.str());
    ss.str("");
    ss << "power estimator latency: ";
    ss << _latency;
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::collectData",
        ss.str());
#endif

    double inputs[_params + 1];
    double output;

    int i;

    _pm->waitUpdate();

    while (elapsed < total)
      {
        gettimeofday(&istart, NULL);

        i = 0;
        if (pm_latency > _latency)
          _pm->waitUpdate();

        output = _pm->getValue().Float;
        for (SensorList::iterator it = _sensors.begin(); it != _sensors.end();
            it++)
          {
            (*it)->update();
            if ((*it)->getType() == Float)
              {
#if DEBUG
                float f = (*it)->getValue().Float;
                inputs[i] = f;
                DebugLog::cout << "  sensor (float): " << inputs[i]
                << DebugLog::endl;
#else
                inputs[i] = (*it)->getValue().Float;
#endif
              }
            else
              {
#if DEBUG
                u64 u = (*it)->getValue().U64;
                inputs[i] = (float) u;
                DebugLog::cout << "  sensor (u64): " << u << DebugLog::endl;
#else
                inputs[i] = (float) (*it)->getValue().U64;
#endif
              }

#if DEBUG
            DebugLog::cout << "  input[" << i << "]: " << inputs[i]
            << DebugLog::endl;
#endif

            i++;
          }
        _lr.addPattern(inputs, &output);

        gettimeofday(&tnow, NULL);
        elapsed = Tools::timevaldiff(tstart, tnow);
        ielapsed = Tools::timevaldiff(istart, tnow);

#if DEBUG
        DebugLog::cout << "  iteration elapsed time (ms): " << ielapsed
        << DebugLog::endl;

        DebugLog::cout << "  total elapsed time (ms):     " << elapsed
        << DebugLog::endl;
#endif

        if (ielapsed < _latency)
          usleep((_latency - ielapsed) * 1000);
      }
  }

  unsigned
  DPELinearRegression::getLatency()
  {
    pm_latency = _pm->getLatency();
    return pm_latency;
  }

  sensor_t
  DPELinearRegression::getValue()
  {
#if DEBUG
    pm_latency = _pm->getLatency();

    for (SensorList::iterator it = _sensors.begin(); it != _sensors.end(); it++)
      {
        if ((*it)->getType() == Float)
          {
            DebugLog::writeMsg(DebugLog::INFO,
                "DPELinearRegression::getValue()",
                Tools::CStr((*it)->getValue().Float));
          }
        else
          {
            DebugLog::writeMsg(DebugLog::INFO,
                "DPELinearRegression::getValue()",
                Tools::CStr((*it)->getValue().U64));
          }
      }
#endif
    return _cValue;
  }

  void
  DPELinearRegression::update()
  {
    double tmp;
    int i = 0;

    tmp = _weights[i];
    for (SensorList::iterator it = _sensors.begin(); it != _sensors.end(); it++)
      {
        (*it)->update();
        i++;
        if ((*it)->getType() == Float)
          tmp += _weights[i] * (*it)->getValue().Float;
        else
          tmp += _weights[i] * ((float) (*it)->getValue().U64);
      }
    _cValue.Float = tmp;
  }

  void
  DPELinearRegression::updatePid(pid_t pid)
  {
    double tmp;
    int i = 0;

    tmp = _weights[i];
    for (SensorList::iterator it = _sensors.begin(); it != _sensors.end(); it++)
      {
        PIDSensor * pd;
        i++;
        pd = dynamic_cast<PIDSensor*>(*it); // dynamic cast to look for PID sensors
        if (pd == 0) // if its a machine level sensor
          {
            if ((*it)->getType() == Float)
              tmp += _weights[i] * (*it)->getValue().Float;
            else
              tmp += _weights[i] * (*it)->getValue().U64;
          }
        else // PID sensor
          {
            if (pd->getType() == Float)
              tmp += _weights[i] * pd->getValuePid(pid).Float;
            else
              tmp += _weights[i] * pd->getValuePid(pid).U64;
          }
      }
    _cValue.Float = tmp;
  }

  std::ostream&
  operator<<(std::ostream &out, DPELinearRegression &cPoint)
  {
    out << "{";

    // weights
    out << " \"weights\": [" << cPoint._weights[0];
    for (int i = 1; i < cPoint._params + 1; i++)
      out << "," << cPoint._weights[i];
    out << "],";

    // history
    out << " \"lr\": " << cPoint._lr << "";

    out << " }";
    return out;
  }

  std::istream&
  operator>>(std::istream &in, DPELinearRegression &cPoint)
  {
#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::operator>>()",
        "-- in");
#endif

    std::string s;

    std::getline(in, s);
    unsigned found_w = s.find("weights");
    if (found_w != std::string::npos)
      {
        unsigned found_ws = s.substr(found_w).find("[");
        unsigned found_we = s.substr(found_w).find("]");
        if ((found_ws != std::string::npos) && (found_we != std::string::npos))
          {
            std::stringstream ss;
            ss << s.substr(found_w).substr(found_ws + 1, found_we - found_ws);
            char c = ',';
            for (int i = 0; i < cPoint._params + 1; i++)
              {
                ss >> cPoint._weights[i];
                ss >> c;
                if (c != ',')
                  DebugLog::writeMsg(DebugLog::ERROR,
                      "DPELinearRegression::operator>>()", "Bad input file.");
              }
          }
      }

#if DEBUG
    DebugLog::writeMsg(DebugLog::INFO, "DPELinearRegression::operator>>()",
        "-- out");
#endif

    return in;
  }

  void
  DPELinearRegression::clean()
  {
    DynamicPowerEstimator::clean();

    _latency = 1000; // 1 second
    _pm = NULL;

    if (_weights != NULL)
      {
        delete[] _weights;
        _weights = NULL;
      }
  }

} /* namespace cea */

