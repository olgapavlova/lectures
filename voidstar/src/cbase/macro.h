/**
 * @file macro.h
 * @brief Макросы для ускорения написания кода
 */

#ifndef MACRO_H_LOADED
#define MACRO_H_LOADED

#define SKIP \
  __attribute__((unused))  // сигнал компилятору: переменная не будет
                           // использована, и это не ошибка


/// Проверить переменную на значение — и если равны, выйти с результатом
#define CHECK_EQ(var, value) do { \
    if((var) != (value)) { return; } \
  } while(0);

/// Проверить переменную на значение — и если НЕ равны, выйти с результатом
#define CHECK_NE(var, value) do { \
    if((var) == (value)) { return; } \
  } while(0);




#define ERR(x)  Ls((x))

#define STOP(x)         \
  do {                  \
    ERR((x));           \
    exit(EXIT_FAILURE); \
  } while (0);

#define NOT_NULL(x)                        \
  do {                                     \
    if ((x) == NULL) {                     \
      STOP("STOP • В функцию пришёл NULL") \
    };                                     \
  } while (0);

#define NOT_NULL_WEAK(x)                      \
  do {                                        \
    if ((x) == NULL) {                        \
      ERR("WARNING • В функцию пришёл NULL"); \
      return;                                 \
    }                                         \
  } while (0);

#define NOT_NULL_POINTER(x)                             \
  do {                                                  \
    if (*(x) == NULL) {                                 \
      STOP("STOP • В функцию пришёл указатель на NULL") \
    };                                                  \
  } while (0);

#define NOT_NULL_POINTER_WEAK(x)                           \
  do {                                                     \
    if ((x) == NULL) {                                     \
      ERR("WARNING • В функцию пришёл указатель на NULL"); \
      return;                                              \
    }                                                      \
  } while (0);

#define NOT_ZERO_WEAK(x)                              \
  do {                                                \
    if ((x) == 0) {                                   \
      ERR("WARNING • Нулевое значение где не ждали"); \
      return;                                         \
    }                                                 \
  } while (0);

#define NOT_ZERO(x)                              \
  do {                                                \
    if ((x) == 0) {                                   \
      STOP("STOP • Нулевое значение где не ждали"); \
      return;                                         \
    }                                                 \
  } while (0);

#define NOT_ZERO_RETURN_NULL(x)                       \
  do {                                                \
    if ((x) == 0) {                                   \
      ERR("WARNING • Нулевое значение где не ждали"); \
      return NULL;                                    \
    }                                                 \
  } while (0);

#define NOT_NULL_RETURN_INT(x)                   \
  do {                                           \
    if ((x) == NULL) {                           \
      ERR("WARNING • NULL в значимом атрибуте"); \
      return -1;                                 \
    }                                            \
  } while (0);

#define NOT_NULL_RETURN_NULL(x)                  \
  do {                                           \
    if ((x) == NULL) {                           \
      ERR("WARNING • NULL в значимом атрибуте"); \
      return NULL;                               \
    }                                            \
  } while (0);

#endif
