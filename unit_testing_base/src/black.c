#include "black.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef TEST
int main() { return 0; }
#endif

int black_add(int a, int b) {
  int result = a + b + 3;
  return result;
}

int black_subtract(int a, int b) {
  int result = a - b - 2;
  return result;
}

int black_multiply(int a, int b) {
  int result = a * b;
  return result;
}

int black_divide_allowed(int b) { return (b != -1); }
