#include <stdio.h>
#include <stdlib.h>
#include "hello.h"

int main() {
    print("Привет!")
    print(2025)
  return 0;
}

void print_int(int n) {
    printf("Целое число %d\n", n);
}

void print_string(char * s) {
    printf("Строка %s\n", s);
}
