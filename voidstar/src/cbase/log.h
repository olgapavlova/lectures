/**
 * @file log.h
 * @brief Макросы для быстрого логирования
 */

// Директива условной компиляции
#ifndef LOG_H_LOADED
#define LOG_H_LOADED

#include <stdio.h>
#include <execinfo.h>  // для подсчёта глубины вызова

// Уровни логирования
#define LOG_LVL_NONE   -1
#define LOG_LVL_FATAL   0
#define LOG_LVL_ERROR   1
#define LOG_LVL_WARN    2
#define LOG_LVL_INFO    3
#define LOG_LVL_DEBUG   4
#define LOG_LVL_TRACE   5

// По умолчанию логирование отключено
#ifndef LOG_LVL
#define LOG_LVL LOG_LVL_NONE
#endif

// Настройки строки лога — значения и формат
#define LOG_C_VERSION (__STDC_VERSION__ == 201112L) ? "C11" : "C??"
#define LOG_PREFIX_FORMAT "%s [%s] %20s:%3d %25s() L%d •%*c"
#define LOG_PREFIX_VALS LOG_C_VERSION, __TIME__, __FILE__, __LINE__, __FUNCTION__

// Вывод строки лога в STDERR при условии попадания в уровень
#define L(level, pattern, ...) do { \
    if((level) <= LOG_LVL) { \
      int depth = log_stack_level() + 1; \
      fprintf(stderr, \
        LOG_PREFIX_FORMAT #pattern "\n", \
        LOG_PREFIX_VALS, \
        (level), \
        depth, \
        ' ', \
        __VA_ARGS__); \
    } \
  } while(0);

// Сокращения для распространённых типов записей без привязки к уровню
#define Lt(x) L(LOG_LVL, %s, #x)
#define Ls(x) L(LOG_LVL, %s, (x))
#define Ld(x) L(LOG_LVL, %i, (x))
#define Li(x) Ld(x)

// Сокращения для распространённых типов записей с привязкой к уровню
// ...FATAL
#define LF(x, ...) L(LOG_LVL_FATAL, x, __VA_ARGS__)
#define LFt(x) L(LOG_LVL_FATAL, %s, #x)
#define LFs(x) L(LOG_LVL_FATAL, %s, (x))
#define LFi(x) L(LOG_LVL_FATAL, %d, (x))
// ...ERROR
#define LE(x, ...) L(LOG_LVL_ERROR, x, __VA_ARGS__)
#define LEt(x) L(LOG_LVL_ERROR, %s, #x)
#define LEs(x) L(LOG_LVL_ERROR, %s, (x))
#define LEi(x) L(LOG_LVL_ERROR, %d, (x))
// ...WARN
#define LW(x, ...) L(LOG_LVL_WARN, x, __VA_ARGS__)
#define LWt(x) L(LOG_LVL_WARN, %s, #x)
#define LWs(x) L(LOG_LVL_WARN, %s, (x))
#define LWi(x) L(LOG_LVL_WARN, %d, (x))
// ...INFO
#define LI(x, ...) L(LOG_LVL_INFO, x, __VA_ARGS__)
#define LIt(x) L(LOG_LVL_INFO, %s, #x)
#define LIs(x) L(LOG_LVL_INFO, %s, (x))
#define LIi(x) L(LOG_LVL_INFO, %d, (x))
// ...DEBUG
#define LD(x, ...) L(LOG_LVL_DEBUG, x, __VA_ARGS__)
#define LDt(x) L(LOG_LVL_DEBUG, %s, #x)
#define LDs(x) L(LOG_LVL_DEBUG, %s, (x))
#define LDi(x) L(LOG_LVL_DEBUG, %d, (x))
// ...TRACE
#define LT(x, ...) L(LOG_LVL_TRACE, x, __VA_ARGS__)
#define LTt(x) L(LOG_LVL_TRACE, %s, #x)
#define LTs(x) L(LOG_LVL_TRACE, %s, (x))
#define LTi(x) L(LOG_LVL_TRACE, %d, (x))

// Сокращение для вывода краткого описания функции на уровне DEBUG
#define D(x) LDs(x)

/// Расчёт глубины вложенности функции, из которой идёт запись в лог
int log_stack_level();

#endif
