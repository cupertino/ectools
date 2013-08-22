/*
 * CpuIdleStates.cpp
 *
 *  Created on: 3 juil. 2013
 *      Author: fontoura
 */

#include <cstdio>

#include <libec/sensor/SensorCpuStateTimeElapsed.h>
#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>

namespace cea
{
  CpuStateTimeElapsed::CpuStateTimeElapsed(int stateId, int cpuId)
  {
    _name = "CPU_ELAPSED_TIME_IN_STATE_" + Tools::CStr(cpuId);
    _alias = "CStateET_" + Tools::CStr(cpuId);
    _type = U64;
    _cValue.U64 = 0;
    _prev = 0;

    _cst = new CpuStateTime(stateId, cpuId);
    _isActive = _cst->getStatus();
    _name.append(_cst->getStateName());
    _alias.append(_cst->getStateName());

    if (_isActive)
      _cst->update();
  }

  CpuStateTimeElapsed::~CpuStateTimeElapsed()
  {
    delete (_cst);
  }

  void
  CpuStateTimeElapsed::update()
  {
    _prev = _cst->getValue().U64;
    _cst->update();
    _cValue.U64 = _cst->getValue().U64 - _prev;
  }

  sensor_t
  CpuStateTimeElapsed::getValue()
  {
    return _cValue;
  }

} /* namespace cea */
