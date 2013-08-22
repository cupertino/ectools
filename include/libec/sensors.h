///////////////////////////////////////////////////////////////////////////////
/// @file		sensors.h
/// @author		Leandro Fontoura Cupertino
/// @version	0.1
/// @date		2012.06
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		A header with all the sensors that can be instantiated.
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_SENSORS_H_
#define LIBEC_SENSORS_H_

/* Base */
#include "sensor/Sensor.h"

/* Multi platform Sensor */
#include "sensor/FakeSensor.h"

#include "sensor/SensorController.h"

/* Unix sensors */
#ifdef __unix__
#include "sensor/SensorNetwork.h"
#include "sensor/SensorPerfCount.h"
#include "sensor/SensorCpuFreq.h"
#include "sensor/SensorCpuFreqMsr.h"
#include "sensor/SensorCpuStateTime.h"
#include "sensor/SensorCpuStateTimeElapsed.h"
#include "sensor/SensorCpuStateMsr.h"
#include "sensor/SensorCpuTemp.h"
#include "sensor/SensorCpuUsage.h"
#include "sensor/SensorPidStat.h"
#include "sensor/SensorPidCpuTime.h"
#include "sensor/SensorPidCpuTimeElapsed.h"
#include "sensor/SensorPidCpuTimeUsage.h"
#include "sensor/SensorPidDiskIO.h"
#include "sensor/SensorPidMemRss.h"
#include "sensor/SensorPidMemUsage.h"

#include "sensor/SensorPowerG5k.h"
#include "sensor/SensorPowerRecs.h"
//#include "sensor/SensorPowerRecsTlse.h"
#include "sensor/SensorPowerAcpi.h"
#include "sensor/SensorPowerPlogg.h"
#include "sensor/SensorPowerWattsUp.h"

#include "sensor/SensorRunningProcs.h"
#endif

/* Windows sensors */
#ifdef _WIN32

#endif

#endif /* LIBEC_SENSORS_H_ */

///////////////////////////////////////////////////////////////////////////////
/// @defgroup sensor Sensor module
///////////////////////////////////////////////////////////////////////////////
