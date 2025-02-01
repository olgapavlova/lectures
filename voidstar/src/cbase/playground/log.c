#include <stdio.h>
#include <stdlib.h>
#include "../log.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_DEBUG

void one();
void two();

int main() {
  D("Основная функция программы");
  L(LOG_LVL_ERROR, %s, "blabla");
  Lt(qqqq);
  Ld(12345);
  Ld(734832);
  Ld(__APPLE__);
  Ld(__APPLE_CC__);
  LF(%d %s, LOG_LVL, "Fatal error");
  LFi(7+5-131);
  LFs("7+5-131");
  char c[20] = "blabla wow";
  int d = 175;
  Li(d)
  LE(%d, LOG_LVL);
  LW(%d, LOG_LVL);
  LI(%d, LOG_LVL);
  LD(%d, LOG_LVL);
  LT(%d, LOG_LVL);

  one();
    return 0;
}

void one() {
  D("Первый уровень вложенности");
  two();
}

void two() {
  D("Второй уровень вложенности");
}
