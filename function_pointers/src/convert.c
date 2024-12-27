#include "convert.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef CONVERT_STANDALONE
// Используем только для тестирования
int main() {

  printf("%d\n", type(45282));

  char *a = malloc(100);
  convert(&a, 72432);
  printf("%s\n", a);

  convert(&a, (char)'m');
  printf("%s\n", a);

  convert(&a, 19.3424);
  printf("%s\n", a);

  convert_error(ERR_CONVERT_TYPE);

  return EXIT_SUCCESS;
}
#endif

void _convert_int(char **a, int n) { sprintf(*a, "%d", n); }

void _convert_char(char **a, char c) { sprintf(*a, "%c", c); }

void _convert_double(char **a, double x) { sprintf(*a, "%.2f", x); }

// Печать кода ошибки
void convert_error(int error) {
  int (*e)(const char *restrict, ...);
  e = printf;
  e("%d\n", error);
}
