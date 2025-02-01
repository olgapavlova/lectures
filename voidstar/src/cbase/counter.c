#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cbase.h"
#include "iter.h"
#include "callback.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

static bool _is_in(counter * c, int * p);

// TODO Вообще эта функция нужна? Или хватит iter_foreach? 
void counter_foreach(counter *d, callback_t cb) {  // Применить функцию ко всем значениям счётчика
  D("Применить функцию ко всем значениям счётчика");
  iter_foreach(d, counter_next, cb);
}

void * counter_next(void * d) { // Выдать следующий элемент списка
  D("Выдать следующий элемент списка");
  static int * current = NULL;
  static void * list = NULL;

  if((list == NULL) || (list != d)) {  // • начать новый «список»
    list = d;
    current = d;
  }

  void * result = current;  // нельзя ++, потому что для NULL не определено

  if(_is_in(list, current)) {  // либо ++, либо сброс
    current++;
  } else {
    current = ITER_FINISHED;
  }

  return result;
}

bool _is_in(counter * c, int * p) {  // Проверить, указывает ли указатель внутрь счётчика
  D("Проверить, указывает ли указатель внутрь счётчика");
  bool result = (p == NULL) ? false : ((p >= (int *)(*c)) && (p < (int *)(*c) + 128));  // сравнивать не-NULL можно всегда
  return result;
}
