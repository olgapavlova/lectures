#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define PATH_LEN 10
#define MAX_STEP 5
#define STEPS 7

// Клетка игрового поля
typedef struct cell cell;
struct cell {
	int n;       // номер клетки // TODO
	int forward; // шаги вперёд
	int back;    // шаги назад
};

// Игровое поле
typedef cell path[PATH_LEN];

// Состояние игры
typedef struct state state;
struct state {
	int position;  // на какой клетке сейчас
	int step;  	   // какой следующий шаг
	int done;      // сколько уже было шагов
};
