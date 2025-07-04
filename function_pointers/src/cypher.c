#include "cypher.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Игра «Доведи до нуля»
 *
 * Система выбирает случайное число.
 * С помощью символов можно делать с этим числом _какие-то_ операции.
 * Каждый символ можно использовать только один раз.
 * Цель — получить 0 на выходе.
 * В некоторых случаях система «отказывается продолжать».
 * В некоторых случаях система сдаётся.
 * Если получили -1, игра начинается заново.
 * Если вышли за MAX_GUESS, превращаем в остаток от деления на MAX_GUESS.
 * Код написан нестрого, всякие проверки нас не интересуют.
 */
int main() {

  // Инициация игры
  srand(time(NULL));
  int (*o_char[128])(int) = {};
  for (int i = 32; i < 128; ++i) {
    o_char[i] = o[rand_o()];
  }

  // Начинаем игру
  begin();

  // Играем
  char c = '\0';
  while ((scanf(" %c", &c) == 1) && (c != 'q')) {
    if (char_used(c)) {
      step_not_allowed(c);
    } else {
      make_step(o_char[c]);
    }
  }
}

// TODO Выбор случайного типа хода
void make_step(operation oo) {
  void (*decision)(operation);

  int des = rand() % 3;

  if (!des) {
    decision = real_step;
  } else {
    decision = fake_step;
  }

  decision(oo);
}

void fake_step(operation oo) {
  if (oo == NULL) {
    return;
  }

  skip();
}

void skip() { printf("%s", "Не хочу делать шаг :(\n\n> "); }

// Делаем настоящий шаг
void real_step(operation oo) {
  static int guess_me = -1;
  if (oo == NULL) {
    return;
  }
  guess_me = (guess_me == -1) ? rand() % 1000 : oo(guess_me);
  guess_me = (guess_me > MAX_GUESS) ? guess_me % MAX_GUESS : guess_me;
  (guess_me == 0) ? win() : cont(guess_me);
}

// Сообщаем о победе и выходим из игры
void win() {
  printf("\n\n%s\n\n", "* * * * * Вы выиграли! Это победа! * * * * *");
  exit(EXIT_SUCCESS);
}

// Продолжаем играть
void cont(int value) { printf("Новое значение: %d\n\n> ", value); }

// Ход запрещён
void step_not_allowed(char c) { printf("Команду %c вы уже использовали\n", c); }

// Начинаем игру
void begin() {
  printf(
      "\nНачинаем.\nЯ загадал число от 0 до %d.\nКаждый знак из ASCII-таблицы "
      "что-то с этим числом делает.\nНекоторые знаки-команды совпадают, "
      "некоторые отличаются.\nКаждый знак-команду можно использовать только "
      "один раз.\nВводите знаки, и я буду менять число (и показывать "
      "вам).\nВаша задача — добраться до нуля.\nq — выход из игры.\n\n> ",
      MAX_GUESS);
}

// Записываем использованные команды
int char_used(char c) {
  static int commands[128] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int result = 0;
  (commands[c] == 0) ? (commands[c] = 1) : (result = 1);
  return result;
}
