#include <ncurses.h>
#include <iostream>

int
main()
{
  WINDOW* win = initscr(); // Start curses mode
  if (win)
    {
      std::cout << "deleting window" << std::endl;
      delwin(win);

      endwin(); // End curses mode
    }
  else
    std::cout << "error" << std::endl;

  return 0;
}
