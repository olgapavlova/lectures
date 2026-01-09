#include <stdio.h>

// Делаем float прозрачным

typedef union flt_u {
  float value;
  struct {
    int wtf:23;
    int degree:7;
    unsigned degree_sign:1;
    unsigned s:1;
  };
} flt_t;

// Покажи кусок памяти
void show_memory(void *, size_t);

// Раскрасить цифру
void show_colored_digit(int);

// Вывести или не вывести разделитель после позиции
void show_divider(int);

int main(void) {

  for(float i = 1.125; i < 2048; i *= 2) {
    flt_t a = { i };
    printf("\n%5.8f\t", a.value);
    show_memory(&i, sizeof i);
    printf("\t\t%1d %1d %7d %23d\n",
        a.s,
        a.degree_sign,
        a.degree,
        a.wtf
        );
  }

  return 0;
}

void show_memory(void * p, size_t n) {
  for(size_t i = n; i > 0; i--) {
    for(int d = 7; d >= 0; d--) {
      show_colored_digit((*((char *)p + i - 1) >> d) & 0x1);
      show_divider((n - i) * 8 + (7 - d));
    }
  }
    printf("\n");
}

void show_colored_digit(int d) {
  switch (d) {
    case 1:  printf("\x1b[33m%1d\x1b[0m", d); break;
    default: printf("%1d", d); break;
  }
}

void show_divider(int n) {
  static int div[] = { 0, 1, 8, -1 };

  for(int i = 0; div[i] != -1; i++) {
    if (div[i] == n) {
      printf("%c", ' ');
      break;
    }
  }
}
