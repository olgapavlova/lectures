/**
 * @file scan.c
 *
 * @brief Сканируем сеть по маске подсети
 */
#include "scan.h"

// Считать маску подсети из входящих данных или значения по умолчанию
static void _get_mask(mask_t* mask, int argc, char** argv) {
  char mask_tmpl[STRL] = {'\0'};
  strcpy(mask_tmpl, (argc < 2) ? SUBNET : argv[1]);

  char* p = strtok(mask_tmpl, "/");
  strcpy(mask->ip, p);

  p = strtok(NULL, "/");
  mask->len = atoi(p);
}

// Считать анализируемые порты из входящих данных или значения по умолчанию
static void _get_ports(portlist_t* pl, int argc, char** argv) {
  char path_tmpl[STRL] = {'\0'};
  strcpy(path_tmpl, (argc < 3) ? PORTSFILE : argv[2]);
  read_portlist_from_file(pl, path_tmpl);
}

// Рассчитать границы пула адресов по маске
static void _count_mask(mask_t* mask) {
  struct in_addr a;
  inet_pton(AF_INET, mask->ip, &a);
  uint32_t ip = ntohl(a.s_addr);
  uint32_t msk = 0xFFFFFFFF << (32 - mask->len);  // определение маски
  mask->network = ip & msk;                       // определение сети
  mask->bandwidth = mask->network | ~msk;  // определение диапазона адресов
}

// Выдать следующий IP-адрес сети
pthread_mutex_t ip_lock = PTHREAD_MUTEX_INITIALIZER;
static uint32_t _next_ip(net_t* net) {
  static uint32_t current = 0;

  pthread_mutex_lock(&ip_lock);

  if (current == 0) {
    current = net->mask.network + 1;
  } else {
    current++;
  }

  pthread_mutex_unlock(&ip_lock);

  return (current < net->mask.bandwidth ? current : 0);
}

// Преобразовать IP-адрес из числа в строку
static void _convert_ip_from_uint_to_str(char* buffer, uint32_t h) {
  struct in_addr ip_a = {0};
  ip_a.s_addr = htonl(h);  // сетевой порядок байтов
  inet_ntop(AF_INET, &ip_a, buffer, INET_ADDRSTRLEN);
}

// Сканировать конкретный порт на конкретном IP
static int _scan_one_host_one_port(const char* ip, const int port) {
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

  // Говорим ядру (да-да): этому сокету не висеть в ожидании больше секунды
  struct timeval tv = {.tv_sec = 1, .tv_usec = 0};
  setsockopt(sckt, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  setsockopt(sckt, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

  // Попытка связи и возврат результата
  int result = connect(sckt, (struct sockaddr*)&addr, sizeof(addr));
  close(sckt);
  return result;
}

// Сканировать все порты на конкретном IP
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;
static void _show_ip_for_all_open_ports(uint32_t ip, portlist_t * pl) {
  char buffer[INET_ADDRSTRLEN] = {'\0'};
  _convert_ip_from_uint_to_str(buffer, ip);
  for(int i = 0; i < pl->len; i++) {
    int closed = _scan_one_host_one_port(buffer, pl->data[i].number);
    
    pthread_mutex_lock(&print_lock);
    if(!closed) {
      printf("%s:%d\t%s\t%s\n", buffer, pl->data[i].number, pl->data[i].protocol, pl->data[i].description);
    } else {
      // printf("%s:%d\t--\n", buffer, pl->data[i].number);
    }
    pthread_mutex_unlock(&print_lock);
  }
}

// Напечатать IP, если хост отзывается на 80 порт
static void _show_ip_if_alive(net_t* net, uint32_t ip) {
  char buffer[INET_ADDRSTRLEN] = {'\0'};
  _convert_ip_from_uint_to_str(buffer, ip);

  int dead = _scan_one_host_one_port(buffer, net->pl.data[0].number);
  if (!dead) printf("%s %d\n", buffer, ip);
}

// Поток для проверки 80-го порта и вывода IP для отзывающихся хостов
void* _show_all_ip_if_alive_in_thread(void* arg) {
  net_t* net = arg;
  uint32_t ip = 0;
  while (0 != (ip = _next_ip(net))) {
    _show_ip_if_alive(net, ip);
  }
  return NULL;
}

// Поток для проверки всех портов и вывода IP для отзывающихся хостов
void* _show_all_ip_if_found_in_thread(void* arg) {
  net_t* net = arg;
  uint32_t ip = 0;
  while (0 != (ip = _next_ip(net))) {
    _show_ip_for_all_open_ports(ip, &(net->pl));
  }
  return NULL;
}

// Точка входа в программу
int main(int argc, char** argv) {
  // Инициируем карту сети
  net_t net = {0};

  // Сначала последовательно (хотя это и не обязательно):
  _get_mask(&(net.mask), argc, argv);  // - считать маску подсети
  _get_ports(&(net.pl), argc,
             argv);  // - считать анализируемые порты
                     // printf("%s %d\n", net.mask.ip, net.mask.len);
                     // for(int i = 0; i < net.pl.len; i++)
                     // printf("%d\t%s\t%s\n", net.pl.data[i].number,
                     // net.pl.data[i].protocol, net.pl.data[i].description);
  _count_mask(
      &(net.mask));  // - рассчитать границы пула адресов по маске
                     // printf("%d %d\n", net.mask.network, net.mask.bandwidth);
                     // printf("%d\n", _next_ip(&net));

  // Сначала пройдёмся по всем адресам одним потоком
  if (0) {
    uint32_t current_ip = 0;
    while (0 != (current_ip = _next_ip(&net))) {
      _show_ip_if_alive(&net, current_ip);
    }
  }

  // Как-то грустно. Давайте параллелить
  if (0) {
    // Запускаем треды
    pthread_t th[TNUM] = {0};
    for (int i = 0; i < TNUM; i++) {
      pthread_create(&th[i], NULL, _show_all_ip_if_alive_in_thread, &net);
    }

    // Ждём, пока треды закончатся
    for (int i = 0; i < TNUM; i++) {
      pthread_join(th[i], NULL);
    }
  }

  // А если все порты?
  if (0) {
    // Запускаем треды
    pthread_t th[TNUM] = {0};
    for (int i = 0; i < TNUM; i++) {
      pthread_create(&th[i], NULL, _show_all_ip_if_found_in_thread, &net);
    }

    // Ждём, пока треды закончатся
    for (int i = 0; i < TNUM; i++) {
      pthread_join(th[i], NULL);
    }
  }

  return EXIT_SUCCESS;
}
