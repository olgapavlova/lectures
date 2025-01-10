#define _GNU_SOURCE

#include "errlib.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void a();
int b(int);

int main() {
  for (int i = 0; i < 106; ++i) {
    printf("%d\t%s\n", i, strerror(i));
  }

  FILE * f __attribute__ ((unused)) = fopen("blabla", "r");
  perror(NULL);

  atexit(a);

  char * c __attribute__ ((unused)) = malloc(-1);
    printf("malloc: %d\t%s\n", errno, strerror(errno));

  printf("%s\n", strerror_ru[0]);
  printf("%s\n", strerror_ru[1]);
  printf("%s\n", getprogname());

    int ok = 1;
    printf("ok = %d\n", ok);
    ok = ok && !b(0);
    printf("ok = %d\n", ok);
    ok = ok && !b(1);
    printf("ok = %d\n", ok);
    ok = ok && !b(0);
    printf("ok = %d\n", ok);

  return EXIT_SUCCESS;
}

void a() {
    printf("Вот я запускаюсь перед выходом\n");
}

int b(int n) {
  int result = (n == 0) - 1;
  printf("result = %d; n = %d\n\n", result, n);
    return result;
}
