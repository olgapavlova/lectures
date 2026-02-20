#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Нам нужна структура для сохранения данных
typedef struct memory_s {
  char * response;
  size_t size;
} memory_t;
 
// И ещё нам нужен callback для обработки очередного куска пришедших данных
static size_t cb(char * chunkdata, size_t size, size_t chunksize, void * storage)
{
  // Вообще-то size == 1 всегда

  // Указателю нужен тип, а то какие уж там операции
  memory_t * mem = (memory_t *)storage;
 
  // Выделить место под старый + новый куски данных + '\0'
  char * newdata = realloc(mem->response, mem->size + chunksize * size + 1);
  if(!newdata) return 0;  // ничего не добавили
 
  mem->response = newdata;       // обновляем указатель
  memcpy(&(mem->response[mem->size]), chunkdata, chunksize); // запись в конец
  mem->size += chunksize;        // заняли больше места
  mem->response[mem->size] = 0;  // ноль в конце
 
  printf("%s\n", chunkdata);

  return chunksize;
}

int main(void) {

  // Как всегда, первым делом инициализация монстра
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Выясним доступные нам протоколы
  curl_version_info_data * c = curl_version_info(CURLVERSION_NOW);
  int pc = 0;
  char ** p = (char **)(c->protocols);
  printf("\n---------------\n");
  while(*p != NULL) {
    printf("%s ", *p);
    p++; pc++;
    if(pc > 7) { printf("\n"); pc = 0; }
  }
  printf("\n---------------\n");

  // Теперь нам нужно кольцо всевластия
  CURL * ch = curl_easy_init();
  if(!ch) { perror("Что-то не так с handle"); }

  // Подготовим машинку к работе
#if 0
  FILE * cf = fopen("downloaded.txt", "w");
  if(!cf) { perror("Не удалось открыть файл для сохранения данных"); }
#endif

  memory_t cdata = { 0 };
  cdata.response = malloc(1);

  curl_easy_setopt(ch, CURLOPT_URL, "https://sobakapav.ru/listMarkets.json");
  // curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)cf);  // для файла
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)&cdata);  // для файла
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(ch, CURLOPT_HEADER, 1L);

  // Работай!
  CURLcode result = curl_easy_perform(ch);
  if(result != CURLE_OK) { perror("Запрос не обработан"); }
  printf("\n");

  // Убираем за собой
  curl_easy_cleanup(ch);
  curl_global_cleanup();

  return 0;
}
