#ifndef ITER_H_LOADED
#define ITER_H_LOADED

#include "callback.h"
#include "iter.h"

//map — создаёт новый «список» применением функции к элементам старого
//filter — фильтрует список по критерию (и создаёт новый)
//reduce — свёртка списка в одно значение (нпр., сумма)
//foreach — как map, но не создаёт новый «список», а просто выполняет действие
//zip/unzip — собирает в пары или разбивает их
//take/drop — выборка/отбрасывание элементов
//scan — накопительная свёртка (сохраняет промежуточные результаты)
//iterate — генерирует последовательность (исходного «списка» нет)
//chunk/group — разбивает на «подсписки»
//partition — разделяет на два списка (удовлетворяет/нет требованию)

#define ITER_FINISHED NULL

/// Тип функций вызова следующего элемента «списка»
typedef void * (*iter_next_t)(void *);
typedef void * (*iter_next_buffer_t)(void *, void *);

/// Применить функцию к каждому элементу «списка»
void iter_foreach(void *, iter_next_t, callback_t);

/// Применить функцию к каждому элементу, используя буфер
void iter_foreach_buffer(void *, iter_next_buffer_t, void *, callback_t);

#endif
