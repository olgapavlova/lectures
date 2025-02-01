#include <stdio.h>
#include <stdlib.h>

void * create_str(void *);

static void ** _REGMEM = NULL;

void free_regmem();

int main() {
  atexit(free_regmem);

  _REGMEM = malloc(sizeof(void *) * 3);
  _REGMEM[2] = NULL;

  char letter_1 = 'w';
  char letter_2 = 'g';

  char * w3 = (char *)create_str(&letter_1);
  void * g3 = create_str(&letter_2);

  // _REGMEM полностью заполнен

  printf("Знаем тип: %s\n", w3);
  printf("Не знаем тип: %s\n", (char *)g3);

  // неявный вызов функции exit()
  return EXIT_SUCCESS;
}

void free_regmem() {
  void ** p = _REGMEM;
  while(*p != NULL) {
    free(*p);
    *p = NULL;
  }
}

void add_to_regmem(void * vv) {
  static int _next = 0;  
  _REGMEM[_next++] = vv;
}


void * create_str(void * cv) {
  char c = *(char *)cv;  // 1

  char * result = malloc(sizeof(char) * 4);
  add_to_regmem(result);

  result[0] = result[1] = result[2] = c;
  result[3] = '\0';

  return (void*)result;  // 3
}



