#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include "log.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_DEBUG

/// Расчёт глубины вложенности функции, из которой идёт запись в лог
/// Работает только в POSIX-системах
/// Внутри этой функции логи вызывать не стоит — зациклится (сама используется при вызове логов)
int log_stack_level() {
  void *buffer[10] = {};
  int result = backtrace(buffer, 10) - 3;
  return result;
}

