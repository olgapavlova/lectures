#ifndef REGMEM_H_LOADED
#define REGMEM_H_LOADED

/// Добавить указатель в реестр
void regmem_add(void *);

/// Заменить один указатель на другой (нужно при realloc)
void regmem_replace(void *, void *);

/// Освободить всю память
void regmem_destroy();

/// Изменить размер памяти по указателю
void regmem_realloc(void **, int, int);


#endif
