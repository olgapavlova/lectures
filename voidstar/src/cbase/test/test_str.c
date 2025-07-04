/**
 * @file test_str.c
 * @brief Тестирование публичного интерфейса библиотеки объектных строк
 */
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../log.h"
#include "../regmem.h"
#include "../str.h"
#include "../callback.h"

#undef LOG_LVL
#define LOG_LVL LOG_LVL_ERROR

/*
START_TEST(test_manage_init) {  // создать пустую строку
  str* s = str_init();
  ck_assert_str_eq(s->data, "");
}
END_TEST
*/

static char* func_create_in[4] = {"", "abra", "abra cadabra", "w\now"};
static char* func_create_out[4] = {"", "abra", "abra cadabra", "w\now"};
START_TEST(test_func_create) {  // создать строку из другой строки
  str s = str_create(func_create_in[_i]);
  ck_assert_str_eq(s, func_create_out[_i]);
}
END_TEST

static char* func_next_in[4] = {"a", "abra", "abra cadabra", "w\now"};
static int func_next_steps[4] = {1, 3, 5, 2};
static char func_next_out[4] = {'a', 'r', ' ', '\n' };
START_TEST(test_func_next) {
  str s = str_create(func_next_in[_i]);
  char * c = NULL;
  for(int i = 0; i < func_next_steps[_i]; i++) { c = str_next(s); }
  ck_assert_int_eq(*c, func_next_out[_i]);
} END_TEST

static int func_foreach_sum = 0;
static void func_foreach_cb(int n, void ** cv) {
  if(n != 1) { return; }
  char *c = (char*)(cv[0]);
  func_foreach_sum += (int)*c;
}
static char* func_foreach_in[4] = {"a", "abra", "abra cadabra", "w\now"};
static int func_foreach_out[4] = {97, 406, 1140, 359};
START_TEST(test_func_foreach) {
  func_foreach_sum = 0;
  str s = str_create(func_foreach_in[_i]);
  str_foreach(s, func_foreach_cb);
  ck_assert_int_eq(func_foreach_sum, func_foreach_out[_i]);
} END_TEST

static char * func_copy_in[4] = { "21", "bla\nbla", "wow wow", "q" };
START_TEST(test_func_copy_char) {
  str s = str_create("");
  str_copy(func_copy_in[_i][0], &s);
  ck_assert_int_eq(s[0], func_copy_in[_i][0]);
  ck_assert_int_eq(s[1], '\0');
} END_TEST
START_TEST(test_func_copy_str) {
  str s = str_create("");
  str_copy(func_copy_in[_i], &s);
  ck_assert_str_eq(s, func_copy_in[_i]);
} END_TEST


static char * func_is_empty_in[4] = { "1", "", "bla", "\n" };
static bool func_is_empty_out[4] = { false, true, false, true };
START_TEST(test_func_is_empty) {
  str s = str_create(func_is_empty_in[_i]);
  ck_assert_int_eq(str_is_empty(s), func_is_empty_out[_i]);
} END_TEST

static char * func_drop_n_in[4] = { "\n", "bla\nbla", "bla\n", "wow" };
static char * func_drop_n_out[4] = { "", "bla\nbla", "bla", "wow" };
START_TEST(test_func_drop_n) {
  str s = str_create(func_drop_n_in[_i]);
  str result = str_drop_n(s);
  ck_assert_str_eq(result, func_drop_n_out[_i]);
} END_TEST

static char * func_crop_in_str[4] = { "qwerty", "a b c d e f g", "param", "welcome" };
static int func_crop_in_from[4] = { 0, 1, 2, 3 };
static int func_crop_in_to[4] = { 2, 4, 6, 8 };
static char * func_crop_out[4] = { "qw", " b ", "ra", "com" };
START_TEST(test_func_crop) {
  str s = str_create(func_crop_in_str[_i]);
  str result = str_crop(s, func_crop_in_from[_i], func_crop_in_to[_i]);
  ck_assert_str_eq(result, func_crop_out[_i]);
} END_TEST

static char * func_add_in_s[4] = { "a", "qwerty", "", "bla bla" };
static char * func_add_in_end[4] = { "b", "123456", "wow", "" };
static char * func_add_out[4] = { "ab", "qwerty123456", "wow", "bla bla" };
START_TEST(test_func_add) {
  str s = str_create(func_add_in_s[_i]);
  str end = str_create(func_add_in_end[_i]);
  str_add(&s, &end);
  ck_assert_str_eq(s, func_add_out[_i]);
} END_TEST

static char func_fill_in_c[4] = { 'a', ' ', '\t', '7' };
static int func_fill_in_n[4] = { 3, 5, 1, 8 };
static char * func_fill_out[4] = { "aaa", "     ", "\t", "77777777" };
START_TEST(test_func_fill) {
  str s = str_create(" ");
  str_fill(&s, func_fill_in_c[_i], func_fill_in_n[_i]);
  ck_assert_str_eq(s, func_fill_out[_i]);
} END_TEST

SRunner* create_runner() {  // создать объект запуска тестов
  SRunner* result;
  Suite* s = suite_create("FEATURES");

  TCase* t_func = tcase_create(" func ");
  tcase_set_tags(t_func, "func");
  suite_add_tcase(s, t_func);
  tcase_add_loop_test(t_func, test_func_create, 0, 4);
  tcase_add_loop_test(t_func, test_func_next, 0, 4);
  tcase_add_loop_test(t_func, test_func_foreach, 0, 4);
  tcase_add_loop_test(t_func, test_func_copy_char, 0, 4);
  tcase_add_loop_test(t_func, test_func_copy_str, 0, 4);
  tcase_add_loop_test(t_func, test_func_is_empty, 0, 4);
  tcase_add_loop_test(t_func, test_func_drop_n, 0, 4);
  tcase_add_loop_test(t_func, test_func_crop, 0, 4);
  tcase_add_loop_test(t_func, test_func_add, 0, 4);
  tcase_add_loop_test(t_func, test_func_fill, 0, 4);

  /*
  tcase_add_test(t_create, test_manage_init);
  tcase_add_loop_test(t_manage, test_manage_create_int, 0, 4);
  tcase_add_loop_test(t_manage, test_manage_create_char, 0, 4);
  tcase_add_test(t_manage, test_manage_destroy);
  tcase_add_loop_test(t_manage, test_cmp_char, 0, 4);
  tcase_add_loop_test(t_manage, test_cmp_str, 0, 4);
  tcase_add_loop_test(t_manage, test_to_str, 0, 4);
  */
  result = srunner_create(s);
  return result;
}

int main() {  // запустить все тесты
  SRunner* sr = create_runner();
  srunner_run_all(sr, CHECK_LEVEL);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
