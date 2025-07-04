#ifndef CYPHER_H_LOADED
#define CYPHER_H_LOADED

#define MAX_GUESS 10

#include <stdlib.h>
#include <time.h>

int dbl(int n) { return n * 2; };
int half(int n) { return n / 2; };
int sqr(int n) { return n * n; };
int minus(int n) { return --n; };
int plus(int n) { return ++n; };

typedef int (*operation)(int);
int (*o[5])(int) = {dbl, half, sqr, minus, plus};

int rand_o() {
  return rand() % 5; // TODO Fix magic number usage
}

int char_used(char);
void make_step(operation);
void win();
void cont(int);
void step_not_allowed(char);
void real_step(operation);
void begin();

void fake_step(operation);
void skip();

#endif
