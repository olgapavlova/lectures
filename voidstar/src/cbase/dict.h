/**
 * @file dict.c
 * @brief Словарь — статичный массив void-указателей с char-ключами
 */

#ifndef DICT_H_LOADED
#define DICT_H_LOADED

#include "cbase.h"
#include "callback.h"

/// Словарь с char-ключами
typedef void * dict[128];

/// Выдать следующий элемент словаря
void * dict_next(void *);

/// Стандартный foreach применительно к словарю
void dict_foreach(dict *, callback_t);

#endif
