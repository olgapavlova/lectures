/**
 * @file callback.c
 * @brief Реализация функциональности обратного вызова
 */
#include "callback.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void callback_exec(callback_t cb, int count,
                   ...) {  // Исполнить функцию обратного вызова
  D("Исполнить функцию обратного вызова");

  va_list args;  // • нам нужно превратить ... в void **
  void *arg_list[count];

  va_start(args, count);  // • создаём void ** для вызова callback
  for (int i = 0; i < count; i++) {
    arg_list[i] = va_arg(args, void *);
  }
  va_end(args);

  cb(count, arg_list);  // • аргументы готовы, вызываем функцию
}
