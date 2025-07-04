#include "../src/black.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

// Требования к программе black
// У нас есть black-сложение → TCase
//   - Black-сумма двух чисел больше их реальной суммы
//   - Black-сумма двух чисел не равна сама себе

/*
   + Black-сумма двух чисел больше их реальной суммы
   : Black-сумма двух чисел не равна сама себе
   + Black-разность двух чисел всегда меньше их реальной разности
   : Black-разность двух чисел не равна сама себе
   : Black-умножение совпадает с реальным умножением
   : Black-деление на -1 не определено
 */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Тест-функции

static const int global_a[7] = {0, -1, 18, 34, -53, 90, -2};
static const int global_b[7] = {15, 24, -32, 34, -5, 9, 0};

START_TEST(test_add_more) {
  // : Black-сумма двух чисел больше их реальной суммы
  int a = global_a[_i];
  int b = global_b[_i];
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


  // Act
  int black_sum = black_add(a, b);
  int real_sum = a + b;

  // Assert
  ck_assert_int_gt(black_sum, real_sum);
}
END_TEST

SRunner *create_runner() {
  SRunner *sr;
  Suite *s;
  TCase *tc;

  s = suite_create("FEATURES");
  tc = tcase_create(" add ");
  tcase_add_test(tc, test_add_more);


  TCase* tc_add = tcase_create(" b+ > + ");
  suite_add_tcase(s_feature, tc_add);
  tcase_add_loop_test(tc_add, test_add_more, 0, 6);

  TCase* tc_subtract = tcase_create(" b- < - ");
  suite_add_tcase(s_feature, tc_subtract);
  tcase_add_test(tc_subtract, test_subtract_10_20);

  TCase* tc_multiply = tcase_create(" b* == * ");
  suite_add_tcase(s_feature, tc_multiply);
  tcase_add_test(tc_multiply, test_multiply_10_20);

  TCase* tc_divide = tcase_create(" b/-1 !!! ");
  suite_add_tcase(s_error, tc_divide);
  tcase_add_test(tc_divide, test_divide_minusone);

  sr = srunner_create(s_feature);
  srunner_add_suite(sr, s_error);

  return sr;
}

int main() {
  SRunner *sr = create_runner();
  srunner_run_all(sr, CK_VERBOSE);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
