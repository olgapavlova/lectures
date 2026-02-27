#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parson.h"

#define GOOGLE_SCOPE "https://www.googleapis.com/auth/youtube.readonly"

int main(void) {

  // Как всегда, первым делом инициализация монстра
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Теперь нам нужно кольцо всевластия
  CURL * ch = curl_easy_init();
  if(!ch) { perror("Что-то не так с handle"); }

  // Подготовим машинку к работе
  FILE * cf = fopen("params.txt", "w");
  if(!cf) { perror("Не удалось открыть файл для сохранения данных"); }

  // Идём в Google
  curl_easy_setopt(ch, CURLOPT_URL, "https://oauth2.googleapis.com/device/code");

  // Нам нужны два параметра запроса — «Вот я» и «Токен мне нужен вот сюда»
  char * client_id = getenv("GOOGLE_CLIENT_ID");  // вот я
  char scope[] = GOOGLE_SCOPE;                    // с токеном пойду сюда

  char params[1000] = {'\0'};
  sprintf(params, "client_id=%s&scope=%s", client_id, scope);  // собираем параметры в строку

  curl_easy_setopt(ch, CURLOPT_POSTFIELDS, params);

  // Сохранять будем в файл — чтобы научиться читать JSON-файлы, это полезно
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)cf);

  // Ну и, конечно, POST-метод, как положено
  curl_easy_setopt(ch, CURLOPT_POST, 1L);

  // Работай!
  CURLcode result = curl_easy_perform(ch);
  if(result != CURLE_OK) { perror("Запрос не обработан"); }
  printf("\n");

  fclose(cf);

  // Вытаскиваем данные из файла
  JSON_Value * params_json = json_parse_file("params.txt");
  JSON_Object * params_obj = json_value_get_object(params_json);
  //const char * device_code = json_object_get_string(params_obj, "device_code");
  int expires_in = (int)json_object_get_number(params_obj, "expires_in");
  int interval = (int)json_object_get_number(params_obj, "interval");
  const char * user_code = json_object_get_string(params_obj, "user_code");
  const char * verification_url = json_object_get_string(params_obj, "verification_url");

  // Отправим пользователя по адресу
  printf("Идите на %s\nи введите там %s\n", verification_url, user_code);

  // Пинаем сервер в ожидании результата
  int timer = 0;
  int ok = 0;
  printf("%d %d\n", expires_in, interval);
  while ((timer < expires_in) && (ok != 1)) {
    printf("Запрос через %5d секунд\n", timer);
    sleep(interval);
    timer += interval;
  }

  // Убираем за собой
  curl_easy_cleanup(ch);
  curl_global_cleanup();

  return 0;
}
