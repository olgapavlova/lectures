#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CYCLE 10

int main(void) {
  void * heap_before = sbrk(0);
  void * reserve;
  printf("Граница кучи до:\t%p\n", heap_before);
  for(int i = 0; i < CYCLE; i++) {
    void * reserve = malloc(1);
    if(reserve == NULL) exit(EXIT_FAILURE);
    printf("Выделенная область:\t%p\n", reserve);
    printf("Ой, что это:\t\t\033[31m0x%012x\033[0m\n", (int)(reserve - heap_before));

    void * heap_after = sbrk(0);
    printf("Граница кучи после:\t%p\n", heap_after);
    printf("Разница:\t\t\033[33m0x%012d\033[0m\n", (int)(heap_after - heap_before));
    printf("\n");
  }
}

