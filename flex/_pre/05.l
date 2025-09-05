%option yylineno

%{
#include <stdio.h>

static int first = 1;
%}

%%

[0-9]+      {
				printf("%3d : %4s\n", yylineno, yytext);
			}
.|\n        ;                            

%%

int main(void) {
    yylex();
    return 0;
}
