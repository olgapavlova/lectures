#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  void * heap_before = sbrk(0);
  void * reserve = malloc(1);
  if(reserve == NULL) exit(EXIT_FAILURE);
  void * heap_after = sbrk(0);

  printf("Границы кучи до:\t%p\n", heap_before);
  printf("Выделенная область:\t%p\n", reserve);
  printf("Ой, что это:\t\t\033[31m0x%012x\033[0m\n", (int)(reserve - heap_before));
  printf("Граница кучи после:\t%p\n", heap_after);
  printf("Разница:\t\t\033[33m0x%012d\033[0m\n", (int)(heap_after - heap_before));

  free(reserve);  // просто чтобы руку не сбивать
}
