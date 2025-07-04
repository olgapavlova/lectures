#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * get_symbol(void * tv) {
  int * t = (int*)tv;
  int i = 0;
  char a = '\n';
  while(i++ < 100) {
    if (a == '\n') {
    pthread_mutex_lock(&lock);
    printf("%d: ", *t);
    pthread_mutex_unlock(&lock); }
    a = fgetc(stdin);
  }

  pthread_exit(NULL);
}

void * counter(void * tv) {
  int * t = (int*)tv;
  int i = 0;
  while(i++ < 100) {
    pthread_mutex_lock(&lock);
    (*t)++;
    pthread_mutex_unlock(&lock);
    sleep(1);
  }

  pthread_exit(NULL);
}


int main() {
  pthread_t g, o;
  int time = 0;

  pthread_create(&o, NULL, &counter, &time);
  pthread_create(&g, NULL, &get_symbol, &time);

  pthread_join(g, NULL);
  pthread_join(o, NULL);

  pthread_exit(NULL);
}
