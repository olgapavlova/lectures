%{
#include <stdio.h>

static int count = 0;

%}

%%

[0-9]+      {
				count++;
				printf("%s, ", yytext);
			}
.|\n        ;                            

%%

int main(void) {
	printf("int arr[] = { ");
    yylex();
	printf(" }; // %d\n", count);
    return 0;
}
