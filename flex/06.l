%{
#include <stdio.h>

static int line = 1;
static int col = 1;
static int tline = 1;
static int tcol = 1;

#define YY_USER_ACTION do { \
	tline = line; tcol = col;	\
	if(yytext[0] == '\n') { line++; col = 1; } \
	else { \
			for (int i = 0; yytext[i] != '\0'; i++) { col++; } \
	} \
	} while(0);

%}

%%

[0-9]+     {
			printf("%d / %d\t:\t%4s\n", tline, tcol, yytext);		
			}

.|\n       ;


%%

int main(void) {
	yylex();
	printf("\n");
	return 0;
}




