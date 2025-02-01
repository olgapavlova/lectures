#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "log.h"

void buffer_init(buffer * b) {  // Инициализировать буфер
  D("Инициализировать буфер");
  for(int i = 0; i < BUFFER_SIZE; i++) { (*b)[i] = '\0'; }
}
