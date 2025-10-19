// main.c — перемножение матриц
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define W_MAX 500
#define H_MAX 500

#define M_MAX 2
#define M_MIN -3

#if(S)
#define SHOW(tmpl, ...) printf(tmpl, __VA_ARGS__);
#else
#define SHOW(tmpl, ...) 
#endif

static inline uint64_t read_cntvct(void) {
    uint64_t val;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r" (val));
    return val;
}

static inline uint64_t read_cntfrq(void) {
    uint64_t val;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r" (val));
    return val;
}

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

  // получаем значения ширины и высоты матрицы
  int wa = W_MAX;
  int ha = H_MAX;

  // выделяем память под матрицы
  int ** a = matrix_init(wa, ha);
  int ** b = matrix_init(ha, wa);

  // заполняем матрицы случайными числами
  matrix_randomize(a, wa, ha);
  matrix_randomize(b, ha, wa);

  // настраиваем измерение времени;
  uint64_t freq = read_cntfrq();

  // прогрев кэша
  for (int i = 0; i < 5; i++) {
    matrix_multiply(a, b, wa, ha);
  }

  // перемножаем матрицы
  uint64_t start = read_cntvct();
  matrix_multiply(a, b, wa, ha);
  uint64_t finish = read_cntvct();

  // освобождаем память
  matrix_destroy(a, ha);
  matrix_destroy(b, wa);

  // отчитываемся
  long double time = (double)(finish - start) / freq;
  printf("time: %Lf\n", time);

  // конец
  return EXIT_SUCCESS;
}
