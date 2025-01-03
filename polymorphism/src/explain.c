#include <stdio.h>
#include <stdlib.h>
#include "explain.h"

int main() {

  // x, y — целые или комплексные
  explain(1, 1)

    cx x = {.d = 4, .i = -5};
    cx y = {.d = 2, .i = 16};

  explain(x, y)
  explain(y, x)
  explain(x, 19)
  explain(23, y)

  return 0;
}

void ex_int_int(int x, int y) {
    printf("(x - %d) • (x - %d)\n", x, y);
}

void ex_int_cx(int x, cx y) {
    printf("(x - %d) • (x - %d - %d • i)\n", x, y.d, y.i);
}

void ex_cx_int(cx x, int y) {
    printf("(x - %d - %d • i) • (x - %d)\n", x.d, x.i, y);
}

void ex_cx_cx(cx x, cx y) {
    printf("(x - %d - %d • i) • (x - %d - %d • i)\n", x.d, x.i, y.d, y.i);
}
