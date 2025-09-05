%{
#include <stdio.h>

static int line = 1;
static int col  = 1;
static int tline = 1;
static int tcol  = 1;

#define YY_USER_ACTION do {                             \
        tline = line; tcol = col;                       \
        for (int i = 0; i < yyleng; i++) {              \
            if (yytext[i] == '\n') { line++; col = 1; } \
            else { col++; }                             \
        }                                               \
    } while (0);

%}

%%

[0-9]+      {
				printf("%3d / %3d : %4s\n", tline, tcol, yytext);
			}
.|\n        ;                            

%%

int main(void) {
    yylex();
    return 0;
}
