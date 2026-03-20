#include <termios.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "my_phone.h"

#define GPS_TERM "/dev/ttyUSB0"
#define GSM_TERM "/dev/ttyUSB1"

#define CTRL_Z 0x1A

// Читаем терминал через буфер
#define BUF_SIZE 1024
static char buf[BUF_SIZE] = { 0 };
static int buf_len = 0;

// Инициировать терминал с координатами
void init_gps(struct termios * tty_p, int * gps_term_p) {
  *gps_term_p = open(GPS_TERM, O_RDWR | O_NOCTTY);

  tcgetattr(*gps_term_p, tty_p);

  cfsetispeed(tty_p, B9600);
  cfsetospeed(tty_p, B9600);

  tty_p->c_cflag = B9600 | CS8 | CLOCAL | CREAD;
  tty_p->c_iflag = IGNPAR;
  tty_p->c_oflag = 0;
  tty_p->c_lflag = 0;

  tty_p->c_cc[VMIN]  = 1;
  tty_p->c_cc[VTIME] = 0;

  tcsetattr(*gps_term_p, TCSANOW, tty_p);
}

// Инициировать терминал с SMS
void init_gsm(struct termios * tty_p, int * gsm_term_p) {
  *gsm_term_p = open(GSM_TERM, O_RDWR | O_NOCTTY);

  tcgetattr(*gsm_term_p, tty_p);

  cfsetispeed(tty_p, B115200);
  cfsetospeed(tty_p, B115200);

  tty_p->c_cflag = B9600 | CS8 | CLOCAL | CREAD;
  tty_p->c_iflag = IGNPAR;
  tty_p->c_oflag = 0;
  tty_p->c_lflag = 0;

  tty_p->c_cc[VMIN]  = 1;
  tty_p->c_cc[VTIME] = 0;

  tcsetattr(*gsm_term_p, TCSANOW, tty_p);
}

// Проверить, нужна ли нам строка
bool is_coord(char * line) {
  return (line[4] == 'M') && (line[0] == '$');
}

// Взять строку из буфера (если есть)
int get_line(char *out, int out_size) {
    for (int i = 0; i < buf_len; i++) {
        if (buf[i] == '\n') {
            int line_len = i + 1; // включая '\n'
            if (line_len >= out_size) {
              line_len = out_size - 1;
            }

            memcpy(out, buf, line_len);
            out[line_len] = '\0';

            // сдвиг остатка буфера
            memmove(buf, buf + i + 1, buf_len - i - 1);
            buf_len -= (i + 1);

            return 1; // строка готова
        }
    }
    return 0; // строки пока нет
}

// Дождаться приглашения
int wait_prompt(int t) {
    char c;
    while (read(t, &c, 1) == 1) {
        if (c == '>') return 1;
    }
    return 0;
}

// Отправить SMS
void send_sms(int gsm_term, char *line) {
    line[strcspn(line, "\r\n")] = '\0';

    if (line[0] == '\0') {
        fprintf(stderr, "send_sms: empty line\n");
        return;
    }

    write(gsm_term, "AT+CMGF=1\r", strlen("AT+CMGF=1\r"));
    sleep(1);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r", MY_PHONE);
    write(gsm_term, cmd, strlen(cmd));

    if (!wait_prompt(gsm_term)) {
        fprintf(stderr, "send_sms: no prompt\n");
        return;
    }

    printf("Sending SMS len=%zu text=[%s]\n", strlen(line), line);

    write(gsm_term, line, strlen(line));

    char end = CTRL_Z;
    write(gsm_term, &end, 1);
}

int main(void) {

  // Инициируем терминал с координатами
  struct termios tty = { 0 };
  int gps_term = -1;
  init_gps(&tty, &gps_term);

  // Инициируем терминал с sms
  struct termios gsm = { 0 };
  int gsm_term = -1;
  init_gsm(&gsm, &gsm_term);

  // Отправляем в GSM-терминал инициирующие команды
  write(gsm_term, "AT\r", 3);
  write(gsm_term, "ATE0\r", 5);
  write(gsm_term, "AT+CMGF=1\r", 10);
  write(gsm_term, "AT+CNMI=2,1,0,0,0\r", 18);

  while(1) {
    char tmp[256];
    int n = read(gps_term, tmp, sizeof(tmp));

    // что-то прочли
    if (n > 0) {
        // добавляем в буфер
        memcpy(buf + buf_len, tmp, n);
        buf_len += n;

        // и тут же пытаемся извлечь строки из буфера
        char line[256] = { 0 };
        char coord_line[256] = { 0 };
        while (get_line(line, sizeof(line))) {
            // убираем конец строки и возврат каретки
            char *p = line;
            while (*p) {
                if (*p == '\r' || *p == '\n') {
                    *p = '\0';
                    break;
                }
                p++;
            }

            if(is_coord(line)) {
              strcpy(coord_line, line);
            }
        }

        printf("%s\n", coord_line);
        send_sms(gsm_term, coord_line);
        sleep(10);
    }

  }

  return 0;
}
