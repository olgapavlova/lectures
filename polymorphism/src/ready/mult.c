#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "mult.h"

int main() {

  printf("%f\n", scalar_square("%d%g%g", 6, 2.4, 8.2));
  return 0;

}

double scalar_square(char const *type, ...) {
    double result = 0;

    va_list argp;
    va_start (argp, type);
    for (const char * p = type; *p != '\0'; ++p) {
        if (*p == '%') {
            ++p;
            switch (*p) {
                case 'd': {
                            int n = va_arg(argp, int);
                            result += n * n;
                            break;
                          }
                case 'g': {
                            double n = va_arg(argp, double);
                            result += n * n;
                            break;
                          }
            }

        }

    }

    va_end(argp);
    return result;
}
