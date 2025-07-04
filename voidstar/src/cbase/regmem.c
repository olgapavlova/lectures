#include "regmem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "iter.h"
#include "callback.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

/// Реестр динамически выделенной памяти
static struct {
  int length;    /**< длина реестра */
  void ** data;  /**< данные — массив void*-указателей */
} _regmem = {0, NULL};

static void _destroy_item(int n, void **pp) {  // Очистить память под указателем
  D("Очистить память под указателем");
  if(n != 1) { return; }  // по уму нужна отдельная обёртка для callback
  free(*pp);
  *pp = NULL;  // ради этого двойной указатель
}

static void * _next() {  // Следующий элемент реестра
  D("Следующий элемент реестра");
  static void * current = NULL;
  if(current == NULL) { current = _regmem.data; }
  void * result = current++;
  if(current == _regmem.data[_regmem.length]) { current = ITER_FINISHED; }
  return result;
}

void regmem_add(void *p) {  // Добавить указатель в реестр
  D("Добавить указатель в реестр");
  _regmem.length++;
  _regmem.data = realloc(_regmem.data, (_regmem.length) * sizeof(void *));
  _regmem.data[_regmem.length - 1] = p;
}

void regmem_replace(void * to, void * from) {  // Заменить один указатель на другой (нужно при realloc)
  D("Заменить один указатель на другой (нужно при realloc)");
  void ** p = _regmem.data;
  int counter = 0;
  while((*p != from) && (counter < _regmem.length)) {p++; counter++;}
  LT(%s %d, "Позиция указателя:", counter);
  if(counter == _regmem.length) {
    LEs("Указатель не найден в реестре");
    exit(EXIT_FAILURE);
  } else {
    LTs("Заменяем значение указателя в реестре");
    _regmem.data[counter] = to;
    LTs("Заменили");
  }
}

void regmem_destroy() {  // Освободить всю память
  D("Освободить всю память");
  iter_foreach(_regmem.data, _next, _destroy_item);
  free(_regmem.data);
  _regmem.data = NULL;
}

void regmem_realloc(void ** v, int size_new, int size_old) {  // Изменить размер памяти по указателю
  D("Изменить размер памяти по указателю");
  void * tmp = malloc(size_new);
  if(tmp == NULL) { LEs("Не удалось выделить память"); exit(EXIT_FAILURE); }
  else {
    int cpy_size = (size_old > size_new) ? size_new : size_old;
    memcpy(tmp, *v, cpy_size);
    regmem_replace(tmp, *v);
    LTs("Продолжаем процедуру изменения размера памяти");
    free(*v);
    LTs("Освободили старую память");
    *v = tmp;
    LTs("Присвоили указателю новое значение");
  }
}
