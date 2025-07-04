#include <stdio.h>
#include <stdlib.h>
#include "parabola.h"

int main() {

  explain(7, 19)
  cx x = {.r = 4, .i = 3};
  cx y = {.r = 5, .i = -3};
  explain(x, y)

  return 0;
}

void explain_int_int(int x, int y) {
    printf("Int %d, Int %d\n", x, y);
}

void explain_int_cx(int x, cx y) {
    printf("Int %d, Complex %d + i * %d\n", x, y.r, y.i);
}

void explain_cx_int(cx x, int y) {
    printf("Complex %d + i * %d, Int %d\n", x.r, x.i, y);
}

void explain_cx_cx(cx x, cx y) {
    printf("Complex %d + i * %d, Complex %d + i * %d\n", x.r, x.i, y.r, y.i);
}
