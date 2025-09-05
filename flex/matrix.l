%{
#include <stdio.h>
#include <stdlib.h>

static int cols = 0;     // число колонок по первой строке
static int ccol = 0;     // сколько чисел уже в строке
static int rows = 0;     // число строк

static int first = 1;    // первая строка
%}

%%

[0-9.]+      {   // нашли число
                if (rows == 0) { cols++; }

                if (ccol == 0) { printf("  { %s", yytext);
                } else		   { printf(", %s", yytext); }
                ccol++;
            }

\n          {   // конец строки
                if (ccol > 0) {  // в строке были данные
                    printf(" },\n");
                    rows++; ccol = 0;
                }
            }

[ \t]+      ;   // табы и пробелы нам ни к чему
.           ;   // и всё остальное тоже

%%

int main(void) {
    printf("double data[ROWS][COLS] = { \n");
    yylex();

	// закроем незакрытое, если в конце последней строки нет \n
    if (ccol > 0) {
        printf(" },\n");
        rows++;
    }

    printf(" };\n");

	// узнаём позже, зато при компиляции кода активируются раньше — фокус!
    printf("#define ROWS %d\n", rows);
    printf("#define COLS %d\n", cols);

    return 0;
}
