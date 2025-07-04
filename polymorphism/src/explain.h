#ifndef EX_H_LOADED
#define EX_H_LOADED

typedef struct cx {
    int d;
    int i;
} cx;  // Комплексное число, ура!

#define explain(x, y) _Generic((x), \
    int: explain_int(y), \
    cx: explain_cx(y))((x), (y));

#define explain_int(y) _Generic((y), \
    int: ex_int_int, \
    cx: ex_int_cx)

#define explain_cx(y) _Generic((y), \
    int: ex_cx_int, \
    cx: ex_cx_cx)

void ex_int_int(int, int);
void ex_int_cx(int, cx);
void ex_cx_int(cx, int);
void ex_cx_cx(cx, cx);

#endif 
