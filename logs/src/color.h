/// Цвет • Заголовочный файл
#ifndef COLOR_H_LOADED
#define COLOR_H_LOADED
#include <stdio.h>
#include <stdlib.h>

/**
 * Формула расчёта цвета
 * BASE_CODE + LIGHT_CORRECTION * [0|1] + [0:MAX_COLOR] + [0|1] * BG_CORRECTION
 */

#define BASE_CODE 30         // Номера цветов в терминале начинаются от 30
#define LIGHT_CORRECTION 60  // Коды светлых тонов на 60 больше кодов тёмных
#define MAX_COLOR 7          // Всего 8 цветов от 0 до 1
#define BG_CORRECTION 10     // Для фона код ещё на 10 больше

#define COLOR_SYM "\033[%dm"  // Шаблон вывода символа цвета
#define SYM_LEN 5             // Длина символа цвета

#define DEFAULT_TXT_COLOR 39  // Вернуть цвет текста к стандарту
#define DEFAULT_BG_COLOR 49   // Вернуть цвет фона к стандарту

int random_zero_or_one();
int random_pure_color_code();
int random_full_color_code();
int random_bg();
int random_txt();
char *color_symbol(int);
char *colored_char(char *, char *, char);

#endif
