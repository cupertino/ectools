///////////////////////////////////////////////////////////////////////////////
/// @file		Console.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Multi platform class for terminal manipulation
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_TOOLS_CONSOLE_H__
#define LIBEC_TOOLS_CONSOLE_H__

#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#endif

#ifdef __unix__
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#endif

#include "Tools.h"
#include "Color.h"

namespace cea
{
  /* Prototype */
  struct GridStyle;

  /// @brief Multi platform class for terminal manipulation
  class Console
  {
  private:

#ifdef __unix__
    /// @brief This class is used to automatically initialize
    ///        and stop Ncurses for Unix implementation
    class NcursesLoader
    {
    public:

      /// @brief Ncurses color pair table
      static short ncurseColorPair[8][8];
      /// @brief Count of Ncurses color pair loaded
      static short ncurseColorPairLoaded;

      static WINDOW* win;

      /// @brief Initialize Ncurses
      NcursesLoader()
      {
        win = initscr();
        cbreak();
        nodelay(stdscr, TRUE);
        noecho();
        start_color();
        keypad(stdscr, TRUE);
        /* Init all the table */
        for (int i = 0; i < 8; i++)
          {
            for (int j = 0; j < 8; j++)
              ncurseColorPair[i][j] = 0;
          }
        /* Init the first and default Table */
        ncurseColorPairLoaded = 1;
        ncurseColorPair[7][0] = 1;
        init_pair(1, 7, 0);
      }

      /// @brief Stop Ncurses
      ~NcursesLoader()
      {
        if (win)
          delwin(win);

        endwin();
      }

      /// @brief Get a valid Ncurses color pair of text color
      ///        and background color
      /// @param txt Text color id
      /// @param back Background color id
      int
      getPairColor(short txt, short back)
      {
        /* Init color */
        if ((txt < 0) || (txt > 7))
          txt = 7;
        if ((back < 0) || (back > 7))
          back = 0;
        /* Create the color pair if not exist */
        if (ncurseColorPair[txt][back] == 0)
          {
            ncurseColorPairLoaded++;

            /// @brief Macro to get corrsponding Ncurses color value
#define LIBEC_MACRO_NCURSES_SETCOLOR(c,v) \
                        switch(c) { \
                        case 0: v = COLOR_BLACK; break; \
                        case 1: v = COLOR_RED; break; \
                        case 2: v = COLOR_GREEN; break; \
                        case 3: v = COLOR_YELLOW; break; \
                        case 4: v = COLOR_BLUE; break; \
                        case 5: v = COLOR_MAGENTA; break; \
                        case 6: v = COLOR_CYAN; break; \
                        case 7: v = COLOR_WHITE; break; \
                        }

            short c1, c2;
            LIBEC_MACRO_NCURSES_SETCOLOR(txt, c1);
            LIBEC_MACRO_NCURSES_SETCOLOR(back, c2);

            ncurseColorPair[txt][back] = ncurseColorPairLoaded;
            init_pair(ncurseColorPairLoaded, c1, c2);
          }
        /* Return the color pair */
        return ncurseColorPair[txt][back];
      }

    };

    /// @brief Ncurses container for Unix implementation
    static NcursesLoader ncurses;
#endif

  public:

    /* Key contants */
#ifdef _WIN32
    /// @brief Key Up for Windows platform
    static const int K_UP = 'i';
    /// @brief Key Down for Windows platform
    static const int K_DOWN = 'k';
    /// @brief Key Left for Windows platform
    static const int K_LEFT = 'j';
    /// @brief Key Right for Windows platform
    static const int K_RIGHT = 'l';
#endif
#ifdef __unix__
    /// @brief Key Up for Unix platform
    static const int K_UP = KEY_UP;
    /// @brief Key Down for Unix platform
    static const int K_DOWN = KEY_DOWN;
    /// @brief Key Left for Unix platform
    static const int K_LEFT = KEY_LEFT;
    /// @brief Key Right for Unix platform
    static const int K_RIGHT = KEY_RIGHT;
#endif

    /* Structure */
    /// @brief Structure used for getSize of the console
    struct Size
    {
      int width; ///< Width size
      int height; ///< Height size
    };
    /// @brief Structure used for getCursor position of the console
    struct Point
    {
      int x; ///< X-coordinate
      int y; ///< Y-coordinate
    };

    /// @brief Console style structure used by drawText
    struct DrawStyle
    {
      /* Default Constructor */
      /// @brief Create a console Style
      /// @param color Text color
      /// @param backColor Background text color
      /// @param bold Bold status
      /// @param italic Italic status
      /// @param underline Underline status
      DrawStyle(int color = Color::White, int backColor = Color::Black,
          bool bold = false, bool italic = false, bool underline = false) :
          color(color), backColor(backColor), bold(bold), italic(italic), underline(
              underline)
      {
      }

      int color; ///< Text color
      int backColor; ///< Text background color
      bool bold; ///< Bold status
      bool italic; ///< Italic status
      bool underline; ///< Underline status
    };

    /* Input/Infos Functions */
    /// @brief Get async terminal key hit
    /// @return the key hit code or 0 if no key hit
    static int
    keyHit();

    /// @brief Move the cursor of the console
    /// @param x X-coordinate
    /// @param y Y-coordinate
    static void
    moveCursor(int x, int y);

    /// @brief Get current console size
    /// @return Current console size
    static Size
    getSize();

    /// @brief Get current cursor position
    /// @return Current cursor position
    static Point
    getCursorPosition();

    /// @brief Show console cursor
    static void
    hideCursor();
    /// @brief Hide console cursor
    static void
    showCursor();

    /// @brief Reset default properties of the console
    static void
    resetDefault();

    /* Draw settings functions */
    /// @brief Set console colors
    /// @param textColor Text color
    /// @param textBackColor Text background color
    static void
    setColor(int textColor, int textBackColor = 0);

    /// @brief Set console bold status
    /// @param bold Bold status
    static void
    setBold(bool bold = true);
    /// @brief Set console italic status
    /// @param italic Italic status
    static void
    setItalic(bool italic = true);
    /// @brief Set console underline status
    /// @param underline Underline status
    static void
    setUnderline(bool underline = true);

    /// @brief Set console style
    /// @param style Style to set
    static void
    setStyle(const DrawStyle& style);
    /// @brief Set console GridStyle
    /// @param style GridStyle to set
    static void
    setStyle(const GridStyle& style);

    /// @brief Get default console style
    /// @return Return default console style
    static DrawStyle
    getDefaultStyle();

    /// @brief Get current console style
    /// @return Current console style
    static DrawStyle
    getCurrentStyle();

    /* Draw functions */
    /// @brief Draw a text to the current position with the current console
    ///        style
    /// @param txt Text to draw
    static void
    drawText(const std::string& txt);
    /// @brief Draw a stylized text to the current position
    /// @param txt Text to draw
    /// @param style Console style to use
    static void
    drawText(const std::string& txt, const DrawStyle& style);
    /// @brief Draw a text to a given position with the current
    ///        console style
    /// @param txt Text to draw
    /// @param x X-coordinate of the text
    /// @param y Y-coordinate of the text
    static void
    drawText(const std::string& txt, int x, int y);
    /// @brief Draw a stylized text to a given position
    /// @param txt Text to draw
    /// @param x X-coordinate of the text
    /// @param y Y-coordinate of the text
    /// @param style Console style to use
    static void
    drawText(const std::string& txt, int x, int y, const DrawStyle& style);
    /// @brief Draw a stylized text to a given position
    /// @param txt Text to draw
    /// @param x X-coordinate of the text
    /// @param y Y-coordinate of the text
    /// @param style Grid style to use
    static void
    drawText(const std::string& txt, int x, int y, const GridStyle& style);

    /// @brief Clear entire screen
    static void
    clear();
    /// @brief Clear a part of the screen
    /// @param x Left coordinate of the box to clear
    /// @param y Top coordinate of the box to clear
    /// @param w Width of the box to clear.
    ///          If < 0 Then width = Console::width
    /// @param h Height of the box to clear.
    ///          If < 0 The height = Console::height
    static void
    clear(int x, int y, int w = 0, int h = 0);

    /// @brief Render to the console the different draw done
    static void
    render();

  private:

    /* Private Constructor/Destructor */
    /// @brief Store default console properties
    ///
    /// This constructor is private to avoid instanciation
    /// of the class.
    ///
    Console();
    /// @brief Reset default console properties
    ~Console();

    /* Members */
    /// @brief Create a unique instance of Console to store the default
    ///        console properties and initializes all the requirements.
    static Console _console;

#ifdef _WIN32
    /// @brief Windows Handle of the console
    static HANDLE handleConsole;
#endif

    /// @brief Default style of the console
    static DrawStyle _defaultStyle;
    /// @brief Current style of the console
    static DrawStyle _currentStyle;

    /// @brief Default cursor visibility status
    static bool _defaultCursorIsShow;
    /// @brief Current cursor visibility status
    static bool _currentCursorIsShow;

    /// @brief Current cursor position
    static Point _currentCursorPos;

  };

}

#endif

/** This class use ncurse on Linux and default console on windows */

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Console
///	@ingroup tools
///
/// This class use the Ncurses library for Unix platform and default console 
/// API for Windows platform.
///
///////////////////////////////////////////////////////////////////////////////
