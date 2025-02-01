#include <stdio.h>
#include <stdlib.h>
#include "cbase.h"
#include "callback.h"
#include "iter.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

void iter_foreach(void * list, iter_next_t next, callback_t cb) { // Применить функцию к каждому элементу «списка»
  D("Применить функцию к каждому элементу «списка»");
  void * p = NULL;
  while((p = next(list)) != ITER_FINISHED) {
    callback_exec(cb, 1, p);    
  }
}

void iter_foreach_buffer(void * list, iter_next_buffer_t next, void * b, callback_t cb) { // Применить функцию к каждому элементу, используя буфер
  D("Применить функцию к каждому элементу, используя буфер");
  void * p = NULL;
  while((p = next(list, b)) != ITER_FINISHED) {
    callback_exec(cb, 1, p);    
  }
}
 
