#include "config.h"

int main(int argc, char ** argv) {
  if (argc != 2) { abort(); }
  size_t mem_size = atoi(argv[1]);
  if (mem_size <= 0) { abort(); }

  char * mem = malloc(mem_size * sizeof(char));

  printf("Запросили:\t%ld\n", mem_size);
  printf("Получили:\t%ld\n", malloc_usable_size(mem));

  size_t * header = (size_t *)mem - 2;

  printf("Начало заголовка:\t%p\n", header);
  printf("Начало блока памяти:\t%p\n", mem);

  void * prev_addr = (void*)(header[0]);
  size_t raw_size = header[1];
  size_t pure_size = raw_size & ~(0x7);

  printf("Адрес предыдущего блока:\t%p\n", prev_addr);
  printf("Сырая память:\t\t%ld\n", raw_size);
  printf("Собственно память:\t%ld\n", pure_size);

  return EXIT_SUCCESS;
}
