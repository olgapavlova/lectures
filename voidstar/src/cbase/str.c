/** Dynamic String Module
 */
#include "cbase.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regmem.h"
#include "str.h"
#include "buffer.h"
#include "log.h"
#include "callback.h"
#include "iter.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

str str_create(str source) {  // Создать динамическую строку из какой угодно
  D("Создать динамическую строку из какой угодно");
  int source_len = strlen(source);
  str result = malloc(sizeof(char)*(source_len + 1));
  strncpy(result, source, sizeof(char)*source_len);
  result[source_len] = '\0';
  regmem_add(result);  // в реестр динамической памяти
  return result;
}

void * str_next(void * s) {  // Вернуть указатель на следующий символ строки (для итераций)
  D("Вернуть указатель на следующий символ строки (для итераций)");
  static char * current = NULL;
  static str list = NULL;

  if ((list == NULL) | (list != (str)s)) { current = (str)s; list = (str)s; }

  void * result = (void *)current;
  if(*current == '\0') { result = current = ITER_FINISHED; } else { current++; }
  return result;
}

void str_foreach(str s, callback_t cb) {  // Применить функцию к каждому символу строки
  D("Применить функцию к каждому символу строки");
  iter_foreach(s, str_next, cb);
}

void str_copy_char(char from, str * to) {  // Скопировать символ в строку
  D("Скопировать символ в строку");

  regmem_realloc((void**)to, (strlen((str)*to) + 1) * sizeof(char), sizeof(char) * 2);

  (*to)[0] = from;
  (*to)[1] = '\0';
  LT(%s %s, "Новая строка:", *to);
}

void str_copy_str(str from, str * to) {  // Скопировать значение одной строки в другую"
  D("Скопировать значение одной строки в другую");
  int from_len = strlen(from);
  regmem_realloc((void**)to,
      (strlen((str)*to) + 1) * sizeof(char),
      sizeof(char) * (from_len + 1));
  strncpy(*to, from, from_len);
  ((str)*to)[from_len] = '\0';
  LTs(*to);
}

bool str_is_empty(str s) {  // Проверить строку на пустоту (совсем пусто или '\n')
  D("Проверить строку на пустоту (совсем пусто или '\\n')");
  bool result = false;
  if((s[0] == '\0') | (s[0] == '\n')) { result = true; }
  return result;
}

str str_drop_n(str s) {  // Обрезать '\n' в конце строки, если есть
  D("Обрезать '\\n' в конце строки, если есть");

  str result = str_create(s);

  int s_len = strlen(s);
  if ((s_len != 0) & (s[s_len - 1] == '\n')) {
    result = realloc(result, sizeof(char)*s_len);
    result[s_len - 1] = '\0';
  }

  return result;
}

str str_crop(str s, int from, int to) {  // Обрезать строчку от и до
  D("Обрезать строчку от и до");
  
  int s_len = strlen(s);

  from = (from < 0) ? 0 : from;
  from = (from > s_len - 1) ? s_len - 1 : from;

  to = (to < 0) ? s_len - 1 : to;
  to = (to > s_len - 1) ? s_len - 1 : to;

  str result = str_create(&(s[from]));

  result = realloc(result, sizeof(char) * (to - from + 1));
  result[to - from] = '\0';

  return result;
}

void str_clear(str * s) {  // Очистить строку
  D("Очистить строку");
  regmem_realloc((void**)s, sizeof(char) * (strlen(*s) + 1), 1);
  (*s)[0] = '\0';
  LTs(*s);
}

void str_add_str(str * s, str * end) {  // Дописать одну строку в конец другой
  D("Дописать одну строку в конец другой");
  int s_len = strlen(*s);
  int end_len = strlen(*end);
  regmem_realloc((void**)s, sizeof(char)*(s_len + 1), sizeof(char)*(s_len + end_len + 1));

  for(int i = 0; i < end_len; ++i) { (*s)[s_len + i] = (*end)[i]; }
  (*s)[s_len + end_len] = '\0';
}

void str_add_char(str * s, char * end) {  // Дописать статичную строку в конец динамической
  D("Дописать статичную строку в конец динамической");
  int s_len = strlen(*s);
  int end_len = strlen(end);
  regmem_realloc((void**)s, sizeof(char)*(s_len + 1), sizeof(char)*(s_len + end_len + 1));

  for(int i = 0; i < end_len; ++i) { (*s)[s_len + i] = end[i]; }
  (*s)[s_len + end_len] = '\0';
}

void str_add_letter(str * s, char end) {  // Дописать символ в конец строки 
  D("Дописать символ в конец строки");
  int s_len = strlen(*s);
  regmem_realloc((void**)s, sizeof(char)*(s_len + 1), sizeof(char)*(s_len + 2));

  (*s)[s_len] = end;
  (*s)[s_len + 1] = '\0';
}

void str_add_buffer(str * s, buffer * end) {  // Дописать буфер в конец строки 
  D("Дописать буфер в конец строки");
  int s_len = strlen(*s);
  LTs(*s);
  int end_len = strlen(*end);
  LTs(*end)
  regmem_realloc((void**)s, sizeof(char)*(s_len + end_len + 1), sizeof(char)*(s_len + 1));

  LT(%s %d %d, "Увеличили размер строки", s_len, end_len);
  for(int i = 0; i < end_len; ++i) {
    LTi(i);
    (*s)[s_len + i] = (*end)[i];
    LT(%s %d, "Новый символ:", (*end)[i]);
  }
  (*s)[s_len + end_len] = '\0';
}

void str_fill(str * s, char c, int n) {
  int s_len = strlen(*s);
  if(s_len < n) { regmem_realloc((void**)s, sizeof(char) * (s_len + 1), sizeof(char)* (n + 1)); }
  for(int i = 0; i < n; i++) { (*s)[i] = c; }
  if(s_len < n) { (*s)[n] = '\0'; }
}
