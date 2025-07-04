#ifndef HELLO_H_LOADED
#define HELLO_H_LOADED

#define print(x) _Generic((x), \
    int: print_int, \
    char *: print_string)(x);

void print_int(int n);
void print_string(char * s);

#endif 
