#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
  for(char * i = argv[1]; *i != '\0'; i++) {
    printf("%x", *i);
  }
  printf("\n");

  return 0;
}
