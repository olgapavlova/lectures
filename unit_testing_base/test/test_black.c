#include "../src/black.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

// Требования к программе black
// У нас есть black-сложение → TCase
//   - Black-сумма двух чисел больше их реальной суммы
//   - Black-сумма двух чисел не равна сама себе

START_TEST(test_add_more) {
  // Black-сумма двух чисел больше их реальной суммы
  // Arrange
  int a = 10;
  int b = 20;

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

  suite_add_tcase(s, tc);
  sr = srunner_create(s);

  return sr;
}

int main() {
  SRunner *sr = create_runner();
  srunner_run_all(sr, CK_VERBOSE);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
