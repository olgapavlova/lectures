%{
#include <stdio.h>

static int first = 1;
static int count = 0;

%}

%%

[0-9]+     {
					count++;
					if (first == 1) {
						printf("%s", yytext);
						first = 0;
					} else {
						printf(", %s", yytext);		
					}
			}

.|\n       ;


%%

int main(void) {
	printf("int arr[SIZE] = { ");
	yylex();
	printf(" };\n");
	printf("#define SIZE %d\n", count);
	return 0;
}




