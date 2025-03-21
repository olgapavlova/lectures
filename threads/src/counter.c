#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * get_symbol(void * time);
void * counter(void * time);

int main() {

  int time = 0;

  pthread_t s, c;

  pthread_create(&c, NULL, &counter, &time);
  pthread_create(&s, NULL, &get_symbol, &time);

  pthread_join(c, NULL);
  pthread_join(s, NULL);

  return 0;
}

void * get_symbol(void * time) {
  int * t = time;
  char c = '\n';

  while(1) {
    if(c == '\n') {
      pthread_mutex_lock(&lock);
      printf("Время %d: ", *t);
      pthread_mutex_unlock(&lock);
    }
    c = fgetc(stdin);
  }

  pthread_exit(NULL);
}

void * counter(void * time) {
  int * t = time;
  while(1) {
    pthread_mutex_lock(&lock);
    (*t)++;
    pthread_mutex_unlock(&lock);
    sleep(1);
  }
  pthread_exit(NULL);
}

