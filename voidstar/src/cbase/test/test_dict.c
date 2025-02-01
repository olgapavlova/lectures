/**
 * @file test_dict.c
 * @brief Тестирование публичного интерфейса библиотеки char-словаря
 */
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../log.h"
#include "../callback.h"
#include "../dict.h"
#include "../str.h"
#include "../regmem.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

static int func_next_steps[4] = {20, 33, 7, 3};
START_TEST(test_func_next) {
  dict d = {NULL};
  void * c = NULL;
  for(int i = 0; i <= func_next_steps[_i]; i++) {
    c = dict_next(&d);
  }
  ck_assert_ptr_eq(c, &d[func_next_steps[_i]]);
} END_TEST

static int func_foreach_sum = 0;
static void func_foreach_cb(int n, void ** cv) {  // тестовая callback-функция
  if(n != 1) { return; }
  if(*(int**)(cv[0]) == NULL) { return; }
  int c = **(int**)cv[0];
  func_foreach_sum += c;
}
static int func_foreach_in[4] = {7, 19, 54, 98};
static int func_foreach_out[4] = {7, 26, 80, 178};
START_TEST(test_func_foreach) {
  func_foreach_sum = 0;
  dict d = {NULL};
  for(int i = 0; i <= _i; i++) { d[i] = &(func_foreach_in[i]); };
  dict_foreach(&d, func_foreach_cb);
  ck_assert_int_eq(func_foreach_sum, func_foreach_out[_i]);
} END_TEST

SRunner* create_runner() {  // создать объект запуска тестов
  SRunner* result;
  Suite* s = suite_create("FEATURES");

  TCase* t_func = tcase_create(" func ");
  tcase_set_tags(t_func, "func");
  suite_add_tcase(s, t_func);
  tcase_add_loop_test(t_func, test_func_next, 0, 4);
  tcase_add_loop_test(t_func, test_func_foreach, 0, 4);

  result = srunner_create(s);
  return result;
}

int main() {  // запустить все тесты
  SRunner* sr = create_runner();
  srunner_run_all(sr, CHECK_LEVEL);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
