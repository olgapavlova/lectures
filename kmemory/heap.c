#include <stdio.h>
#include <unistd.h>  // Для sbrk()
#include <stdlib.h>  // Для malloc()

int main(void)
{
    void *heap_start, *heap_after_malloc;

    // Узнаём текущую верхнюю границу кучи
    heap_start = sbrk(0);
    printf("Heap start: %p\n", heap_start);

    // Выделяем всего 1 байт через malloc
    void *p = malloc(1);
    if (!p) {
        perror("malloc");
        return 1;
    }

    // Снова проверяем границу кучи
    heap_after_malloc = sbrk(0);
    printf("Heap after malloc(1): %p\n", heap_after_malloc);

    printf("Diff: %d\n", (int)(heap_after_malloc - heap_start));

    // Выводим адрес выделенного блока
    printf("malloc(1) returned pointer: %p\n", p);

    // Очищаем память
    free(p);

    return 0;
}
