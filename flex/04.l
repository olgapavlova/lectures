%{
#include <stdio.h>
#include <stdlib.h>

static int fib = 1;
static int prepre = 1;
static int pre = 0;
static int current = 0;
%}

%%

[0-9]+      {
				current = atoi(yytext);
				fib &= (prepre + pre == current);
				prepre = pre; pre = current;
			}
.|\n        ;                            

%%

int main(void) {
    yylex(); 
	printf("Следующий элемент:\t%d\n", pre + prepre);
    return 0;
}
