#include <stdio.h>
#include <math.h>

// Удобный для анализа тип данных
typedef union dbl_u {
  float value;
  struct {
    unsigned mantiss:23;
    int degree:7;
    unsigned degree_sign:1;
    unsigned sign:1;
  };
} dbl_t;

// Цветные цифры
void show_colored_int(int c) {
  switch (c) {
    case 1: printf("\x1b[33m%d\x1b[0m", c); break;
    default: printf("%d", c); break;
  }
}

// Вставлять разделители
char divider(int p) {
  static int pos[] = { 0, 1, 8, -1 }; // Вот бы нам не -1 сюда ставить, а что-то совсем невероятное

  char result = '\0';
  for(int i = 0; pos[i] != -1; i++) {
    if (pos[i] == p) result = ' ';
  }

  return result;
}


// Печатать бинарный отрезок памяти
void show_memory(void * p, size_t n) {
  for(size_t i = 1; i <= n; i++) {
    for(int d = 7; d >= 0; d--) {
      show_colored_int((*((char *)p + n - i) >> d) & 1);
      printf("%c", divider((7 - d) + (i - 1)*8));
    }
  }
  printf("\n");
}


int main(void) {

  float min = (long) 2 * 2048 + 1 + 0.125;
  for(float a = min; a != min * 128; a *= 2) {
    dbl_t d = { a };
    printf("%34.16f\t", d.value);
    show_memory(&(d.value), sizeof d.value);
    printf("\t\t\t\t\t%c %c %7d %23d\n\n",
        (d.sign ? '-' : ' '), 
        (d.degree_sign ? ' ' : '-'), 
        d.degree,
        d.mantiss);
  }

  return 0;
}
