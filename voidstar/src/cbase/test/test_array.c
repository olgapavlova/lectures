/**
 * @file test_array.c
 * @brief Тестирование публичного интерфейса библиотеки динамического массива указателей
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
#include "../array.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

START_TEST(test_func_init) {
  array a = array_init();
  ck_assert_ptr_eq(a[0], ARRAY_LAST);
} END_TEST


START_TEST(test_func_len_empty) {
  array a = array_init();
  int a_len = array_len(&a);
  ck_assert_int_eq(a_len, 0);
} END_TEST


static char * func_add_in[4] = { "a", "bcd", "wow", "qqq" };
START_TEST(test_func_add) {
  array a = array_init();
  for(int i = 0; i<= _i; i++) {
    str line = str_create(func_add_in[i]);
    array_add(&a, line);
  }
  ck_assert_str_eq((char *)a[_i], func_add_in[_i]);
} END_TEST


static int func_next_steps[4] = {20, 33, 7, 3};
START_TEST(test_func_next) {
  D("Тестируем next");
  array a = array_init();
  int d = _i;
  for(int i = 0; i < func_next_steps[_i]; i++) {
    array_add(&a, &d);
  }

  void ** c = NULL;
  for(int i = 0; i < func_next_steps[_i]; i++) {
    c = array_next(&a);
  }

  ck_assert_int_eq(**(int**)c, d);
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
  array a = array_init();
  for(int i = 0; i <= _i; i++) { array_add(&a, &(func_foreach_in[i])); };
  array_foreach(&a, func_foreach_cb);
  ck_assert_int_eq(func_foreach_sum, func_foreach_out[_i]);
} END_TEST

SRunner* create_runner() {  // создать объект запуска тестов
  SRunner* result;
  Suite* s = suite_create("FEATURES");

  TCase* t_func = tcase_create(" func ");
  tcase_set_tags(t_func, "func");
  suite_add_tcase(s, t_func);
  tcase_add_test(t_func, test_func_init);
  tcase_add_test(t_func, test_func_len_empty);
  tcase_add_loop_test(t_func, test_func_add, 0, 4);
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
