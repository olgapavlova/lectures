// main.c — перемножение матриц
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_MAX 500
#define H_MAX 500

#define M_MAX 1
#define M_MIN -1

#if(S)
#define SHOW(tmpl, ...) printf(tmpl, __VA_ARGS__);
#else
#define SHOW(tmpl, ...) 
#endif

// Функция выделения памяти под матрицу
int ** matrix_init(int w, int h) {
  int ** result = calloc(h, sizeof(int *));  // память под указатели на строки, размер h (высота)
  if(result == NULL) { abort(); }

  for(int i = 0; i < h; i++) {
    result[i] = calloc(w, sizeof(int));  // память под строку, размер w (ширина)
    if(result[i] == NULL) { abort(); }
  }
  return result;
}

// Функция заполнения матрицы случайными числами
void matrix_randomize(int ** m, int w, int h) {
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      m[i][j] = rand() % (M_MAX - M_MIN + 1) + M_MIN;
      SHOW("%d\n", m[i][j]);
    }
  }
}

// Функция освобождения памяти из-под матрицы
void matrix_destroy(int ** m, int h) {
  for(int i = 0; i < h; i++) { free(m[i]); }
  free(m);
}

// Функция перемножения матриц — возвращает указатель на результат
int ** matrix_multiply(int ** a, int ** b, int w, int h) {
  int ** result = matrix_init(w, h);

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      for (int k = 0; k < w; k++) {
        result[i][j] += a[i][k] * b[k][j];
      }
        SHOW("i: %d\tj: %d | %d\n", i, j, result[i][j]);
    }
  }

  return result;
}

int main(void) {

  // инициируем генератор случайных чисел
  srand(time(NULL));

  // получаем случайные значения ширины и высоты матрицы
  int wa = W_MAX; // rand() % W_MAX + 1;
  int ha = H_MAX; // rand() % H_MAX + 1;

  // выделяем память под матрицы
  int ** a = matrix_init(wa, ha);
  int ** b = matrix_init(ha, wa);

  // заполняем матрицы случайными числами
  matrix_randomize(a, wa, ha);
  matrix_randomize(b, ha, wa);

  // настраиваем измерение времени;
  struct timespec start, stop;

  // перемножаем матрицы
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  matrix_multiply(a, b, wa, ha);
  clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

  // освобождаем память
  matrix_destroy(a, ha);
  matrix_destroy(b, wa);

  // отчитываемся
  double time = (stop.tv_sec - start.tv_sec) + 
                (stop.tv_nsec - start.tv_nsec) / 1e9;
  printf("time: %lf\n", time);

  // конец
  return EXIT_SUCCESS;
}
