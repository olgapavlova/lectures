#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void task(int n) {
  printf("Задание %d\n", n);
  if(n == 2) { longjmp(env, 17); }
}

void no_task() {
  printf("Явка провалена\n");
}

int main() {
  volatile int counter = -25;
  printf("Счётчик: %d\n", counter);

  if(setjmp(env) == 0) {
    // начинаем транзакцию
    task(1);
    counter++;
    printf("Счётчик: %d\n", counter);

    task(2);
    counter++;
    printf("Счётчик: %d\n", counter);

    task(3);
    counter++;
    printf("Счётчик: %d\n", counter);
    // заканчиваем
  } else {
    // что-то в транзакции пошло не так
    no_task();
    printf("Тем временем счётчик: %d\n", counter);
    // ну не так и не так, проехали
  }

  // дальше всё хорошо
  printf("Дальше всё хорошо.\n");
  printf("В итоге счётчик: %d\n", counter);

}
