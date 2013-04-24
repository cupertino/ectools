///////////////////////////////////////////////////////////////////////////////
/// @file		DebugLog.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Provide a global and unique debug stream to debug code
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_DEBUGLOG_H__
#define LIBEC_DEBUGLOG_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/time.h>

namespace cea
{

  /// @brief Provide a global and unique debug stream to debug code
  class DebugLog
  {
  private:

    /* Constructor */
    /// @brief Default debug log is a Type::STREAM and output stream is
    ///        std::cerr.
    ///
    /// The constructor is private to avoid instanciation
    /// and so make the class a singleton.
    ///
    DebugLog();

  public:

    /* Enums/const */
    /// @brief Type of the debug log
    enum Type
    {
      FILE, ///< Ouput the debug entries into a file
      STREAM ///< Output the debug entries into a given stream
    };

    /// \brief Type of message to be logged
    enum MsgType
    {
      INFO = 0, WARNING = 1, ERROR = 2
    };

    static const char endl; ///< Represent the end of line chars

    /* Public Member */
    /// @brief Unique instance of DebugLog
    ///
    /// This member allow to use DebugLog same as std::cout:
    /// @code
    ///     DebugLog::cout << "Result = " << 5.45 << DebugLog::endl;
    /// @endcode
    ///
    static DebugLog cout;

    /* Destructor */
    /// @brief Destroy the debug log stream if created
    ~DebugLog();

    /* Create functions */
    /// @brief Initialize the log into Type::FILE
    /// @param file Output file to use
    /// @param appendToFile If false Then file will be cleared
    /// @param autoAddHeader If true Then call DebugLog::writeHeader()
    /// @return Reference to the unique DebugLog instance
    static DebugLog&
    create(const std::string& file = "dbg.log", bool appendToFile = true,
        bool autoAddHeader = true);
    /// @brief Initialize the log into Type::STREAM
    /// @param stream Output stream to use
    /// @param autoAddHeader If true Then call DebugLog::writeHeader()
    /// @return Reference to the unique DebugLog instance
    static DebugLog&
    create(std::ostream& stream, bool autoAddHeader = true);

    /* Log functions */
    /** Return a reference to the stream */
    /// @brief Get a reference to the unique instance of DebugLog
    ///
    /// <h3>Example</h3>
    /// @code
    ///     DebugLog::out() << "Result = " << 5.45 << DebugLog::endl;
    /// @endcode
    ///
    /// @return Reference to the unique instance of DebugLog
    static DebugLog&
    out();

    /// @brief Change type of debug log into Type::FILE
    /// @param file Output file to use
    void
    setOutput(const std::string& file);
    /// @brief Change type of debug log into Type::STREAM
    /// @param stream Output Stream to use
    void
    setOutput(std::ostream& stream);

    /// @brief Write header entry in the debug log
    ///
    /// <h3>Header format</h3>
    /// @code
    ///     ------------------
    ///     start log at $CURRENT_DATE$
    ///     --
    /// @endcode
    ///
    void
    writeHeader();

    static void
    writeMsg(MsgType type, std::string classname, std::string msg);

    /// @brief Clear the log only work with Type::FILE
    static void
    clear();

    /// @brief Write entry by stream output operator in the debug log
    /// @param data Entry to add
    /// @return Reference to the unique and global DebugLog
    template<typename T>
      DebugLog&
      operator<<(const T& data);

    /// @brief Write an entry in the debug log
    /// @param data Entry to add
    /// @return Reference to the unique and global DebugLog
    template<typename T>
      static DebugLog&
      write(const T& data);
    /// @brief Write an entry and add a line in the debug log
    /// @param data Entry to add
    /// @return Reference to the unique and global DebugLog
    template<typename T>
      static DebugLog&
      writeln(const T& data);

  protected:

    /* Members */
    Type _type; ///< Current type of the DebugLog
    std::ostream* _buffer; ///< Pointer to the stream for output entries
    std::string _lastFile; ///< File path of the DebugLog

  };

  /// @brief Allias for DebugLog class
  typedef DebugLog dbgLog;

  /* Stream write */
  template<typename T>
    DebugLog&
    DebugLog::operator<<(const T& data)
    {
      if (_buffer != 0)
        {
          (*_buffer) << data;
          _buffer->flush();
        }
      return (*this);
    }

  /* Utils function */
  template<typename T>
    DebugLog&
    DebugLog::write(const T& data)
    {
      return (DebugLog::cout << data);
    }
  template<typename T>
    DebugLog&
    DebugLog::writeln(const T& data)
    {
      return (DebugLog::cout << data << DebugLog::endl);
    }

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::DebugLog
///	@ingroup tools
///
/// To simplify usage of DebugLog a typedef dbgLog == DebugLog is provide
///
/// The DebugLog class is a singleton. This means that DebugLog class cannot be
/// instanciated. \n
/// To access to the unique DebugLog instance static member cout and out()
/// static function can be used. \n \n
/// 
/// DebugLog by default output entries into std::cerr. \n
/// However its possible to change the output of the DebugLog and set
/// output into a file or into a std::stream (such as cout,cerr). \n \n
/// 
/// To write entry to the log you can use operator << or the write and 
/// writeln function.
/// 
/// <h3>Example</h3>
/// @code
///     // Ouput debug log entries into file "out.log" and add header entry
///     DebugLog::create("out.log");
///     
///     // Write entries
///     int i = 42;
///     DebugLog::cout << "Result = " << i << DebugLog::endl;
///     // or equivalent
///     DebugLog::out() << "Result = " << i << dbgLog::endl;
///     // or
///     dbgLog::write("Result = "); dbgLog::writeln(i);
///     
/// @endcode
/// 
///////////////////////////////////////////////////////////////////////////////
