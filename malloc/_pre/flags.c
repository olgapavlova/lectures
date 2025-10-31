#include "config.h"

int main(void) {
    size_t req = 123;
    void *p = malloc(req);
    if (!p) return 1;

    size_t *header = (size_t *)p - 2;

    size_t prev_size = header[0];
    size_t raw_size  = header[1];

    const size_t PREV_INUSE = 0x1;
    const size_t IS_MMAPPED = 0x2;
    const size_t NON_MAIN_ARENA = 0x4;
    const size_t MASK = ~(PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA);

    size_t real_size = raw_size & MASK;

    printf("ptr: %p\n", p);
    printf("header: %p\n", (void*)header);
    printf("prev_size: 0x%zx\n", prev_size);
    printf("raw size:  0x%zx\n", raw_size);
    printf("real size (masked): 0x%zx (%zu)\n", real_size, real_size);
    printf("flags:\n");
    printf("  PREV_INUSE:     %s\n", (raw_size & PREV_INUSE) ? "yes" : "no");
    printf("  IS_MMAPPED:     %s\n", (raw_size & IS_MMAPPED) ? "yes" : "no");
    printf("  NON_MAIN_ARENA: %s\n", (raw_size & NON_MAIN_ARENA) ? "yes" : "no");

    printf("malloc_usable_size: %zu\n", malloc_usable_size(p));

    free(p);
    return 0;
}

