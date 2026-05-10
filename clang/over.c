#include <stdio.h>
#include <limits.h>

int main(void) {
    int x = INT_MAX;
    if (x + 1 < x) {
        puts("overflow detected");
    } else {
        puts("no overflow");
    }
}
