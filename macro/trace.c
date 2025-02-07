#include <stdio.h>

#define TRACE(x) do { \
  printf(#x ": %d\n", (x)); \
   } while(0);

int main() {
  int age = 13;
  int school = 30;

  // printf("school: %d\n", school);
  // printf("age: %d\n", age);


  if(age>10) TRACE(age) else TRACE(school) 

  TRACE(age);
  TRACE(school);
}
