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
  WINDOW * w = newwin(5, 13, 2, 3);
  if(!w) perror("newwin();");

  box(w, 0, 0);
  wrefresh(w);
  wgetch(w);

  // рамочку сделаем
  cchar_t vert_char1, horiz_char1,
        tl_corner1, tr_corner1,
        bl_corner1, br_corner1;

  setcchar(&vert_char1, L"│", WA_NORMAL, 0, NULL);
  setcchar(&horiz_char1, L"─", WA_NORMAL, 0, NULL);
  setcchar(&tl_corner1, L"╭", WA_NORMAL, 0, NULL);
  setcchar(&tr_corner1, L"╮", WA_NORMAL, 0, NULL);
  setcchar(&bl_corner1, L"╰", WA_NORMAL, 0, NULL);
  setcchar(&br_corner1, L"╯", WA_NORMAL, 0, NULL);

  wborder_set(w,
           &vert_char1, &vert_char1,     // left, right
           &horiz_char1, &horiz_char1,   // top, bottom
           &tl_corner1, &tr_corner1,     // top-left, top-right
           &bl_corner1, &br_corner1);    // bottom-left, bottom-right

  // вывод почти как уже делали
  wmove(w, 2, 2);
  waddstr(w, "Привет ;)");
  wrefresh(w);
  wgetch(w);

  // новое окно
  WINDOW * u = newwin(7, 11, 7, 15);
  if(!w) perror("newwin();");

  // рамочку сделаем — теперь двойную
  cchar_t vert_char, horiz_char,
        tl_corner, tr_corner,
        bl_corner, br_corner;

  setcchar(&vert_char, L"║", WA_NORMAL, 0, NULL);
  setcchar(&horiz_char, L"═", WA_NORMAL, 0, NULL);
  setcchar(&tl_corner, L"╔", WA_NORMAL, 0, NULL);
  setcchar(&tr_corner, L"╗", WA_NORMAL, 0, NULL);
  setcchar(&bl_corner, L"╚", WA_NORMAL, 0, NULL);
  setcchar(&br_corner, L"╝", WA_NORMAL, 0, NULL);

  wborder_set(u,
           &vert_char, &vert_char,     // left, right
           &horiz_char, &horiz_char,   // top, bottom
           &tl_corner, &tr_corner,     // top-left, top-right
           &bl_corner, &br_corner);    // bottom-left, bottom-right

  // вывод почти как уже делали
  wmove(u, 3, 2);
  waddstr(u, "Пока ;)");
  wrefresh(u);
  wgetch(u);

  wmove(w, 2, 2);
  waddstr(w, "Чё как?");
  wgetch(w);

  wmove(u, 3, 2);
  waddstr(u, "Норм");
  wgetch(u);

  // заканчиваем
  delwin(w);
  delwin(u);
  int r_end = endwin();
	return (r_end == ERR);
}
