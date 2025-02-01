/**
 * @file file.c
 * @brief Работа с текстовым файлом построчно
 */
#ifndef FILE_H_LOADED
#define FILE_H_LOADED

#include <stdio.h>

#include "str.h"
// #include "array.h"
#include "callback.h"
#include "buffer.h"

#define FILE_EOL 1
#define FILE_EOF 2

/// Структура объект-файла
typedef struct file {
  str path;    /**< Путь к файлу */
  int ok;       /**< 1, если это обычный файл, доступный для чтения */
  FILE *stream; /**< Поток для чтения (пока только чтение) */
} file;

/// Инициировать структуру файла 
file *file_init();

/// Создать файл на основе введённого пути
file * file_create(str x);

/// Проверить, что файл существует, обычный и открыт для чтения
int file_is_ok(file *);

// Стрим не выносим в отдельный объект — его не надо обновлять, только вкл/выкл
void file_open(void *);  // обе функции — callback_t
void file_close(void *);

void file_show_metadata(void *);  // callback_t
// void file_show_all(); // переписать на iter

/// Получить следующую строку файла
void * file_next_line(void *, void *);  // callback_t

/// Применить функцию к каждой строке файла
void file_foreach(file *, callback_t);

/// Напечатать весь файл
void file_print(file *);



// ! Это не сюда
/// Считать все файлы из командной строки
void file_config(int, char **);

#endif
