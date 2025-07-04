#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef STRL
#define STRL 1000
#define PORTLIST_LEN 50

typedef struct port_description_t {
  int number;
  char protocol[STRL];
  char description[STRL];
} port_description_t;

typedef struct portlist_t {
  int len;
  port_description_t data[PORTLIST_LEN];
} portlist_t;

/// Прочесть описания портов из файла
void read_portlist_from_file(portlist_t *, char *);
