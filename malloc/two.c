#include "config.h"

int main(int argc, char ** argv) {

  // размер массива
  if(argc != 3) { abort(); }
  size_t arr_h = atoi(argv[1]); 
  size_t arr_w = atoi(argv[2]); 
  if((arr_h == 0) || (arr_w == 0)) { abort(); }

  // бронируем память
  double ** mem = malloc(arr_h * sizeof(double *) + arr_w * arr_h * sizeof(double));

  for(size_t h = 0; h < arr_h; h++) {
    mem[h] = (double *)(&mem[arr_h]) + h * arr_w;
    for(size_t w = 0; w < arr_w; w++) {
      mem[h][w] = h + 1.1;
    }
  }

  // освобождаем память
  free(mem);

  return EXIT_SUCCESS;
}
