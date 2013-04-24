///////////////////////////////////////////////////////////////////////////////
/// @file		logs.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		All headers file needed for log and format
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_LOGS_H__
#define LIBEC_LOGS_H__

/* Log Base */
#include "log/LogType.h"
#include "log/Log.h"
#include "log/base/BaseLog.h"
#include "log/base/BaseLogger.h"

/* Log File */
#include "log/file/FileLog.h"
#include "log/file/FileLogger.h"

/* Formatter Base */
#include "formatter/FormatType.h"
#include "formatter/base/BaseFormat.h"
#include "formatter/base/BaseFormatter.h"

/* Formatter CSV */
#include "formatter/csv/CSVFormat.h"
#include "formatter/csv/CSVFormatter.h"

/* Formatter XML */
#include "formatter/xml/XMLFormat.h"
#include "formatter/xml/XMLFormatter.h"

/* Formatter XML */
#include "formatter/xml/XMLFormat.h"
#include "formatter/xml/XMLFormatter.h"

/* Formatter Gnuplot */
#include "formatter/gnuplot/GnuplotFormat.h"
#include "formatter/gnuplot/GnuplotFormatter.h"

/* Formatter Json */
#include "formatter/json/JsonFormat.h"
#include "formatter/json/JsonFormatter.h"


#endif

///////////////////////////////////////////////////////////////////////////////
/// @defgroup logmod Log module
/// @brief Log and format
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @defgroup format Format section
///	@ingroup logmod
/// @brief Formatter and their settings
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @defgroup log Log section
///	@ingroup logmod
/// @brief Logger and their settings
///////////////////////////////////////////////////////////////////////////////

