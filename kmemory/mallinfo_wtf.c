#include <malloc.h>
#include <stdio.h>

int main(void) {

  int * a = malloc(sizeof(int)*2);

  struct mallinfo2 mi = mallinfo2();

  printf("Вся память кучи: %ld\n", mi.arena);
  printf("Обычная свободная память: %ld\n", mi.ordblks);
  printf("Свободная fastbin-память: %ld\n", mi.smblks);
  printf("Ушло на mmap(): %ld\n", mi.hblks);
  printf("То же в байтах: %ld\n", mi.hblkhd);
  printf("Не использовано (всегда ноль): %ld\n", mi.usmblks);
  printf("Всего байтов в свободном fasbin: %ld\n", mi.fsmblks);
  printf("Всего занятых байт?: %ld\n", mi.uordblks);
  printf("Всего свободных байт?: %ld\n", mi.fordblks);
  printf("Сколько всего в идеале можно освободить: %ld\n", mi.keepcost);

  return 0;
}
