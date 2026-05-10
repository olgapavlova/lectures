#include <stdio.h>

void f(int n) {
    int arr[n];
    arr[0] = 42;
    printf("%d\n", arr[0]);
}

int main(void) {
    f(10);
}
