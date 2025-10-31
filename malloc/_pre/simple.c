#include <stdio.h>
#include <stdlib.h>

int main(void) {
    void *p = malloc(123);
    printf("malloc returned: %p\n", p);
    getchar();
    return 0;
}

