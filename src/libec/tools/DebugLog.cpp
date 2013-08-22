#include <libec/tools/DebugLog.h>
#include <libec/tools/Tools.h>
#include <cstdarg>

namespace cea
{

  /** Static member */
  DebugLog DebugLog::cout;
  const char DebugLog::endl = '\n';

  /** Construtor */
  DebugLog::DebugLog() :
      _type(STREAM), _buffer(&(std::cerr))
  {
    ;
  }

  /** Destructor */
  DebugLog::~DebugLog()
  {
    if ((_type == FILE) && (_buffer != 0))
      {
        static_cast<std::ofstream*>(_buffer)->close();
        delete _buffer;
        _buffer = 0;
      }
  }

  /** +create */
  DebugLog&
  DebugLog::create(const std::string& file, bool appendToFile,
      bool autoAddHeader)
  {
    /* Create the buffer */
    cout._lastFile.assign(file);
    cout._buffer = new std::ofstream();
    static_cast<std::ofstream*>(cout._buffer)->open(file.c_str(),
        (appendToFile ? std::ios::out | std::ios::app : std::ios::out));
    cout._type = FILE;
    if (cout._buffer == 0)
      return cout;
    /* Add Header */
    if (autoAddHeader)
      cout.writeHeader();
    /* Return the actual instance */
    return cout;
  }
  DebugLog&
  DebugLog::create(std::ostream& stream, bool autoAddHeader)
  {
    /* Create the buffer */
    cout._buffer = &stream;
    cout._type = STREAM;
    /* Add Header */
    if (autoAddHeader)
      cout.writeHeader();
    /* Return the actual instance */
    return cout;
  }

  /** +out */
  DebugLog&
  DebugLog::out()
  {
    return DebugLog::cout;
  }

  /** +setOutput */
  void
  DebugLog::setOutput(const std::string& file)
  {
    /* Create the buffer if not created */
    if ((_type == STREAM) || (_buffer == 0))
      {
        _buffer = new std::ofstream();
        if (_buffer == 0)
          return;
      }
    else if ((_type == FILE) && (_buffer != 0))
      {
        static_cast<std::ofstream*>(_buffer)->close();
        delete _buffer;
        _buffer = 0;
      }
    /* Open file */
    static_cast<std::ofstream*>(_buffer)->open(file.c_str(),
        std::ios::out | std::ios::app);
    _lastFile.assign(file);
    _type = FILE;
  }

  /** +setOutput */
  void
  DebugLog::setOutput(std::ostream& stream)
  {
    /* Close the old buffer */
    if ((_type == FILE) && (_buffer != 0))
      {
        static_cast<std::ofstream*>(_buffer)->close();
        delete _buffer;
        _buffer = 0;
      }
    /* Set the new one */
    _type = STREAM;
    _buffer = &stream;
  }

  /** +writeHeader */
  void
  DebugLog::writeHeader()
  {

    /* DebugLog the header */
    cout << "------------------" << endl << "start log at "
        << Tools::nowToString() << endl << "--" << endl;
  }

  void
  DebugLog::writeMsg(MsgType type, const char* classname, const char* format,
      ...)
  {
    char buffer[256];
    va_list args;

    cout << classname << ": ";
    switch (type)
      {
    case INFO:
      cout << "<info>  ";
      break;
    case WARNING:
      cout << "<warn>  ";
      break;
    case ERROR:
      cout << "<error> ";
      break;
    default:
      break;
      }

    va_start(args, format);
    vsprintf(buffer, format, args);
    cout << buffer << endl;
    va_end(args);
  }

  /** Clear the log */
  void
  DebugLog::clear()
  {
    if ((cout._type == FILE) && (cout._buffer != 0))
      {
        static_cast<std::ofstream*>(cout._buffer)->close();
        /* Open and clear file */
        static_cast<std::ofstream*>(cout._buffer)->open(cout._lastFile.c_str(),
            std::ios::out);
      }
  }

}
