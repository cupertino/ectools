///////////////////////////////////////////////////////////////////////////////
/// @file		libec/tools/Tools.h
/// @author		Dorian RODDE, Leandro Fontoura Cupertino
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Static tools function
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_TOOLS_H__
#define LIBEC_TOOLS_H__

#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __unix__
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#endif

namespace cea
{

  /// @ingroup tools
  /// @brief Time Unit
  typedef unsigned long long int cea_time_t;

  /// @ingroup tools
  /// @brief Color Unit
  typedef int color_t;

  /// @brief Class containing static tools function
  class Tools
  {
  private:

    /* Private class */
    /// @brief Randomize initializer
    class Randomize
    {
    public:
      /// @brief Initialize the rand seed with current time
      Randomize()
      {
        srand((unsigned int) (time(0)));
      }
    };
    /// @brief Execute Randomize construct code at the beginning of the app
    static Randomize randomize;

  public:
    /// \brief Check if file exists
    /// \param filename  File name
    static bool
    fileExists(const std::string& filename);

    /// \brief Check if file exists
    /// \param filename  File name
    static bool
    fileExists(const char* filename);

    /// Time difference between two timeval in milliseconds (ms)
    static long
    timevaldiff(const struct timeval &start, const struct timeval &end);

    /// Time difference between two timeval.tv_usec in microseconds (us)
    static long
    usecdiff(const struct timeval *start, const struct timeval *end);

    /* Test functions */
    /// @brief Check if a string represent a number
    /// @param str String to test
    /// @return true if str represent a number
    static bool
    isNumeric(const std::string& str);
    /// @brief Check if a string represent a number
    /// @param str String as const char pointer to test
    /// @return true if str represent a number
    static bool
    isNumeric(const char* str);
    /// @brief Check if a string represent a number
    /// @param str String as const char pointer to test
    /// @param len Len of string to test
    /// @return true if str represent a number
    static bool
    isNumeric(const char* str, unsigned int len);

    /* Convert functions */
    /// @brief Convert almost any variable into a string
    /// @param data Variable to convert
    /// @return Corresponding string
    template<typename T>
      static std::string
      CStr(const T& data);

    /// @brief Convert a string into almost any variable
    /// @param data String to convert
    /// @param to Out variable converted
    template<typename T>
      static void
      CStrTo(const std::string& data, T& to);

    /* String functions */
    /// @brief Check if two string are identical, ignoring case
    /// @param str1 First String considered
    /// @param str2 Second String to compare with
    /// @return true if strings are identical
    static bool
    identicalString(const std::string& str1, const std::string& str2);

    /// @brief Convert all char of a string into lower case
    /// @param str String to convert
    /// @return Lower case version of str
    static std::string
    toLower(std::string str);
    /// @brief Retrive file title of a file path
    ///
    /// <h3>Example</h3>
    /// @code
    ///     std::cout << Tools::getFileName("dir1/dir2/file.txt")
    ///               << std::endl;
    ///
    ///     // Will output to the screen
    ///     // file.txt
    /// @endcode
    ///
    /// @param path File path
    /// @return File title
    static std::string
    getFileName(const std::string& path);

    /// @brief Get the nth token in a string
    ///
    /// Token are separated by whitespace char. \n
    /// This function consider string litteral with char '"', '\'', '`'
    ///
    /// <h3>Example</h3>
    /// @code
    ///     std::string text = "token1   \"token \' 2\"\token3";
    ///
    ///     // Get the first token
    ///     std::string tok1 = Tools::getWhiteSpaceToken(text);
    ///     // tok1 = "token1";
    ///
    ///     // Get the second token
    ///     std::string tok2 = Tools::getWhiteSpaceToken(text,1);
    ///     // tok2 = "token ' 2";
    ///
    ///     // Get the third token
    ///     std::string tok3 = Tools::getWhiteSpaceToken(text, 2);
    ///     // tok3 = "token3";
    ///
    /// @endcode
    ///
    /// @param str String to parse
    /// @param tokenId Token number to retrieve (start from 0)
    /// @return The tokenId Token of str Or "" if no token found
    static std::string
    getWhiteSpaceToken(const std::string& str, unsigned int tokenId = 0);

    /* Random functions */
    /// @brief Get a random number
    /// @return A random number
    static int
    rnd();
    /// @brief Get a random number in a specific range
    ///
    /// If (max not precised) Then max = RAND_MAX
    ///
    /// @param min Minimum number
    /// @param max Maxmimum number
    /// @return A random number between min and max included
    static int
    rnd(int min, int max = RAND_MAX);

    /* Time functions */
    /// @brief Get system time in milliseconds
    ///
    /// This function work as well on UNIX platforms and on
    /// Windows platforms. \n
    ///
    /// <h3>Return values</h3>
    ///     <ul>
    ///         <li><b>UNIX platforms</b> == Current time expressed in
    ///              milliseconds (man gettimeofday)</li>
    ///         <li><b>Windows platforms</b> == Number of milliseconds that
    ///             have elapsed since the system was started, up to 49.7
    ///             days. (see GetTickCount function on MSDN)</li>
    ///     </ul>
    ///
    /// @return A time in milliseconds depending of the platform
    static cea_time_t
    tick();

    /// @brief Convert the current date into a string
    /// @return The current date converted into a string
    static std::string
    nowToString();

    /// @brief Sleep the current process for a period expressed in
    ///        milliseconds
    ///
    /// This function work as well on UNIX platforms and on
    /// Windows platforms.
    ///
    /// @param ms Sleep time in millisecond
    static void
    sleep_ms(unsigned int ms);

    /* Color functions */
    /// @brief Convert a hexadecimal string into a signed number
    /// @param hexStr hexadecimal string to parse
    /// @return the decimal number
    static signed int
    hexToInt(const std::string& hexStr);

    /// @brief Transform a color format "#rrggbb" or "rrggbb" into rgb color
    /// @param code HTML color code to convert
    /// @return RGB color corresponding or 0 if bad code format
    static color_t
    htmlColorToRGB(const std::string& code);

    // -------------------------------------------------------------------
    // System functions
    /// \brief Execute a string as a system call and returns the output
    ///        as a string
    ///
    /// This function work as well on UNIX platforms and on
    /// Windows platforms. \n
    ///
    /// \param cmd Command to execute
    /// \return Output string result of the executed command
    static std::string
    exec(const char* cmd);

    /// \brief Execute a string as a system call and returns the output
    ///        as a string
    ///
    /// This function work as well on UNIX platforms and on
    /// Windows platforms. \n
    ///
    /// \param cmd Command to execute
    /// \return Output string result of the executed command
    static std::string
    exec(const std::string &cmd);
  };

  /* +CStr */
  template<typename T>
    std::string
    Tools::CStr(const T& data)
    {
      std::ostringstream oss;
      oss << data;
      return oss.str();
    }

  /* +CStrTo */
  template<typename T>
    void
    Tools::CStrTo(const std::string& data, T& to)
    {
      std::stringstream oss;
      oss << data;
      oss >> to;
    }
}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Tools
///	@ingroup tools
/// 
/// All the function of Tools class work as well on UNIX platforms and on 
/// Windows platforms.
/// 
///////////////////////////////////////////////////////////////////////////////
