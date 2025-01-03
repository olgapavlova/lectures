#include <stdio.h>
#include <stdlib.h>
#include "simple_print.h"

int main() {
    printype("abc")
    printype(7)
    printype(28.8)

    print("abc")
    print(7)
    print(82.8)
}

void print_int(int n) {
    printf("%d\n", n);
}
void print_str(char * s) {
    printf("%s\n", s);
}
void print_float(float f) {
    printf("%f\n", f);
}
void print_double(double f) {
    printf("%f\n", f);
}
