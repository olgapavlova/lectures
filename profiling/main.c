// main.c — перемножение матриц
#include "config.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int main(void) {

  // выделяем память под матрицы
  matrix_t a = matrix_new(W_MAX, H_MAX);
  matrix_t b = matrix_new(W_MAX, H_MAX);

  // заполняем матрицы случайными числами
  //matrix_randomize(&a);
  //matrix_randomize(&b);

  // перемножаем матрицы
  // matrix_t c = matrix_mult(&a, &b);

  // освобождаем память
  matrix_destroy(&a);
  matrix_destroy(&b);
  // matrix_destroy(&c);

  // конец
  return EXIT_SUCCESS;
}
