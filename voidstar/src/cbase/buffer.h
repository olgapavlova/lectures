#ifndef BUFFER_H_LOADED
#define BUFFER_H_LOADED

#define BUFFER_SIZE 1000
#define BUFFER_FULL -1

typedef char buffer[BUFFER_SIZE];

/// Инициализировать буфер
void buffer_init(buffer *);

/// Очистить буфер
#define buffer_clear(x) buffer_init((x))

#endif
