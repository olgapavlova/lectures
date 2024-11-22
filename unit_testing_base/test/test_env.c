#include <check.h>
#include <stdio.h>
#include <stdlib.h>

/* Тестирование среды запуска
   (не требует внешнего модуля)

 * + Тест запущен в оболочке zsh                $SHELL
 * + Глубина вызовов оболочки больше двух       $SHLVL
 * - Версия оболочки больше 5                   $ZSH_VERSION
 * - Тест запущен в русской локали              $LANG
 * - На компьютере работает операционка darwin  $OSTYPE

 */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Создаём функции для тестирования конкретного требования
// Используем макросы библиотеки check


START_TEST(test_env_shell) {
  // : Тест запущен в оболочке zsh
  char* env_shell = getenv("SHELL");
  ck_assert_str_eq(env_shell, "/bin/zsh");  // Только одна проверка!
}
END_TEST

START_TEST(test_env_level) {
  // : Глубина вызовов оболочки больше двух
  char* env_lvl_str = getenv("SHLVL");
  int env_lvl;
  sscanf(env_lvl_str, "%d", &env_lvl);

  ck_assert_int_gt(env_lvl, 1);
}
END_TEST




//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Создаём всю инфраструктуру для тестирования
// SRunner → Suite → TCase → [функция test_*]
SRunner* create_runner() {
  SRunner* sr;
  Suite* s;
  TCase* tc_shell;

  s = suite_create("ENV");
  sr = srunner_create(s);

  tc_shell = tcase_create(" shell ");
  tcase_add_test(tc_shell, test_env_shell);
  tcase_add_test(tc_shell, test_env_level);

  suite_add_tcase(s, tc_shell);

  return sr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Основная функция нужна, как и всегда
int main(void) {
  system("clear");  // Очистить экран

  SRunner* sr = create_runner();
  srunner_run_all(sr, CK_VERBOSE);

  int failed_quantity = srunner_ntests_failed(sr);
  return (failed_quantity == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
