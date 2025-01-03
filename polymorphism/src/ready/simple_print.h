#ifndef SIMPLE_PRINT_H_LOADED
#define SIMPLE_PRINT_H_LOADED

#define printype(x) print(x) print(type(x))

#define type(x) _Generic((x), \
    int: "int", \
    char *: "char *", \
    float: "float", \
    double: "double", \
    default: "unknown type" \
    )

#define print(x) _Generic((x), \
    int: print_int, \
    char *: print_str, \
    float: print_float, \
    double: print_double \
    )(x);

void print_int(int);
void print_str(char *);
void print_float(float);
void print_double(double);

#endif 
