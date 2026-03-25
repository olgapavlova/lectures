#ifndef JSON_H
#define JSON_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/// Проверка на символ из пробельных
static bool _is_whitespace_sign(char * p) {
  bool result = false;
  switch(*p) {
    case '\t': 
    case '\n':
    case ' ': result = true; break;
    default: break;
  }
  return result;
}

/// Указатель на следующий значимый символ
static char * _next_char(char * str) {
  static char * p = NULL;
  if (p == NULL) { p = str; } else { p++; }

  while(_is_whitespace_sign(p)) p++;  // игнорируем пробельные символы

  char * result = p;
  if (*p == '\0') { p = NULL; }  // сброс в конце строки

  return result;
}

bool is_json(char * str) {
  bool result = false;

  for (char * p = str; *p != '\0'; p = _next_char(str)) {
    switch (*p) {
      case '{': result = (p == str) ? true : false; break;
      case '}': if(*(p+1) != '\0') result = false; break;
      default: result = false; break;
    }
  }

  return result;
}

#endif
