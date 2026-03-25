#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "zero.h"

#define TRUE_FILE "tests/true.txt"

#define MAX_STR_LEN 2048

#define PASS_MSG "\x1b[32mPASS\x1b[0m"
#define FAIL_MSG "\x1b[33mFAIL\x1b[0m"

int main(void) {

  // открываем файл с примерами
  FILE * test_f = fopen(TRUE_FILE, "r");
  if(!test_f) { perror("Не удалось открыть файл" TRUE_FILE); exit(1); }

  // проходим по всем строкам файла с примерами
  char line[MAX_STR_LEN] = { '\0' };
  while(fgets(line, sizeof(line), test_f) != NULL) {
    int len = strlen(line);
    if ((len > 0) && (line[len - 1] == '\n')) { line[len - 1] = '\0'; }

    // проверяем результат вызова функции
    bool state = is_json(line);
 
    // выводим результат проверки примера
    printf("\t%s\t%s\n",
              (state == true) ? PASS_MSG : FAIL_MSG,
              line
        );
  } 

  fclose(test_f);

  return 0;
}
