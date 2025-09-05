%{
#include <stdio.h>
%}

%%

[0-9]+     {
			printf("%s ", yytext);		
			}

.|\n       ;


%%

int main(void) {
	yylex();
	printf("\n");
	return 0;
}




