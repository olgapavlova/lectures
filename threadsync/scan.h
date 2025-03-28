#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "config.h"

// Максимальная длина статической строки (напишем просто)
#undef STRL
#define STRL 1000

// Сколько потоков заводим
#define TNUM 4

// Значения по умолчанию
#define SUBNET "192.168.0.1/24"
#define PORTSFILE "ports.config"
#define HOSTLIST 256

// Маска подсети (тип)
typedef struct mask_t {
  char ip[INET_ADDRSTRLEN];
  int len;
  uint32_t network;
  uint32_t bandwidth;
} mask_t;

// Хост (тип)
typedef struct host_t {
  char ip[INET_ADDRSTRLEN];
  bool pl[PORTLIST_LEN];
} host_t;

// Список хостов (тип)
typedef struct hostlist_t {
  int len;
  host_t data[HOSTLIST];
} hostlist_t;

// Сеть (тип)
typedef struct net_t {
  mask_t mask;
  portlist_t pl;
  hostlist_t h;
} net_t;
