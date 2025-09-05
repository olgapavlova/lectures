%{
#include <stdio.h>
#include <stdlib.h>

static int pre = 0;
static int prepre = 1;

static int fib = 1;

%}

%%

[0-9]+     {
				int current = atoi(yytext);
				fib &= (current == pre + prepre);
				prepre = pre; pre = current;
			}

.|\n       ;


%%

int main(void) {
	yylex();
	printf("Фибоначчи?\t%s\n", (fib == 1) ? "да" : "нет" );
	return 0;
}

