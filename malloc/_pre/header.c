#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>


int main(void) {
    void *p = malloc(123);

    size_t *header = (size_t *)p - 2;

    printf("prev_size:   %zd\n", header[0]);
    printf("size:        %zd\n", header[1]);
    printf("usable size: %zu\n", malloc_usable_size(p));
    printf("user ptr:    %p\n", p);
    printf("header:      %p\n", (void*)header);

    free(p);
    return 0;
}

