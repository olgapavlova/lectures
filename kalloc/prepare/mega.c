#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define PAGE_Q 32

int main(void) {
  void * heap_before = sbrk(0);
  void * reserve = malloc(PAGE_Q * PAGE_SIZE);
  if(reserve == NULL) exit(EXIT_FAILURE);
  void * heap_after = sbrk(0);

  printf("Границы кучи до:\t%p\n", heap_before);
  printf("Граница кучи после:\t%p\n", heap_after);
  printf("Разница:\t\t\033[33m0x%012x\033[0m\n", (int)(heap_after - heap_before));
  printf("Выделенная область:\t\033[45m%p\033\[0m\n", reserve);

  free(reserve); 
}
