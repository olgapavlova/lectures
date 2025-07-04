/// Радуга • Основной код программы
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "color.h"
#include "log.h"

int main(int argc, char** argv) {
  LI("Программа начала работать.");
  // + Напечатать строку в случайной расцветке
  // + Один символ — один случайный выбор цвета фона и текста
  // - Если строка не введена — выбирать случайно одну из предустановленных
  // строк
  // + Возвращать в исходное положение

  char line[100] = "Jump around the dog";

  srand(time(NULL));

  for (int i = 0; line[i] != '\0'; i++) {
    char* txt = color_symbol(random_txt());
    char* bg = color_symbol(random_bg());
    printf("%s", colored_char(txt, bg, line[i]));
    free(txt);
    free(bg);
  }

  printf(colored_char(color_symbol(DEFAULT_TXT_COLOR),
                      color_symbol(DEFAULT_BG_COLOR), '\n'));

  LI("Программа закончила работать.");
  return EXIT_SUCCESS;
}
