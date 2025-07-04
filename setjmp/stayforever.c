#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env;

void handler(int sg) {
  printf("Тут вот что случилось: %d. Давайте-ка ещё раз.\n");
  longjmp(env, 93);
}

int main() {
  int a, b;

  signal(SIGINT, handler);

  setjmp(env);

  printf("Введите два числа: ");
  scanf("%d %d", &a, &b);

  printf("Частное: %.2f\n", (double)a / b);
}
