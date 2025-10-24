/*
	@file  matrix.h
	@brief Работа с матрицами. Объявления
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <stdbool.h>
#include <time.h>

#include "config.h"

/// Максимальные размеры матрицы
#define W_MAX UINT8_MAX
#define H_MAX UINT8_MAX

/// Максимальное и минимальное значения элементов матрицы
#define VAL_MAX 100
#define VAL_MIN -100

// Тип «матрица»
typedef struct matrix_s {
	double ** data;
	uint8_t w;
	uint8_t h;	
} matrix_t;

/// Инициировать матрицу
result_t matrix_init(matrix_t *);

/// Создать новую матрицу
matrix_t matrix_new(uint8_t w, uint8_t h);

/// Заполнить матрицу случайными числами
void matrix_randomize(matrix_t * m);

/// Обнулить матрицу и очистить её динамическую память
void matrix_destroy(matrix_t *);

/// Перемножить две матрицы
result_t matrix_multiply(matrix_t *, matrix_t *, matrix_t *);

/// Перемножить две матрицы и вернуть результат
matrix_t matrix_mult(matrix_t *, matrix_t *);

#endif
