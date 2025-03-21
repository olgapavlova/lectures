#include <stdio.h>
#include <pthread.h>

void * out(void * name) {
  printf("%s\n", (char*)name);
  pthread_exit(NULL);
}

int main() {
  char a[] = "Вася";
  char b[] = "Маша";
  char c[] = "Ираида Константиновна";

  pthread_t ta, tb, tc;

  pthread_create(&ta, NULL, &out, a);
  pthread_create(&tb, NULL, &out, b);
  pthread_create(&tc, NULL, &out, c);

  pthread_join(ta, NULL);
  pthread_join(tb, NULL);
  pthread_join(tc, NULL);

  pthread_exit(NULL);
}
