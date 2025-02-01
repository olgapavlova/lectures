#include <stdio.h>
#include <stdlib.h>
#include "../cbase.h"
#include "../iter.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

void f(int n, void ** d) {
  D("Тестовая функция");
  char * c = (char *)d[n - 1];
  printf("%d — %c\n", *c, *c);
}

int main() {
  D("Основная функция программы");
  counter a = {64, 89, 45, 36};
  counter_foreach(&a, f);


    return EXIT_SUCCESS;
}
 
