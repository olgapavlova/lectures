/**
 * @file array.h
 * @brief Работа с динамическими массивами из void-указателей
 */

#ifndef ARRAY_H_LOADED
#define ARRAY_H_LOADED

#include "callback.h"

/// Последний указатель массива
#define ARRAY_LAST NULL

/// Объект-массив void-указателей
typedef void ** array;

/// Инициировать динамический массив указателей
array array_init();

/// Посчитать длину массива (не включая последний элемент)
int array_len(array *);

/// Добавить элемент в динамический массив
void array_add(array *, void *);

/// Выдать указатель на следующий элемент массива
void * array_next(void *);

/// Применить одну функцию к каждому элементу массива
void array_foreach(array *, callback_t);

#endif
