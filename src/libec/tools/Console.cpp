#include <libec/grid/view/GridStyle.h>
#include <libec/tools/Console.h>
#include <libec/tools/Color.h>
#include <libec/tools/DebugLog.h>

namespace cea
{

  /** Static variable instanciation */
  Console Console::_console;
  Console::DrawStyle Console::_defaultStyle;
  Console::DrawStyle Console::_currentStyle;
  bool Console::_defaultCursorIsShow = true;
  bool Console::_currentCursorIsShow = true;
  Console::Point Console::_currentCursorPos;

#ifdef __unix__
  Console::NcursesLoader Console::ncurses;
  short Console::NcursesLoader::ncurseColorPair[8][8];
  short Console::NcursesLoader::ncurseColorPairLoaded = 0;
  WINDOW* Console::NcursesLoader::win = NULL;
#endif

#ifdef _WIN32
  HANDLE Console::handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

  /** -Constructor */
  Console::Console()
  {
    Console::_defaultCursorIsShow = true;
    Console::_defaultStyle.backColor = Color::Black;
    Console::_defaultStyle.color = Color::Gray;
    Console::_defaultStyle.bold = false;
    Console::_currentStyle = Console::_defaultStyle;
    Console::_currentCursorPos.x = -1;
    Console::_currentCursorPos.y = -1;
    /* Check if the cursor is visible */
#ifdef _WIN32
    /// TODO
#endif
#ifdef __unix__
#endif
    /* Get the default terminal style */
#ifdef _WIN32
    /// TODO
#endif
#ifdef __unix__
#endif
  }

  /** -Destructor */
  Console::~Console()
  {
    /* At the correct end of the software this function will be called and
     * will reinitialize the console */
    Console::resetDefault();
  }

  /** Return the key hit or 0 if no key hit */
  int
  Console::keyHit()
  {
#ifdef _WIN32
    if (_kbhit())
      {
        return _getch();
      }
    else
      {
        return 0;
      }
#endif
#ifdef __unix__
    int k = getch();
    if (k == ERR)
      return 0;
    return k;
#endif
  }

  /** +moveConsoleCursor */
  void
  Console::moveCursor(int x, int y)
  {
    if ((Console::_currentCursorPos.x == x)
        && (Console::_currentCursorPos.y == y))
      return;
    Console::_currentCursorPos.x = x;
    Console::_currentCursorPos.y = y;
#ifdef _WIN32
    COORD curPos;
    curPos.X = x;
    curPos.Y = y;
    SetConsoleCursorPosition(Console::handleConsole,curPos);
#endif
#ifdef __unix__
    /// TODO use ncurses instead of escaped char
    printf("\033[%d;%df", y + 1, x + 1);
#endif
  }

  /** +getCursorPosition */
  Console::Point
  Console::getCursorPosition()
  {
    static Point csPosition;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(Console::handleConsole, &info);
    csPosition.x = info.dwCursorPosition.X;
    csPosition.y = info.dwCursorPosition.Y;
#endif
#ifdef __unix__
    getyx(stdscr, csPosition.y, csPosition.x);
#endif
    return csPosition;
  }

  /** Return the size of the Console */
  Console::Size
  Console::getSize()
  {
    static Size csSize;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(Console::handleConsole, &info);
    csSize.width = info.srWindow.Right-info.srWindow.Left-1;
    csSize.height = info.srWindow.Bottom-info.srWindow.Top-1;
#endif
#ifdef __unix__
    getmaxyx(stdscr, csSize.height, csSize.width);
#endif
    return csSize;
  }

  /** DrawText */
  void
  Console::drawText(const std::string& txt)
  {
#ifdef __unix__
    addstr(txt.c_str());
#else
    std::cout << txt;
#endif
  }
  void
  Console::drawText(const std::string& txt, const Console::DrawStyle& style)
  {
#ifdef __unix__
    setStyle(style);
    addstr(txt.c_str());
#else
    std::cout << txt;
#endif
  }
  void
  Console::drawText(const std::string& txt, int x, int y)
  {
#ifdef __unix__
    mvaddstr(y, x, txt.c_str());
#else
    moveCursor(x,y);
    std::cout << txt;
#endif
  }
  void
  Console::drawText(const std::string& txt, int x, int y,
      const Console::DrawStyle& style)
  {
#ifdef __unix__
    setStyle(style);
    mvaddstr(y, x, txt.c_str());
#else
    moveCursor(x,y);
    setStyle(style);
    std::cout << txt;
#endif
  }
  void
  Console::drawText(const std::string& txt, int x, int y,
      const GridStyle& style)
  {
#ifdef __unix__
    setStyle(style);
    mvaddstr(y, x, txt.c_str());
#else
    moveCursor(x,y);
    setStyle(style);
    std::cout << txt;
#endif
  }

  /** +getDefaultStyle */
  Console::DrawStyle
  Console::getDefaultStyle()
  {
    return Console::_defaultStyle;
  }

  /** Get console Style */
  Console::DrawStyle
  Console::getCurrentStyle()
  {
    return Console::_currentStyle;
  }

  /** Show/Hide cursor */
  void
  Console::hideCursor()
  {
    if (Console::_currentCursorIsShow)
      {
        Console::_currentCursorIsShow = false;
#ifdef _WIN32
        /// TODO
#endif
#ifdef __unix__
        curs_set(0);
#endif
      }
  }
  void
  Console::showCursor()
  {
    if (!(Console::_currentCursorIsShow))
      {
        Console::_currentCursorIsShow = true;
#ifdef _WIN32
        /// TODO
#endif
#ifdef __unix__
        curs_set(1);
#endif
      }
  }

  /** +clear */
  void
  Console::clear()
  {
    /// TODO need to use API on windows and the right command on unix
    clear(0, 0, 0, 0);
  }

  /** +clear */
  void
  Console::clear(int x, int y, int w, int h)
  {
    /* Check the w,h */
    Size csSize = getSize();
    if ((w <= 0) || (w + x > csSize.width))
      w = csSize.width - x;
    if ((h <= 0) || (h + y > csSize.height + 1))
      h = csSize.height + 1 - y;
    if ((w < 0) || (h < 0))
      return;
    /* Prepare the string that will clear the screen */
    std::string strClear;
#ifdef _WIN32
    strClear.resize(w+1, ' ');
    strClear[strClear.length()-1] = '\n';
#else
    strClear.resize(w, ' ');
#endif

    /* Clear the screen */
    int maxH = y + h;
    for (; y <= maxH; y++)
      {
#ifdef _WIN32
        if (y == maxH) strClear.erase(strClear.length()-1);
#endif
        drawText(strClear, x, y);
      }
  }

  /** Set Style */
  void
  Console::setStyle(const Console::DrawStyle& style)
  {
    setColor(style.color, style.backColor);
    setBold(style.bold);
  }
  void
  Console::setStyle(const GridStyle& style)
  {
    setColor(style.color, style.backColor);
    setBold(style.bold);
  }

  /** Set console bold */
  void
  Console::setBold(bool bold)
  {
    if (Console::_currentStyle.bold == bold)
      return;
    Console::_currentStyle.bold = bold;
#ifdef _WIN32
    /// TODO
#endif
#ifdef __unix__
    if (bold)
      {
        attron(A_BOLD);
      }
    else
      {
        attroff(A_BOLD);
      }
#endif
  }
  void
  Console::setItalic(bool italic)
  {
    if (Console::_currentStyle.italic == italic)
      return;
    Console::_currentStyle.italic = italic;
#ifdef _WIN32
    /// TODO
#endif
#ifdef __unix__
    /// TODO
#endif
  }
  void
  Console::setUnderline(bool underline)
  {
    if (Console::_currentStyle.underline == underline)
      return;
    Console::_currentStyle.underline = underline;
#ifdef _WIN32
    /// TODO
#endif
#ifdef __unix__
    if (underline)
      {
        attron(A_UNDERLINE);
      }
    else
      {
        attroff(A_UNDERLINE);
      }
#endif
  }

  /** +colorConsoleRGB */
  void
  Console::setColor(int textColor, int textBackColor)
  {
    /* Check if color not already attribuate */
    if ((Console::_currentStyle.color == textColor)
        && (Console::_currentStyle.backColor == textBackColor))
      return;

    if (textColor == -1)
      textColor = _defaultStyle.color;
    if (textBackColor == -1)
      textBackColor = _defaultStyle.backColor;

    /* Get the R,G,B of the color */
    unsigned char rgbTxt[3] =
      { (unsigned char) (textColor),
          (unsigned char) ((unsigned short) (textColor) >> 8),
          (unsigned char) (textColor >> 16) };
    unsigned char rgbBack[3] =
      { (unsigned char) (textBackColor),
          (unsigned char) ((unsigned short) (textBackColor) >> 8),
          (unsigned char) (textBackColor >> 16) };

    /* Set the color */
#ifdef _WIN32

    /* Algo: [r,g,b] = color; // decompose color intor Red,Green,Blue
     *       ret (r>64) ? 4 : 0 + (g>64) ? 2 : 0 + (b>64) ? 1 : 0 */
#define LIBEC_CONSOLE_WIN_COLOR_INTERPOLATE_MACRO(c) \
            ((c[0]>64) ? 4 : 0)+((c[1]>64) ? 2 : 0)+((c[2]>64) ? 1 : 0)

#define LIBEC_CONSOLE_WIN_ADD_BRIGHT_MACRO(c) \
			(((c[0]<=128) && (c[1] <= 128) && (c[2] <= 128)) ? 0 : 8)

    SetConsoleTextAttribute(Console::handleConsole,
        /* BackColor */
        ((unsigned)(
                LIBEC_CONSOLE_WIN_COLOR_INTERPOLATE_MACRO(rgbBack) +
                LIBEC_CONSOLE_WIN_ADD_BRIGHT_MACRO(rgbBack)) << 4) |
        /*TextColor*/
        ((unsigned)(
                LIBEC_CONSOLE_WIN_COLOR_INTERPOLATE_MACRO(rgbTxt) +
                LIBEC_CONSOLE_WIN_ADD_BRIGHT_MACRO(rgbTxt)
            ))
    );

#endif
#ifdef __unix__

    /* Algo: [r,g,b] = color; // decompose color intor Red,Green,Blue
     *       ret (r>64) ? 1 : 0 + (g>64) ? 2 : 0 + (b>64) ? 3 : 0 */
#define LIBEC_CONSOLE_UNIX_COLOR_INTERPOLATE_MACRO(c) \
                ((c[0]>64) ? 1 : 0)+((c[1]>64) ? 2 : 0)+((c[2]>64) ? 4 : 0)

#define LIBEC_CONSOLE_UNIX_IS_BRIGHT_MACRO(c) \
            ((c[0]>128) && (c[1] > 128) && (c[2] > 128))

    /* Set the color */
    attron(
        COLOR_PAIR(ncurses.getPairColor( LIBEC_CONSOLE_UNIX_COLOR_INTERPOLATE_MACRO(rgbTxt), LIBEC_CONSOLE_UNIX_COLOR_INTERPOLATE_MACRO(rgbBack))));
#endif

    Console::_currentStyle.color = textColor;
    Console::_currentStyle.backColor = textBackColor;
  }

  /** +resetDefault */
  void
  Console::resetDefault()
  {
#ifdef _WIN32
    /* Reset the cursor visibility */
    if (Console::_defaultCursorIsShow) Console::showCursor();
    else Console::hideCursor();
    /* Reset the terminal style */
    Console::setStyle(Console::_defaultStyle);
#endif
  }

  /** +render */
  void
  Console::render()
  {
#ifdef __unix__
    refresh();
#endif
  }

}
