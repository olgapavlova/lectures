#ifndef PARABOLA_H_LOADED
#define PARABOLA_H_LOADED

typedef struct cx {
    int r;
    int i;
} cx;

#define explain(x, y) _Generic((x), \
    int: explain_int(y), \
    cx: explain_cx(y))((x), (y));

#define explain_int(y) _Generic((y), \
    int: explain_int_int, \
    cx: explain_int_cx)

#define explain_cx(y) _Generic((y), \
    int: explain_cx_int, \
    cx: explain_cx_cx)

void explain_int_int(int, int);
void explain_cx_int(cx, int);
void explain_int_cx(int, cx);
void explain_cx_cx(cx, cx);

#endif 
