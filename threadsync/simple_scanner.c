#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "config.h"

/// Сканировать конкретный порт на конкретном IP
int scan_one_host_one_port(const char *, const int);

/// Посчитать диапазон адресов по разобранной маске

/// Очередь IP-адресов

/// Собираем живых
struct queue {
  int len;
  char *address[255];
};

int main(void) {
  // Считываем описания портов
  char *port_description_file = "ports.config";
  portlist_t pl = {0};
  read_portlist_from_file(&pl, port_description_file);

  for (int i = 0; i < pl.len; i++) {
    printf("%5d\t%s\t%s\n", pl.data[i].number, pl.data[i].protocol,
           pl.data[i].description);
  }

  // Маска подсети для сканирования
  char mask[] = "192.168.0.1/24";
  // Тут нет потокобезопасности, кстати
  char *ip = strtok(mask, "/");
  int len = atoi(strtok(NULL, "/"));

  // Определяем диапазон адресов сети
  struct in_addr a;
  inet_pton(AF_INET, ip, &a);
  uint32_t ipl = ntohl(a.s_addr);
  uint32_t msk = 0xFFFFFFFF << (32 - len);  // просто определение маски
  uint32_t ntw = ipl & msk;                 // определение сети
  uint32_t brd = ntw | ~msk;                // определение сети

  // Все адреса подсети
  char buffer[INET_ADDRSTRLEN] = {'\0'};
  for (uint32_t h = ntw + 1; h < brd; h++) {
    struct in_addr ip_a = {0};
    ip_a.s_addr = htonl(h);  // сетевой порядок байтов
    inet_ntop(AF_INET, &ip_a, buffer, INET_ADDRSTRLEN);

    int alive = scan_one_host_one_port(buffer, pl.data[0].number);
    if (!alive) {
      // Добавить массив для работы с очередью
      printf("%s\t%d\n", buffer, alive);
    }
  }

  return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/// Сканировать конкретный порт на конкретном IP
int scan_one_host_one_port(const char *ip, const int port) {
  // Создать дескриптор сокета (вся работа с сетью — через него)
  int sckt = socket(AF_INET, SOCK_STREAM, 0);
  if (sckt < 0) {
    exit(EXIT_FAILURE);
  }

  // Создать структуру, описывающую адрес, который будем сканировать
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));            // забьём нулями, а то мало ли
  addr.sin_family = AF_INET;                 // TCP/IP
  addr.sin_port = htons(port);               // а вот и big/little endian
  inet_pton(AF_INET, ip, &(addr.sin_addr));  // преобразуем IP в бинарный формат

  // Попытка связи и возврат результата
  int result = connect(sckt, (struct sockaddr *)&addr, sizeof(addr));
  shutdown(sckt, 2);
  return result;
}

/// Итератор IP-адресов по маске подсети
/*
void next_ip(void * mask, char * ip) {
  static _mask = NULL;
  if (_mask == NULL) { _mask = mask; }

}
*/
