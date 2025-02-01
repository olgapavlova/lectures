#include <stdio.h>
#include <stdlib.h>
#include "../file.h"
#include "../str.h"
#include "../log.h"
#include "../buffer.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_TRACE

int main() {
  file * f = file_create("/Users/op/dev/intellib/sb/src/data/4.next");
  file_open(f);
  /*
  buffer buff = {0};
  buffer_init(&buff);
  str s1 = (str)file_next_line(f, &buff);
  str s2 = (str)file_next_line(f, &buff);
  str s3 = (str)file_next_line(f, &buff);
  //str s4 = (str)file_next_line(f, &buff);
  printf("%s", s1);
  printf("%s", s2);
  printf("%s", s3);
  //printf("%s", s4);
  */

  file_print(f);

    return EXIT_SUCCESS;
}
 
