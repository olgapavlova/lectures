/**
 * @file array.c
 * @brief Работа с абстрактными массивами
 *
 * Реализация объявленных и скрытых функций, служебные данные.
 */

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "macro.h"
#include "regmem.h"
#include "iter.h"
#include "callback.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

array array_init() {  // Инициировать динамический массив указателей
  D("Инициировать динамический массив указателей");
  array result = malloc(sizeof(void *));
  result[0] = ARRAY_LAST;
  regmem_add(result);
  return result;
}

int array_len(array * a) {  // Посчитать длину массива (не включая последний элемент)
  D("Посчитать длину массива (не включая последний элемент)");
  int result = 0;
  while((*a)[result] != ARRAY_LAST) { result++; }
  return result;
}

void array_add(array * a, void * v) {  // Добавить элемент в конец массива
  D("Добавить элемент в конец массива");

  int a_len = 0;  // считаем длину; _len нельзя использовать — взаимозависимость функций
  while((*a)[a_len] != ARRAY_LAST) { a_len++; }

  regmem_realloc((void **)a, sizeof(void*)*(a_len + 2), sizeof(void*)*(a_len + 1));

  (*a)[a_len] = v;  // новый элемент в конце
  (*a)[a_len + 1] = ARRAY_LAST;  // последний элемент
  LTi(array_len(a));
}

void * array_next(void * a) {  // Выдать указатель на следующий элемент массива
  D("Выдать указатель на следующий элемент массива");
  static int current = -1;
  static array * list = NULL;

  if((list == NULL) || (list != a)) { list = a; current = 0; }

  void ** result = &((*list)[current++]);
  if(*result == ARRAY_LAST) {
    LTt(-);
    list = NULL;
    current = -1;
    result = ITER_FINISHED;
  }

  return result;
}

void array_foreach(array * a, callback_t cb) {
  iter_foreach(a, array_next, cb);
}
