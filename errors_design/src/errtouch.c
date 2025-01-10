#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>

//static char * strerror_ru[107] = {"Непонятная ошибка", "Так нельзя"};

void a() {
    static int n = 0;
    printf("func a; n = %d; errno = %d\n", n++, errno);
}


int main() {
  //errno = 0;
  //FILE * f __attribute__ ((unused)) = fopen("blablabla", "r");
  //printf("%d\t%s\n", errno, strerror(errno));

  //for(int i = 0; i <= 107; ++i) {
  //   printf("%d\t%s\t%s\n", i, strerror(i), strerror_ru[i]);
  //}

  //printf("%s\n", getprogname());

  atexit(a);
  atexit(a);

  errno = 0;
  char * s __attribute__ ((unused)) = malloc(-1);
  if(errno != 0) { exit(EXIT_FAILURE);}

  exit(EXIT_SUCCESS);
}
 
