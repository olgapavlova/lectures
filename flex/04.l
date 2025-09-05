%{
#include <stdio.h>
#include <stdlib.h>

static int pre = 0;
static int current = 0;
static int next = 0;

%}

%%

[0-9]+     {
				int current = atoi(yytext);
				next = pre + current;
				pre = current;
			}

.|\n       ;


%%

int main(void) {
	yylex();
	printf("Следующее:\t%d\n", next);
	return 0;
}

