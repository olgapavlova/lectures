/// Цвет • Основной код библиотеки
#include "color.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

#ifdef COLOR_STANDALONE_COMPILE
int main() { return 0; }
#endif

int random_zero_or_one() { return rand() % 2; }

int random_pure_color_code() { return rand() % MAX_COLOR; }

int random_full_color_code() {
  int result = random_pure_color_code() +
               random_zero_or_one() * LIGHT_CORRECTION + BASE_CODE;
  return result;
}

int random_bg() {
  LI("Выбираем случайный цвет фона")
  int result = random_full_color_code() + BG_CORRECTION;
  return result;
}

int random_txt() {
  LI("Выбираем случайный цвет текста")
  int result = random_full_color_code();
  return result;
}

char* color_symbol(int c) {
  LI("Создаём символ для печати цвета.");
  LW("Внимание! Выделена динамическая память! Не забыть почистить!");
  char* result = malloc(sizeof(char) * (SYM_LEN + 1));
  sprintf(result, COLOR_SYM, c);
  return result;
}

char* colored_char(char* txt, char* bg, char c) {
  char* result = malloc(sizeof(char) * (SYM_LEN * 2 + 1 + 1));
  sprintf(result, "%s%s %c ", txt, bg, c);
  return result;
}
