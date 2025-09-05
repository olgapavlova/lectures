%{
#include <stdio.h>

static int first = 1;
%}

%%

[0-9]+      { if (first) { first = 0; printf("%s", yytext); } else { printf(" %s", yytext); } }
.|\n        ;                            

%%

int main(void) {
    yylex();   /* запускаем сканер */
	printf("\n");
    return 0;
}
