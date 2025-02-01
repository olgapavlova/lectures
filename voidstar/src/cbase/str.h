/**
 * @file str.h
 * @brief Работа со строками как с динамическими данными
 */

#ifndef STR_H_LOADED
#define STR_H_LOADED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "buffer.h"
#include "callback.h"

/// Основной тип модуля — динамическая строка
typedef char * str;

/// Создать динамическую строку из какой угодно 
str str_create(str);

/// Вернуть указатель на следующий символ строки (для итераций)
void * str_next(void *);

/// Применить функцию к каждому символу строки
void str_foreach(str, callback_t);

/// Скопировать одну строку в другую
#define str_copy(x, y) _Generic((x), \
  char: str_copy_char, \
   str: str_copy_str \
  )((x), (y))
void str_copy_char(char, str*);
void str_copy_str(str, str*);

/// Проверить строку на пустоту (совсем пусто или '\n')
bool str_is_empty(str);

/// Обрезать '\n' в конце строки, если есть
str str_drop_n(str);

/// Обрезать строчку от и до 
str str_crop(str, int, int);

/// Очистить строку
void str_clear(str *);

/// Дописать одну строку в конец другой
#define str_add(x, y) _Generic((y), \
  str*: str_add_str, \
  char*: str_add_char, \
  char: str_add_letter, \
  buffer*: str_add_buffer \
  )((x), (y))
void str_add_str(str *, str *);
void str_add_char(str *, char *);
void str_add_letter(str *, char);
void str_add_buffer(str *, buffer *);

/// Заполнить строку символом на N позиций от начала
void str_fill(str *, char, int);

#endif
