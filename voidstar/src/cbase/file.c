#define _GNU_SOURCE
#include "file.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "macro.h"
#include "regmem.h"
#include "str.h"
#include "array.h"
#include "callback.h"
#include "dict.h"
#include "log.h"
#include "buffer.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

static int _read_buffer_to_str(str * s, buffer * b, file * f) {  // Считать кусок файла до конца буфера или переноса строки
  D("Считать кусок файла до конца буфера или переноса строки");
  int b_position = 0;
  char c = '\0';
  bool eof = false;
  int result = 0;

  do {
    c = fgetc(f->stream);
    if (c == EOF) { eof = true; break; }
    (*b)[b_position++] = c;
    LT(%s %d, "Символ:", c);
  } while ((c != '\n') && b_position != BUFFER_SIZE);  // в конце, потому что нам нужно сохранить перенос

  // Не нужно: там всегда \0: (*buffer)[buff_position] = '\0';

  LT(%s %s, "Значение буфера:", *b);

  if(!eof) {
    LTs("Не конец файла");
    str_add(s, b);
    LT(%s %s, "Новое значение строки:", *s);
    buffer_clear(b);
    result = (b_position == BUFFER_SIZE) ? BUFFER_FULL : FILE_EOL;
  } else { result = FILE_EOF; }

  LT(%s %s, "Считаная часть строки:", *s);
  return result;
}

void _line_print(int n, void ** s) {  // Напечатать строку
  D("Напечатать строку");
  if(n != 1) {exit(EXIT_FAILURE); }
  str s_notn = str_drop_n(*(str*)s);
  printf("%s\n", s_notn);
}

file *file_init() {
  D("Инициировать структуру файла");
  file *result = malloc(sizeof(file));
  result->path = NULL;
  result->ok = -1;
  result->stream = NULL;
  regmem_add(result);
  return result;
}

file *file_create(str path) {
  D("Создать файл из строки пути");
  file *result = file_init();
  result->path = str_create(path);
  result->ok = strcmp(path, "-") ? file_is_ok(result) : 1;  // STDIN
  return result;
}

int file_is_ok(file *f) {
  D("Проверить, доступен ли файл для чтения");
  int result = f->ok;
  if (f->ok == -1) {
    struct stat fs;
    int file_exists = !stat(f->path, &fs);
    int file_is_usual = S_ISREG(fs.st_mode);
    int file_is_readable = !access(f->path, R_OK);
    result = file_exists * file_is_usual * file_is_readable;
  }
  return result;
}

void file_open(void *fv) {  // Закрыть стрим
  D("Открыть стрим");
  file * f = (file*)fv;
  f->stream = (!strcmp(f->path, "-")) ? stdin : fopen(f->path, "r");
  f->ok = (f->stream == NULL) ? 0 : 1;
}

void file_close(void *fv) {  // Закрыть стрим
  D("Закрыть стрим");
  file * f = (file*)fv;
  if ((f->stream != NULL) & (f->stream != stdin)) {
    fclose(f->stream);
  }
}

void file_show_metadata(void *fv) {  // Показать метаданные файла
  D("Показать метаданные файла");
  file * f = (file *)fv;
  printf("File: path = \"%s\", ok = %d\n", f->path, f->ok);
}

void * file_next_line(void * f, void * b) {  // Получить следующую строку файла
  D("Получить следующую строку файла");
  str result = str_create(*(buffer*)b);

  while(_read_buffer_to_str(&result, (buffer*)b, (file*)f) == BUFFER_FULL) {
    LTs("Читаем следующий буфер");
  };
  LT(%s %s %s, "Считали строку:", result, "<EOL>");
  if(feof(((file*)f)->stream)) { result = NULL; }
  return result;
}

void file_foreach(file * f, callback_t cb) {  // Применить функцию к каждой строке файла
  D("Применить функцию к каждой строке файла");
  buffer b = {0};
  buffer_init(&b);
  iter_foreach_buffer((void*)f, file_next_line, &b, cb);
}


void file_print(file * f) {  // Напечатать весь файл
  D("Напечатать весь файл");
  file_foreach(f, _line_print);
}




/*
/// TODO Куда это?!
void file_config(int argc, char **argv) {
  Ls("Получить все файлы из командной строки");
  gFILES = array_init();
  for (int i = optind; i < argc; ++i) {
    file *f = file_create(argv[i]);
    array_add(gFILES, f);
  }
  if (gFILES->length == 0) {
    str * stdash = str_create("-");
    file *f = file_create(stdash);
    array_add(gFILES, f);
  }
}
*/
