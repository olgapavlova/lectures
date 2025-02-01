/**
 * @file counter.h
 * @brief Счётчик — статичный массив целых чисел с char-ключами
 */

#ifndef COUNTER_H_LOADED
#define COUNTER_H_LOADED

#include "cbase.h"
#include "iter.h"
#include "callback.h"

/// Счётчик — массив с char-ключами
typedef int counter[128];

/// Применить функцию ко всем значениям счётчика
void counter_foreach(counter *, callback_t);

/// Выдать следующий элемент списка
void * counter_next(void *);  // тип iter_next_t

#endif
