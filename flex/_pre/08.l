%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>

static int prepre = 1;
static int pre = 0;
static int current = 0;
static int goal = 0;
static int all = 0;

%}

%%

[0-9]+      {
				all++;
				current = atoi(yytext);
				int result = -1;
				printf("%d + %d = ", pre, prepre);
				fflush(stdout);
				scanf("%d", &result);
				if (result == current) {
					printf("Да, всё верно, ура!\n");
					goal++;
				} else {
					printf("Ой, нет. На самом деле %d + %d = %d\n", pre, prepre, current);
				}
				prepre = pre; pre = current;
			}
.|\n        ;                            

%%

int main(void) {

	FILE * fin = fopen("fibonacci.txt", "r");
	yyin = fin;

    yylex(); 
	printf("\nВаш счёт в игре:\t%d из %d\n\n", goal, all);

	fclose(fin);
    return 0;
}
