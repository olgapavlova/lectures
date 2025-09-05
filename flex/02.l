%{
#include <stdio.h>
#include <stdlib.h>

static int sum = 0;

%}

%%

[0-9]+     {
				sum += atoi(yytext);
			}

.|\n       ;


%%

int main(void) {
	yylex();
	printf("Сумма чисел:\t%d\n", sum);
	return 0;
}




