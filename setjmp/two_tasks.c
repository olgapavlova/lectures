#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static jmp_buf a, b;
static int task = 1;
static int num = 0;

void switch_tasks() {
  if (task == 1) {
    task = 2;
    longjmp(b, 1);
  } else {
    task = 1;
    longjmp(a, 1);
  }
}

void task_a() {
  if (setjmp(a) == 0) {
  } else {
    printf("\033[1;%dHA\n", num);
    sleep(1);
    switch_tasks();
  }
}

void task_b() {
  if (setjmp(b) == 0) {
  } else {
    printf("\033[2;%dHB\n", (num++) * 2);
    sleep(1);
    switch_tasks();
  }
}

int main() {
  printf("\033[2J\033[H");
  task_a();
  task_b();
  switch_tasks();
}
