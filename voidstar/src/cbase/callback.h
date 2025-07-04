#ifndef CALLBACK_H_LOADED
#define CALLBACK_H_LOADED

#include <stdarg.h>
#include <stdio.h>

/// Тип функций обратного вызова; первый параметр — кол-во аргументов
typedef void (*callback_t)(int, void **);

/// Исполнить функцию обратного вызова (превратить ... в void** + запуск)
void callback_exec(callback_t, int, ...);

#endif
