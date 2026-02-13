#include <curses.h>
#include <locale.h>

int main(void) {

	// хотим кириллицу 
	setlocale(LC_ALL, "");

	// иногда это даже терминал, а не экран или окно
	initscr();

  // не нужно, чтобы вводимые символы отображались
  noecho();

  // курсор тоже на экране не нужен
  curs_set(0);

  // нужны цвета
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_WHITE);
  init_pair(2, COLOR_CYAN, COLOR_YELLOW);
  init_pair(3, COLOR_WHITE, COLOR_GREEN);
  init_pair(4, COLOR_WHITE, COLOR_RED);

  // новое окно
  WINDOW * w = newwin(5, 13, 3, 5);
  if(!w) perror("newwin();");

  // цвета окна
  wbkgd(w, COLOR_PAIR(1));

  // рамочку сделаем
  box(w, 0, 0);

  // вывод почти как уже делали
  wmove(w, 2, 2);
  waddstr(w, "Привет ;)");
  wrefresh(w);
  wgetch(w);

  // новое окно
  WINDOW * u = newwin(7, 11, 8, 18);
  if(!w) perror("newwin();");

  // цвета окна
  wbkgd(u, COLOR_PAIR(2));

  // рамочку сделаем
  box(u, 0, 0);

  // вывод почти как уже делали
  wmove(u, 3, 2);
  waddstr(u, "Пока ;)");
  wrefresh(u);
  wgetch(u);

  // диалог
  wmove(w, 2, 2);
  wbkgd(w, COLOR_PAIR(3));
  waddstr(w, "Как дела?");
  wgetch(w);

  wmove(u, 3, 2);
  waddstr(u, "Нормуль");
  wgetch(u);

  wbkgd(u, COLOR_PAIR(4));
  wgetch(u);

  // заканчиваем
  delwin(w);
  delwin(u);
  int r_end = endwin();
	return (r_end == ERR);
}
