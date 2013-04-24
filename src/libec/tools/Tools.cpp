#include <stdio.h>
#include <sys/time.h>

#include <libec/Globals.h>
#include <libec/tools/Tools.h>
#include <libec/tools.h>

#if DEBUG
#include <libec/tools/DebugLog.h>
#endif

namespace cea
{

  /* Static variable initialization */
  Tools::Randomize Tools::randomize;

  /* -----------------------------------------------------------------------------
   *    Number Method
   * ---------------------------------------------------------------------------*/

  /** +isNumeric(str : string) : bool */
  bool
  Tools::isNumeric(const std::string& str)
  {
    bool dot_flag = false;
    bool e_flag = false;
    bool sign_flag = false;
    bool digit_flag = false;
    unsigned short start_pos = 0;

    if ((str.length() > 0) && ((str[0] == '-') || (str[0] == '+')))
      {
        start_pos = 1;
      }

    /* Check if is number */
    for (unsigned int i = start_pos, max = str.length(); i < max; i++)
      {
        if (str[i] == '.')
          {
            if (dot_flag)
              return false;

            dot_flag = true;
          }
        else if ((str[i] == 'E') || (str[i] == 'e'))
          {
            if ((!digit_flag) || (e_flag))
              return false;
            else
              e_flag = true;
          }
        else if ((str[i] == '+') || (str[i] == '-'))
          {
            if ((!digit_flag) || (sign_flag) || (!e_flag))
              return false;
            else
              sign_flag = true;
          }
        else if ((str[i] < '0') || (str[i] > '9'))
          return false;
        else
          digit_flag = true;
      }
    /* Is a Number return true */
    return true;
  }

  /** +isNumeric(str : char*) : bool */
  bool
  Tools::isNumeric(const char* str)
  {
    bool dot_flag = false;
    bool e_flag = false;
    bool sign_flag = false;
    bool digit_flag = false;

    if ((str != 0) && (*str != '\0') && ((*str == '-') || (*str == '+')))
      {
        str++;
      }

    /* Check if is number */
    while ((str != 0) && (*str != '\0'))
      {
        if (*str == '.')
          {
            if (dot_flag)
              return false;

            dot_flag = true;
          }
        else if ((*str == 'E') || (*str == 'e'))
          {
            if ((!digit_flag) || (e_flag))
              return false;
            else
              e_flag = true;
          }
        else if ((*str == '+') || (*str == '-'))
          {
            if ((!digit_flag) || (sign_flag) || (!e_flag))
              return false;
            else
              sign_flag = true;
          }
        else if ((*str < '0') || (*str > '9'))
          {
            return false;
          }
        else
          digit_flag = true;
        str++;
      }
    /* Is a Number return true */
    return digit_flag;
  }

  /** +isNumeric(str : char*, len : int) : bool */
  bool
  Tools::isNumeric(const char* str, unsigned int len)
  {
    bool dot_flag = false;

    unsigned int i = 0;
    /* Check if is number */
    while ((str != 0) && (*str != '\0') && (i < len))
      {
        if (*str == '.')
          {
            if (dot_flag)
              return false;

            dot_flag = true;
          }
        else if ((*str < '0') || (*str > '9'))
          return false;
        str++;
        i++;
      }
    /* Is a Number return true */
    return true;
  }

  /** +getFileName(path : string) : string */
  std::string
  Tools::getFileName(const std::string& path)
  {
    /* Search '/' or '\' */
    unsigned int i = path.length();
    while (i > 0)
      {
        i--;
        switch (path[i])
          {
        case '/':
        case '\\':
          return path.substr(i + 1);
          }
      }
    /* No found return path */
    return path;
  }

  /** +identicalString */
  bool
  Tools::identicalString(const std::string& str1, const std::string& str2)
  {
    /* Check if same size */
    unsigned int max = str1.length();
    if (max != str2.length())
      return false;
    /* Check if same char */
    for (unsigned int i = 0; i < max; i++)
      {
        /* Get Char */
        unsigned char c1 = str1[i];
        unsigned char c2 = str2[i];
        /* Check if same char */
        if (c1 != c2)
          {
            if ((c1 >= 'A') && (c1 <= 'Z'))
              c1 += 32;
            if ((c2 >= 'A') && (c2 <= 'Z'))
              c2 += 32;
            if (c1 != c2)
              return false;
          }
      }
    /* String identical return true */
    return true;
  }

  /** +toLower */
  std::string
  Tools::toLower(std::string str)
  {
    /* Convert to lower case all char */
    for (unsigned int i = 0, max = str.length(); i < max; i++)
      {
        if ((str[i] >= 'A') && (str[i] <= 'Z'))
          {
            str[i] += 32;
          }
      }
    /* Return value */
    return str;
  }

  /** +getWhiteSpaceToken(str : string, tokenId : int) : string */
  std::string
  Tools::getWhiteSpaceToken(const std::string& str, unsigned int tokenId)
  {
    unsigned int begin = 0;
    char currentStringChar = '\0'; /* '\0' = not in a string */
    /* Search the Token */
    for (unsigned int i = 0, max = str.length(); i < max; i++)
      {
        if (currentStringChar == '\0')
          {
            /* Not in a string --> search for a whitespace char */
            switch (str[i])
              {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
            case '\0':
            case ':':
              /* Token Found */
              if ((tokenId == 0) && (begin < i))
                {
                  /* Return the token */
                  return str.substr(begin, i - begin);
                }
              else
                {
                  /* Set begin to i position + 1 */
                  begin = i + 1;
                  /* Advance the current token if token not empty */
                  if (begin < i)
                    tokenId--;
                }
              break;
            case '\"':
            case '\'':
            case '`': /* String Char */
              /* Enter in String mode */
              currentStringChar = str[i];
              begin++;
              break;
              }
          }
        else
          {
            /* Find the string char */
            if (str[i] == currentStringChar)
              {
                /* Token Found */
                if ((tokenId == 0) && (begin < i))
                  {
                    /* Return the token */
                    return str.substr(begin, i - begin);
                  }
                else
                  {
                    /* Set begin to i position + 1 */
                    begin = i + 1;
                    /* Advance the current token if token not empty */
                    if (begin < i)
                      tokenId--;
                  }
              }
          }
      }
    /* If current Token == 0 return the string */
    if (str.empty())
      return "";
    return str.substr(begin);
  }

  /** +tick */
  cea_time_t
  Tools::tick()
  {
#ifdef __unix__
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
      {
        return 0;
      }
    else
      {
//        return (tv.tv_usec + 1000000 * tv.tv_sec) / 1000;
        return (tv.tv_usec / 1000 + 1000 * tv.tv_sec);
      }
#endif
#ifdef _WIN32
    return GetTickCount();
#endif
  }

  long
  Tools::timevaldiff(const struct timeval &start, const struct timeval &end)
  {
#if DEBUG
    DebugLog::cout << "Tools::timevaldiff -- in" << DebugLog::endl;
#endif
    long msec;
    msec = (end.tv_sec - start.tv_sec) * 1000;
    msec += (end.tv_usec - start.tv_usec) / 1000;

#if DEBUG
    DebugLog::cout << "  Start time (s.us): " << start.tv_sec << "." << start.tv_usec << DebugLog::endl;
    DebugLog::cout << "  End time (s.us):   " << end.tv_sec << "." << end.tv_usec << DebugLog::endl;
    DebugLog::cout << "  Elapsed time (ms): " << msec << DebugLog::endl;
    DebugLog::cout << "Tools::timevaldiff -- out" << DebugLog::endl;
#endif
    return msec;
  }

  long
  Tools::usecdiff(const struct timeval *start, const struct timeval *end)
  {
    long usec;
    usec = end->tv_usec - start->tv_usec;
    return usec;
  }

  /** +rnd */
  int
  Tools::rnd()
  {
    return rand();
  }
  /** +rnd */
  int
  Tools::rnd(int min, int max)
  {
    return (rand() % (max - min + 1) + min);
  }

  /** @return the current date in string */
  std::string
  Tools::nowToString()
  {
    /* Get the current Date */
    time_t now;
    time(&now);
#if defined _WIN32 && defined _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
    char cNow[255];
    ctime_s(cNow, 255, &now);
#else
    char* cNow = ctime(&now);
#endif
    char* cPtr = cNow;
    unsigned int szNow = 0;
    std::string s;
    for (; (*cPtr != '\0') && (*cPtr != '\n') && (szNow < 80); cPtr++, szNow++)
      ;
    s.assign(cNow, szNow);
    /* Return the date */
    return s;
  }

  /** +sleep */
  void
  Tools::sleep_ms(unsigned int ms)
  {
#ifdef _WIN32
    Sleep(ms);
#endif
#ifdef __unix__
    struct timespec ts =
      { 0 };
    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);
    ts.tv_sec = sec;
    ts.tv_nsec = ms * 1000000L;
    while (nanosleep(&ts, &ts) == -1)
      continue;
#endif
  }

  /** -hexToInt */
  signed int
  Tools::hexToInt(const std::string& hexStr)
  {
    signed int x;
    std::stringstream ss;
    /* Get Hex */
    ss << std::hex << hexStr;
    /* Return int value */
    ss >> x;
    return x;
  }

  /** +htmlColorToRGB */
  color_t
  Tools::htmlColorToRGB(const std::string& code)
  {
    /// Check if "-1"
    if (code.compare("-1") == 0)
      {
        return -1;
      }
    /// Format code
    bool bFormatRRGGBB = (code.length() >= 5);
    unsigned int j = 0, id = 0;
    std::string rgbStr[3];
    /* Prepare String */
    rgbStr[0].resize(2, '0');
    rgbStr[1].resize(2, '0');
    rgbStr[2].resize(2, '0');
    /* Fill String */
    for (unsigned int i = 0, max = code.length(); i < max; i++)
      {
        switch (code[i])
          {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
          /* Get Id */
          if (bFormatRRGGBB)
            {
              rgbStr[id][j] = code[i];
            }
          else
            {
              rgbStr[id][0] = code[i];
              rgbStr[id][1] = code[i];
              j++;
            }
          /* Advance */
          j++;
          if (j > 1)
            {
              j = 0;
              id++;
            }
          break;
          }
        if (id >= 3)
          break;
      }
    /// Return value
    /* Get R,G,B */
    unsigned char r = (unsigned char) (hexToInt(rgbStr[0])), g =
        (unsigned char) (hexToInt(rgbStr[1])), b = (unsigned char) (hexToInt(
        rgbStr[2]));
    /* Return the RGB value */
    return (((b) << 16) | ((g) << 8) | (r));
  }

  std::string
  Tools::exec(const std::string &cmd)
  {
    return Tools::exec(cmd.c_str());
  }

  std::string
  Tools::exec(const char* cmd)
  {
#ifdef _WIN32
    FILE* pipe = _popen(cmd, "r");
#else
    FILE* pipe = popen(cmd, "r");
#endif
    if (!pipe)
      return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
      {
        if (fgets(buffer, 128, pipe) != NULL)
          result += buffer;
      }
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    return result;
  }

  bool
  Tools::fileExists(const std::string& filename)
  {
    return fileExists(filename.c_str());
  }

  bool
  Tools::fileExists(const char* filename)
  {
    std::ifstream ifile(filename);
    return ifile;
  }

}
