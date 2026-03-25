#ifndef ZERO_H
#define ZERO_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>


bool is_json(char * str) {
  bool result = true;

  result &= (str[0] == '{');
  result &= (((str[1] == ' ') && (str[2] == '}') && (str[3] == '\0'))
          || ((str[1] == '}') && (str[2] == '\0')));

  return result;
}

#endif
