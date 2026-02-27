#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parson.h"

#define PARAM_FILE "param.json"
#define GOOGLE_SCOPE "https://www.googleapis.com/auth/youtube.readonly"

int main(void) {

  // Включаем монстра
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Узнаем, что включили-то?
  curl_version_info_data * cd = curl_version_info(CURLVERSION_NOW);
  if(!cd) perror("Не дал инфы");

  // Хотим пойти в конкретное место
  CURL * ch = curl_easy_init();

  // Основная часть
  curl_easy_setopt(ch, CURLOPT_URL, "https://oauth2.googleapis.com/device/code");

  FILE * cf = fopen(PARAM_FILE, "w");
  if(!cf) perror("А нету файла");
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, cf);

  curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)cf);
  curl_easy_setopt(ch, CURLOPT_POST, 1L);

  
  char param_request[1000] = {'\0'};
  
  char * client_id = getenv("GOOGLE_CLIENT_ID");

  sprintf(param_request, "client_id=%s&scope=" GOOGLE_SCOPE, client_id);

  curl_easy_setopt(ch, CURLOPT_POSTFIELDS, param_request);

  CURLcode result = curl_easy_perform(ch);
  if (result != CURLE_OK) { perror("Не удалось сходить по адресу"); }

  fclose(cf);

  JSON_Value * param_result = json_parse_file(PARAM_FILE);
  if (!param_result) { perror("JSON-файл не удалось прочесть"); }

  JSON_Object * param_obj = json_object(param_result);
  if (!param_obj) { perror("JSON-объект не удалось создать"); }

  const char * user_code = json_object_get_string(param_obj, "user_code");
  const char * verification_url = json_object_get_string(param_obj, "verification_url");
  printf("Иди сюда: %s\nВводи это: %s\n", verification_url, user_code);


  // Уходим
  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return 0;
}
