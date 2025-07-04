#include <stdio.h>
#include <pthread.h>
#include <string.h>

void * thread_function(void * name) {
  char * cn = (char*)name;
  int i = 0;
  while(cn[i] != '\0') {
    fprintf(stdout, "%c", cn[i++]);
  }
  fprintf(stdout, "\n");
  pthread_exit(NULL);
}

int main() {

  char son[] = "Вася";
  char dauther[] = "Маша";
  char mum[] = "Ираида Степановна";
  pthread_t s, d, m;
  pthread_create(&s, NULL, &thread_function, &son);
  pthread_create(&d, NULL, &thread_function, &dauther);
  pthread_create(&m, NULL, &thread_function, &mum);


  pthread_join(s, NULL);
  pthread_detach(d);
  pthread_detach(m);

  pthread_exit(NULL);
}
