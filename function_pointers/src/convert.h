#ifndef CONVERT_H_LOADED
#define CONVERT_H_LOADED

#define type(x) _Generic((x), \
        int: 0, \
        char: 1, \
        double: 2, \
        default: 3 \
)

#define convert(a, x)                                                          \
  _Generic((x),                                                                \
      int: _convert_int(a, x),                                                 \
      double: _convert_double(a, x),                                           \
      char: _convert_char(a, x),                                               \
      default: convert_error(ERR_CONVERT_TYPE))

void convert_error(int);
void _convert_int(char **, int);
void _convert_char(char **, char);
void _convert_double(char **, double);

#define ERR_CONVERT_TYPE -35

#endif
