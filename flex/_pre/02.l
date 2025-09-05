%{
#include <stdio.h>
#include <stdlib.h>

static int sum = 0;
%}

%%

[0-9]+      { sum += atoi(yytext); }
.|\n        ;                            

%%

int main(void) {
    yylex();   /* запускаем сканер */
	printf("Сумма:\t%d\n", sum);
    return 0;
}
