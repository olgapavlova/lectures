#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char * debug = getenv("DEBUG");
    printf("DEBUG = %s\n", debug ? debug : "undefined");
    return 0;
}
