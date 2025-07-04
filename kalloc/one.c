#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UNIT_SIZE 1024

int main(void) {
  void * heap_before = sbrk(0);
  void * reserv = malloc(UNIT_SIZE);
  void * heap_after = sbrk(0);

  printf("Верх кучи до:\t\t\t%p\n", heap_before);
  printf("Адрес выделенного куска:\t%p\n", reserv);
  printf("Верх кучи после:\t\t%p\n", heap_after);
  printf("Дельта:\t\t\t\t0x%012x\n", (int)(heap_after - heap_before));

  return 0;
}
