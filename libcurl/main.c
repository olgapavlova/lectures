#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory {
  char * response;
  size_t size;
};
 
static size_t cbf(char * newchunk, size_t always_one, size_t chunksize,
    void * olddata) {
  chunksize *= always_one;

  struct memory * mem = (struct memory *)olddata;
 
  char * newplace = realloc(mem->response, mem->size + chunksize + 1);
  if(!newplace) return 0;  // не удалось довыделить память 
 
  mem->response = newplace;
  memcpy(&(mem->response[mem->size]), newchunk, chunksize);
  mem->size += chunksize;
  mem->response[mem->size] = '\0';
 
  return chunksize;
}

int main(void) {

  // Включаем монстра
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Узнаем, что включили-то?
  curl_version_info_data * cd = curl_version_info(CURLVERSION_NOW);
  if(!cd) perror("Не дал инфы");

#if 0
  char ** p = (char **)(cd->protocols);
  while(*p != NULL) {
    printf("%s ", *p);
    p++;
  }
#endif

  // Хотим пойти в конкретное место
  CURL * ch = curl_easy_init();

  // Основная часть
  curl_easy_setopt(ch, CURLOPT_URL, "https://sobakapav.ru/listMarkets.json");

#if 0
  FILE * cf = fopen("download.txt", "w");
  if(!cf) perror("А нету файла");
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, cf);
#endif

  struct memory chunk = { 0 };
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, cbf);
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(ch, CURLOPT_HEADER, 1L);

  CURLcode result = curl_easy_perform(ch);
  if (result != CURLE_OK) { perror("Не удалось сходить по адресу"); }


  printf("%s\n", chunk.response);

  // Уходим
  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return 0;
}
