#include <stdio.h>

#define MAX(a, b) ({        \
    __typeof__(a) _a = (a);  \
    __typeof__(b) _b = (b);  \
    _a > _b ? _a : _b;       \
})

int main(void) {
    printf("%d\n", MAX(10, 20));
}
