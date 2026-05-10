#include <stdio.h>

int main(void) {
    int add(int a, int b) {
        return a + b;
    }

    printf("%d\n", add(2, 3));
    return 0;
}
