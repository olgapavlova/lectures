#include <curses.h>
#include <locale.h>

int main(void) {

  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);

  box(stdscr, 0, 0);

  move(10, 5);
  printw("Привет!");

  getch();
  getch();

  // тут использование NCurses

  endwin();
  return 0;
}
