#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbase.h"
#include "iter.h"
#include "callback.h"
#include "dict.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

void * dict_next(void * d) { // Выдать указатель на следующий элемент словаря 
  D("Выдать указатель на следующий элемент словаря");
  static int current = -1;
  static dict * list = NULL;

  if((list == NULL) || (list != d)) { list = d; current = 0; }

  void * result = NULL;
  if(current == 128) {
    list = NULL;
    current = -1;
    result = ITER_FINISHED;
  } else {
    result = &((*list)[current++]);
  }
  return result;
}

void dict_foreach(dict * d, callback_t cb) {
 iter_foreach(d, dict_next, cb); 
}
