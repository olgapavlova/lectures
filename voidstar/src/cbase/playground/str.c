#include <stdio.h>
#include <stdlib.h>
#include "../str.h"
#include "../log.h"
#include "../cbase.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_TRACE

int main() {

  str a = str_create("blabla");
  LTs(a);
  str b = str_create("wow wow");
  LTs(b);
  str c = str_create("qwerty");
  LTs(c);
  str_clear(&b);
  LTs(b);
  str_copy(a, &b);
  LTs(b);


    return EXIT_SUCCESS;
}
 
