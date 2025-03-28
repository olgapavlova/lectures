#include "config.h"

void read_portlist_from_file(portlist_t* pl, char* path) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }

  char buffer[STRL] = {'\0'};
  pl->len = 0;
  while (fgets(buffer, STRL, f)) {
    (pl->data[pl->len]).number = atoi(strtok(buffer, ":"));
    strcpy((pl->data[pl->len]).protocol, strtok(NULL, ":"));
    strcpy((pl->data[pl->len]).description, strtok(NULL, ":\n"));
    (pl->len)++;
  }
}
