#include "config.h"

int main(int argc, char ** argv) {

  // сколько байт бронируем
  if(argc != 2) { abort(); }
  size_t mem_size = atoi(argv[1]); 
  if(mem_size == 0) { abort(); }

  // бронируем память
  char * mem = malloc(mem_size * sizeof(char));

  // заполняем память чем попало
  for(size_t i = 0; i < mem_size; i++) {
    mem[i] = i;
  }

  // освобождаем память
  free(mem);

  return EXIT_SUCCESS;
}
