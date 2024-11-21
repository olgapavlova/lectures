#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/black.h"

/* Тестируем чёрный ящик
   ../build/black
   ../src/black.c
   ../src/black.h

   + Black-сумма двух чисел больше их реальной суммы
   : Black-сумма двух чисел не равна сама себе
   + Black-разность двух чисел всегда меньше их реальной разности
   : Black-разность двух чисел не равна сама себе
   : Black-умножение совпадает с реальным умножением
   : Black-деление на -1 не определено
 */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Тест-функции

START_TEST(test_add_10_20) {
  // : Black-сумма двух чисел больше их реальной суммы
  int a = 10;
  int b = 20;
  ck_assert_int_gt(black_add(a, b), a + b);
}
END_TEST

START_TEST(test_subtract_10_20) {
  // : Black-разность двух чисел всегда меньше их реальной разности
  int a = 10;
  int b = 20;
  ck_assert_int_lt(black_subtract(a, b), a - b);
}
END_TEST

START_TEST(test_multiply_10_20) {
  // : Black-умножение совпадает с реальным умножением
  int a = 10;
  int b = 20;
  ck_assert_int_eq(black_multiply(a, b), a * b);
}
END_TEST

START_TEST(test_divide_minusone) {
  // : Black-деление на -1 не определено
  ck_assert_int_eq(black_divide_allowed(-1), 0);
}
END_TEST

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Инфраструктура тестирования

SRunner* create_runner() {
  SRunner* sr;

  Suite* s_feature = suite_create("FEATURES");
  Suite* s_error = suite_create("ERRORS");

  TCase* tc_add = tcase_create("Black add > +\t");
  suite_add_tcase(s_feature, tc_add);
  tcase_add_test(tc_add, test_add_10_20);

  TCase* tc_subtract = tcase_create("Black substract < -\t");
  suite_add_tcase(s_feature, tc_subtract);
  tcase_add_test(tc_subtract, test_subtract_10_20);

  TCase* tc_multiply = tcase_create("Black multiply == *\t");
  suite_add_tcase(s_feature, tc_multiply);
  tcase_add_test(tc_multiply, test_multiply_10_20);

  TCase* tc_divide = tcase_create("Black divider -1 does not work\t");
  suite_add_tcase(s_error, tc_divide);
  tcase_add_test(tc_divide, test_divide_minusone);

  sr = srunner_create(s_feature);
  srunner_add_suite(sr, s_error);

  return sr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Основная функция
int main() {
  system("clear");

  SRunner* sr = create_runner();
  srunner_run_all(sr, CK_VERBOSE);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
