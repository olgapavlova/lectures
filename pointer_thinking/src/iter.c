#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO Вынести объявления в отдельный .h-файл

typedef struct metastr {
  int length;
  char * data;
} metastr;

typedef void (*cb)(char);  // тип функции для обратных вызовов при обработке метастрок

void run_callback(char);
void run_another_callback(char);

void iterator_char(metastr *, cb);  // итератор по метастроке

metastr * metastr_create(char *);

int main() {

  // TODO Создание строки было отдельной функцией
  char * c = malloc(sizeof(char) * 10);
  strcpy(c, "abcdefghi");

  // c.data -- УКАЗЫВАЕТ на abcdefghi, но не СОДЕРЖИТ их

  // TODO Создание метастроки было отдельной функцией
  metastr * mmm = metastr_create(c);

  iterator_char(mmm, run_callback);
  iterator_char(mmm, run_another_callback);

  free(mmm);  // TODO Как бы нам сделать, чтобы оно само?
  free(c);
  return 0;
}

void run_callback(char h) {
    printf("%c\n", h);
}

void run_another_callback(char h) {
    printf("%d -- %c\n", h, h);
}

/// Итератор по метастроке (точнее, по её символам)
void iterator_char(metastr * mts, cb rcb) {
  for(int i = 0; i < mts->length; i++) {
    rcb(mts->data[i]);
  }
}

/// Создать метастроку из обычной
metastr * metastr_create(char * c) {
  metastr * result = malloc(sizeof(metastr));
  *result = (metastr){.length = strlen(c), .data=c };
  // TODO …и вот тут бы нам запланировать её очистку!
  return result;
}
