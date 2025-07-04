#include <stdio.h>
#include <math.h>

#undef M_PI
#define M_PI 3.14

#define STOP(x) do {printf("%s\n", #x); } while(0);

#if 0
  #define IAMSTATIC "Me, too"
#endif


int main() {
  printf("%f\n", M_PI);
  STOP(Остановите меня пожалуйста);
}
