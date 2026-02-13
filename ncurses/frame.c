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

  // новое окно
  WINDOW * w = newwin(5, 13, 5, 5);
  if(!w) perror("newwin();");

  // рамочку сделаем
  box(w, 0, 0);

  // вывод почти как уже делали
  wmove(w, 2, 2);
  waddstr(w, "Привет ;)");
  wrefresh(w);
  wgetch(w);

  // новое окно
  WINDOW * u = newwin(7, 11, 15, 30);
  if(!w) perror("newwin();");

  // рамочку сделаем
  wborder(u, 186,0x000000ba,0x000000cd,0x000000cd, 0x000000c9,0x000000bb,0x000000c8,0x000000bc);

  // вывод почти как уже делали
  wmove(u, 3, 2);
  waddstr(u, "Пока ;)");
  wrefresh(u);
  wgetch(u);

  wmove(w, 2, 2);
  waddstr(w, "Как дела?");
  wgetch(w);

  wmove(u, 3, 2);
  waddstr(u, "Нормуль");
  wgetch(u);

  // заканчиваем
  delwin(w);
  delwin(u);
  int r_end = endwin();
	return (r_end == ERR);
}
