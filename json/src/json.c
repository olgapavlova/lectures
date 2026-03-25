#ifndef JSON_H
#define JSON_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>


bool is_json(char * str) {
  bool result = false;

  for(char * p = str; *p != '\0'; p++) {
    switch (*p) {
      case '\t':
      case ' ': break;

      case '{': if(p == str) result = true; break;
      case '}': if(*(p+1) != '\0') result = false; break;

      default: result = false; break;
    }
  }

  return result;
}

#endif
