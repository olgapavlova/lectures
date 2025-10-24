/*
   @file  matrix.c
   @brief Работа с матрицами. Реализация
*/

#include "matrix.h"

/// Инициировать матрицу
result_t matrix_init(matrix_t * m) {
  result_t result = RESULT_OK;

  if((m->h == 0) || (m->w == 0)) { result = RESULT_FAIL; return result; }

  // память под индекс строк
  m->data = malloc(m->h * sizeof(double *) + m->w * m->h * sizeof(double));  // память под указатели на строки, размер h (высота)
  if(!m->data) { result = RESULT_FAIL; return result; }

  // память под каждую конкретную строку
  for(int i = 0; i < m->h; i++) {
    m->data[i] = *m->data + m->h * sizeof(double *) + m->w * sizeof(double) * i;  // память под строку, размер w (ширина)
    if(!m->data[i]) { result = RESULT_FAIL; }
  }

  // если память выделилась плохо — подчищаем
  if(result == RESULT_FAIL) { matrix_destroy(m); }

  return result;
}

/// Создать новую матрицу
matrix_t matrix_new(uint8_t w, uint8_t h) {
  matrix_t result = { .w = w, .h = h };
  result_t init_result = matrix_init(&result);
  if(init_result != RESULT_OK) { matrix_destroy(&result); abort(); }
  return result;
}

/// Сбросить матрицу
void matrix_destroy(matrix_t * m) {
  free(m->data);
  m->w = 0;
  m->h = 0;
}

/// Проверить корректность значения элемента матрицы
static bool _is_value_valid(double val) {
  return (val <= VAL_MAX ) && (val >= VAL_MIN);
}

/// Заполнить матрицу случайными числами
void matrix_randomize(matrix_t * m) {
  static bool rand_init = false;
  if (rand_init == false) {
    srand(time(NULL));
    rand_init = true;
  }

  for(int i = 0; i < m->h; i++) {
    for(int j = 0; j < m->w; j++) {
      m->data[i][j] = VAL_MAX * ((double)rand() / RAND_MAX);
      int sign = (rand() % 2) ? 1 : -1;
      m->data[i][j] *= sign;
      if(!_is_value_valid(m->data[i][j])) { abort(); }
    }
  }
}

#if 0
static matrix_t matrix_transp(matrix_t * m) {
  matrix_t result = matrix_new(m->h, m->w);

  for(int i = 0; i < m->h; i++) {
    for(int j = 0; j < m->w; j++) {
      result.data[j][i] = m->data[i][j];
    }
  }

  return result;
}
#endif

/// Перемножить две матрицы
result_t matrix_multiply(matrix_t * a, matrix_t * b, matrix_t * c) {
  result_t result = RESULT_OK;

  if ((a->w != b->h) || (a->h != c->h) || (b->w != c->w)) { result = RESULT_FAIL; return result; }

  // транспонировать b
  // matrix_t bt = matrix_transp(b);

  // умножить построчно
  for (int i = 0; i < c->h; i++) {
    for (int j = 0; j < c->w; j++) {
      for (int k = 0; k < a->w; k++) {
        // if (j + 8 < b->w) { __builtin_prefetch(&(b->data[k][j + 8]), 0, 1); }
        c->data[i][j] += a->data[i][k] * b->data[k][j];
      }
    }
  }

  return result;
}

/// Перемножить матрицы и вернуть результат
matrix_t matrix_mult(matrix_t * a, matrix_t * b) {
  matrix_t result = matrix_new(a->w, b->h);
  matrix_multiply(a, b, &result);
  return result;
}
