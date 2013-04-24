///////////////////////////////////////////////////////////////////////////////
/// @file		Log.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Log class which use logger and formatter
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_LOG_H__
#define LIBEC_LOG_H__

#include <iostream>
#include <string>
#include <list>

#include "../Globals.h"
#include "../value/Value.h"

#include "../formatter/base/BaseFormat.h"
#include "../formatter/base/BaseFormatter.h"
#include "../formatter/FormatType.h"

#include "base/BaseLog.h"
#include "base/BaseLogger.h"
#include "LogType.h"

#include "../tools/Tools.h"

namespace cea
{

  /// @brief Log class which use logger and formatter
  class Log
  {
  public:

    /* Constructor/Destructor */
    /// @brief Default constructor
    Log();
    /// @brief Create a log with a predefined type
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // Set file log with output file "out.log"
    ///     Log log(FileLog("out.log"));
    /// @endcode
    ///
    /// @param logType Type of log to use
    Log(const BaseLog& logType);
    /// @brief Create a log with a predefined type and format
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // Set file log with output file "out.log" and CSV format
    ///     Log log(FileLog("out.log"), CSVFormat(";"));
    /// @endcode
    ///
    /// @param logType Type of log to use
    /// @param logFormat Format of log to use
    Log(const BaseLog& logType, const BaseFormat& logFormat);

    /// @brief Delete all created memory
    virtual
    ~Log();

    /* Logger/Formatter property */
    /// @copydoc Log::setLogger
    void
    set(const BaseLog& logType);
    /// @copydoc Log::setFormat
    void
    set(const BaseFormat& logFormat);
    /// @brief Set log type and format
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // A Log
    ///     Log log;
    ///
    ///     // Set file log with output file "out.log" and CSV format
    ///     log.set(FileLog("out.log"), CSVFormat(";"));
    /// @endcode
    ///
    /// @param logType New type of log to use
    /// @param logFormat New format of log to use
    void
    set(const BaseLog& logType, const BaseFormat& logFormat);

    /// @brief Get the log type
    /// @return Pointer to the logger or 0 if no logger associated
    BaseLogger*
    getLogger();
    /// @brief Set log type
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // A Log
    ///     Log log;
    ///
    ///     // Set file log with output file "out.log"
    ///     log.set(FileLog("out.log"));
    /// @endcode
    ///
    /// @param logType New type of log to use
    void
    setLogger(const BaseLog& logType);

    /// @brief Get the log format
    /// @return Pointer to the formatter or 0 if no formatter associated
    BaseFormatter*
    getFormat();
    /// @brief Set log format
    ///
    /// <h3>Example</h3>
    /// @code
    ///     // A Log
    ///     Log log;
    ///
    ///     // Set XML format in balise mode
    ///     log.set(XMLFormat(true));
    /// @endcode
    ///
    /// @param logFormat New format of log to use
    void
    setFormat(const BaseFormat& logFormat);

    /// @copydoc Log::setLogger
    void
    setOption(const BaseLog& logType);
    /// @copydoc Log::setFormat
    void
    setOption(const BaseFormat& logFormat);

    /* Block type property */
    /// @copydoc BaseFormatter::setBlockType
    void
    setBlockType(FormatBlockType blockType, const std::string& name);
    /// @copydoc BaseFormatter::getBlockType
    std::string
    getBlockType(FormatBlockType blockType);

    /* Auto Flush Property */
    /// @brief Get auto flush frequency
    /// @return Auto flush frequency in ms. \n
    ///	        Or 0 if no auto flush by frequency activate.
    cea_time_t
    getFlushFrequency() const;
    /// @brief Set auto flush frequency
    /// @param frequency Auto flush frequency in ms. \n
    ///	                 Or 0 if no auto flush by frequency activate.
    void
    setFlushFrequency(cea_time_t frequency);

    /// @brief Get auto flush item count
    /// @return Auto Flush Item Count. \n
    ///         Or 0 if no auto flush by item count activate
    unsigned int
    getFlushItemCount() const;
    /// @brief Set auto flush by item count
    /// @param itemCount Auto Flush Item Count. \n
    ///                  Or 0 if no auto flush by item count activate.
    void
    setFlushItemCount(unsigned int itemCount);

    /// @brief Get Item count pending for flush
    /// @return Item count pending for flush
    unsigned int
    getItemCount() const;

    /* Log function */
    /// @brief Clear the flush pending data
    void
    clear();

    /// @copydoc BaseFormatter::openBlockType
    void
    openBlockType(FormatBlockType blockType);

    /// @brief Open a named block follow log format
    /// @param name Name of the block to open
    void
    openBlock(const std::string& name = "");

    /// @brief Add to log an formatted entry follow log format
    /// @param data Data to log
    void
    write(const Value& data);

    /// @brief Add to log an formatted entry follow log format
    /// @param name Key of the entry
    /// @param data Data to log
    void
    write(const std::string& name, const Value& data);

    /// @copydoc BaseFormatter::closeBlockType
    void
    closeBlockType(FormatBlockType blockType);
    /// @brief Close a named block follow log format
    /// @param name Name of the block to close
    void
    closeBlock(const std::string& name = "");

    /// @brief Add to log an formatted comment follow log format
    /// @param comment Comment to log
    void
    addComment(const std::string& comment);

    /* Flush */
    /// @brief Flush all pending data
    /// @return True if succeed
    bool
    flush();

    /// @brief Do auto flush if needed
    /// @see Log::setFlushItemCount, Log::setFlushFrequency
    void
    update();

  protected:

    /* Members */
    BaseLogger* _logger; ///< Logger used
    BaseFormatter* _format; ///< Formatter used

    /// @brief Auto flush by frequency in millisecond.
    ///        If = 0 Flush need to be done manually
    cea_time_t _autoFlushFrequency;
    unsigned int _autoFlushItemCount; ///< Auto flush by item count.
    cea_time_t _lastFlushTick; ///< Last time that an auto flush was done

    unsigned int _dataToFlushCount; ///< Data pending for flush

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Log
///	@ingroup log
///
/// <h3>Example</h3>
/// @code
///     // Create a file CSV log
///     // Log will be saved in test.log and the separated for CSV will be ";"
///     Log log(FileLog("test.log"), CSVFormat(";"));
///     
///     // Add CSV entries
///     log.openBlock();
///     log.write("name","ectop");
///     log.write("pid", 2502);
///     log.write("cpu", "10%");
///     log.closeBlock();
///     
///     // Change log format to XML attribute mode
///     log.setOption(XMLFormat(true));
///     
///     // Change log output to "out.log" and clear the file before add entries
///     log.setOption(FileLog("out.log", true));
///     
///     // Add XML entries
///     log.openBlockType(ITEM_ADDED);
///     log.write("name", "ectop");
///     log.write("pid", 2502);
///     log.write("cpu", 0.98);
///     log.closeBlockType(ITEM_ADDED);
///      
///     // Flush log data
///     log.flush();
/// @endcode		
///
///////////////////////////////////////////////////////////////////////////////
