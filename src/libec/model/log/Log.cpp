#include <libec/log/Log.h>
#include <libec/tools/Tools.h>

namespace cea
{

  /** Constructor */
  Log::Log() :
      _logger(0), _format(0), _autoFlushFrequency(5000), _autoFlushItemCount(0), _lastFlushTick(
          0), _dataToFlushCount(0)
  {
    ;
  }
  Log::Log(const BaseLog& logType) :
      _logger(0), _format(0), _autoFlushFrequency(5000), _autoFlushItemCount(0), _lastFlushTick(
          0), _dataToFlushCount(0)
  {
    setLogger(logType);
  }
  Log::Log(const BaseLog& logType, const BaseFormat& logFormat) :
      _logger(0), _format(0), _autoFlushFrequency(5000), _autoFlushItemCount(0), _lastFlushTick(
          0), _dataToFlushCount(0)
  {
    setLogger(logType);
    setFormat(logFormat);
  }

  /** +Destructor */
  Log::~Log()
  {
    if (_logger != 0)
      delete _logger;
    if (_format != 0)
      delete _format;
  }

  /** +set */
  void
  Log::set(const BaseLog& logType)
  {
    setLogger(logType);
  }
  /** +set */
  void
  Log::set(const BaseFormat& logFormat)
  {
    setFormat(logFormat);
  }
  /** +set */
  void
  Log::set(const BaseLog& logType, const BaseFormat& logFormat)
  {
    setLogger(logType);
    setFormat(logFormat);
  }

  /** +getLogger */
  BaseLogger*
  Log::getLogger()
  {
    return _logger;
  }
  /** +setLogger */
  void
  Log::setLogger(const BaseLog& logType)
  {
    if (_logger != 0)
      delete _logger;
    _logger = logType.createLogger();
    if (_logger != 0)
      _logger->setOption(logType);
  }

  /** Formatter Property */
  BaseFormatter*
  Log::getFormat()
  {
    return _format;
  }
  /** +setFormat */
  void
  Log::setFormat(const BaseFormat& logFormat)
  {
    if (_format != 0)
      delete _format;
    _format = logFormat.createFormatter();
    if (_format != 0)
      _format->setOption(logFormat);
  }

  /** +setOption */
  void
  Log::setOption(const BaseLog& logType)
  {
    if ((_logger != 0) && (logType.getType() == _logger->getType()))
      {
        _logger->setOption(logType);
      }
    else
      {
        setLogger(logType);
      }
  }
  /** +setOption */
  void
  Log::setOption(const BaseFormat& logFormat)
  {
    if ((_format != 0) && (logFormat.getType() == _format->getType()))
      {
        _format->setOption(logFormat);
      }
    else
      {
        setFormat(logFormat);
      }
  }

  /** +setBlockTypeName */
  void
  Log::setBlockType(FormatBlockType blockType, const std::string& name)
  {
    if (_format != 0)
      {
        _format->setBlockType(blockType, name);
      }
  }
  /** +getBlockType */
  std::string
  Log::getBlockType(FormatBlockType blockType)
  {
    if (_format != 0)
      {
        return _format->getBlockType(blockType);
      }
    else
      {
        return "";
      }
  }

  /** +getFlushFrequency */
  cea_time_t
  Log::getFlushFrequency() const
  {
    return _autoFlushFrequency;
  }

  /** +setFlushFrequency */
  void
  Log::setFlushFrequency(cea_time_t frequency)
  {
    _autoFlushFrequency = frequency;
    _lastFlushTick = Tools::tick();
  }

  /** +getFlushItemCount */
  unsigned int
  Log::getFlushItemCount() const
  {
    return _autoFlushItemCount;
  }

  /** +setFlushItemCount */
  void
  Log::setFlushItemCount(unsigned int itemCount)
  {
    /* Set the item count */
    _autoFlushItemCount = itemCount;
    /* Flush if needed */
    if ((itemCount > 0) && (_dataToFlushCount >= itemCount))
      {
        flush();
      }
  }

  /** +getItemCount */
  unsigned int
  Log::getItemCount() const
  {
    return _dataToFlushCount;
  }

  /** +clear */
  void
  Log::clear()
  {
    if (_format != 0)
      _format->clear();
    _dataToFlushCount = 0;
  }

  /** +openBlockType */
  void
  Log::openBlockType(FormatBlockType blockType)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->openBlockType(blockType);
      }
  }
  /** +openBlock */
  void
  Log::openBlock(const std::string& name)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->openBlock(name);
      }
  }
  /** +write */
  void
  Log::write(const Value& data)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->format(data);
      }
  }
  /** +write */
  void
  Log::write(const std::string& name, const Value& data)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->format(name, data);
      }
  }
  /** +closeBlockType */
  void
  Log::closeBlockType(FormatBlockType blockType)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->closeBlockType(blockType);
      }
  }
  /** +closeBlock */
  void
  Log::closeBlock(const std::string& name)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->closeBlock(name);
      }
  }
  /** +addComment */
  void
  Log::addComment(const std::string& comment)
  {
    if (_format != 0)
      {
        _dataToFlushCount++;
        _format->formatComment(comment);
      }
  }

  /** +flush */
  bool
  Log::flush()
  {
    /* Do flush */
    if ((_logger != 0) && (_format != 0))
      {
        /* Peek format data */
        std::string data;
        _format->peek(data);
        /* Try to flush */
        if (_logger->flush(data))
          {
            /* Clear formatter */
            _format->clear();
            _dataToFlushCount = 0;
            return true;
          }
      }
    /* If arrive here then no flush done */
    return false;
  }

  /** +update */
  void
  Log::update()
  {
    /* Check if auto flush frenquency > 0 ms AND if there is data to flush */
    if ((_autoFlushFrequency > 0) && (_dataToFlushCount > 0) && (_logger != 0)
        && (_format != 0))
      {
        /* Get current Tick */
        cea_time_t tick = Tools::tick();
        /* Check if is it time to flush */
        if ((tick - _lastFlushTick) > _autoFlushFrequency)
          {
            /* Flush */
            if (flush())
              {
                _lastFlushTick = tick;
              }
          }
      }
  }

}
