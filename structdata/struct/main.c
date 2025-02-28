#include "main.h"

// Создадим поле
static path game = {0};

// Нажатая клавиша
char get_key() {
  struct termios oldt, newt;
  char result;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  read(STDIN_FILENO, &result, 1);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return result;
}

// Показать, где мы
void show_state(state w) {

  int position = w.position;
  int done = w.done;

  if (position) {
  printf("      ");
  for(int i = 0; i < PATH_LEN; i++) {
    if(i == position) { printf("  \e[90m|\e[0m"); } else { printf("   "); }
  }

  }

  printf("\n");

  char * tmpl_regular = " \e[90m%2d\e[0m";
  char * tmpl_here = " \e[33m%2d\e[0m";
  char * tmpl_back = " \e[31m%2d\e[0m";
  char * tmpl_forward = " \e[32m%2d\e[0m";
  char * p = NULL;

  printf("[ %d ] ", done);
  for (int i = 0; i < PATH_LEN; i++) {
    if(i == position) { p = tmpl_here; }
    else if(i == position - game[position].back) { p = tmpl_back; }
    else if(i == position + game[position].forward) { p = tmpl_forward; }
    else { p = tmpl_regular; };
    printf(p, i);
  }
  printf("\n");

}

// Сделать шаг
static void make_step(state * w) {
    w->position += w->step;
    w->done++;
}

// Подготовить игровое поле
static void prepare_gameplay(void) {
  printf("\033[?25l");
  fflush(stdout);
  system("clear");
}

int main(void) {
  prepare_gameplay();

  // Заполним поле шагами
  srand(time(NULL));
  for (int i = 0; i < PATH_LEN; i++) {
    int forward = (rand() % MAX_STEP) + 1;
    int back = (rand() % MAX_STEP) + 1;
    forward = (forward > PATH_LEN - i) ? (PATH_LEN - i)
                                       : forward;  // вперёд не дальше конца
    back %= i;  // назад не ближе начала
    if (i == 0) {
      back = 0;
    }  // с первой точки никуда назад нельзя
    game[i] = (cell){ .forward = forward, .back = back };
  }

  // Встаём на первую клетку
  state whereami = {
    .position = 0,
    .done = 0,
    .step = 0 };

  do {
    // Проверяем, не дошли ли мы до конца
    if (whereami.position >= PATH_LEN - 1) {
      break;
    }

    // Показываем, где мы
    show_state(whereami);

    // В зависимости от нажатой клавиши делаем шаг
    char c = get_key();
    switch (c) {
      case ' ': { whereami.step = (rand() % 2) ? game[whereami.position].forward : (-1) * game[whereami.position].back; break; }
      default: { whereami.step = 1; break; }
    }

    // Делаем ход
    make_step(&whereami);

    // И так в пределах STEPS шагов
  } while (whereami.done < STEPS);

  if (whereami.done < STEPS) {
    // Если дошли до конца — победа
    printf("\n * * * * * * *  You win! * * * * * *\n\n");
  } else {
    // Если не дошли — проигрыш
    printf("\n :( :( :( :(  You lost  ): ): ): ):\n\n");
  }

  return EXIT_SUCCESS;
}
