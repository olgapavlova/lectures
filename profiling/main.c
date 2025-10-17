// main.c — перемножение матриц

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_MAX 100
#define H_MAX 100

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

// Функция освобождения памяти из-под матрицы
void matrix_destroy(int ** m, int h) {
  for(int i = 0; i < h; i++) { free(m[i]); }
  free(m);
}

// Функция перемножения матриц — возвращает указатель на результат
void * mult(void * a, void * b, int wa, int ha);

int main(void) {

  // инициируем генератор случайных чисел
  srand(time(NULL));

  // получаем случайные значения ширины и высоты матрицы
  int wa = rand() % W_MAX + 1;
  int ha = rand() % H_MAX + 1;

  // выделяем память под матрицы
  int ** a = matrix_init(wa, ha);
  int ** b = matrix_init(ha, wa);

  // заполняем матрицы случайными числами


  // перемножаем матрицы

  // освобождаем память
  matrix_destroy(a, ha);
  matrix_destroy(b, wa);

  // конец
  return EXIT_SUCCESS;
}
