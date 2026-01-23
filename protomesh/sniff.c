/** @file
  * @brief Слушаем Meshtastic через Serial-порт
  */

#include <errno.h>    // обрабатываем ошибки системных вызовов
#include <fcntl.h>    // открываем порт
#include <inttypes.h> // типы фиксированного размера + спецификаторы для их вывода
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>  // настраиваем терминал
#include <unistd.h>   // читаем и пишем в терминал

/** @brief Открыть и настроить Serial-порт
  * 
  * @param[in]  path  путь к порту
  * @return           файловый дескриптор файл-терминала
  */
static int open_serial(const char *path) {
    // открываем порт как файл
    // права: чтение, запись, не управляющий терминал, закрыть при закрытии процесса
    int fd = open(path, O_RDWR | O_NOCTTY | O_CLOEXEC);
    if (fd < 0) {
        perror(strerror(fd));
        return -1;
    }

    // создаём структуру для настройки терминала
    struct termios tio;

    // будем донастраивать файл-терминал 
    int tc = tcgetattr(fd, &tio);
    if (tc!= 0) {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    // это прямо в документации предписано сделать :)
    // суть — работать с сырыми данными без дополнительной обработки
    // и без попыток выводить символы, попавшие в терминал
    tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
               | INLCR | IGNCR | ICRNL | IXON);
    tio.c_oflag &= ~OPOST;
    tio.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tio.c_cflag &= ~(CSIZE | PARENB);
    tio.c_cflag |= CS8;

    // настраиваем скорость терминала
    cfsetispeed(&tio, B115200);
    cfsetospeed(&tio, B115200);

    // способ передачи байта — стандартный:
    //  [1 бит старта] — всегда 0, его и не специфицируют
    //  [8 бит нашего байта]
    //  [нет бита контроля чётности] — то есть тут пусто
    //  [1 бит финиша]
    // итого 10 бит на 1 байт
    // обозначение такого способа — 8N1
    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag &= ~CRTSCTS;

    // настраиваем как бы неблокирующее поведение read
    tio.c_cc[VMIN]  = 1;   // получили 1 байт — и можно возвращаться
    tio.c_cc[VTIME] = 1;   // но можем и подождать ещё, пока не случится 0.1 секунды паузы между байтами 

    // транзакция настройки файл-порта из структуры
    int tcs = tcsetattr(fd, TCSANOW, &tio);
    if (tcs != 0) {
        perror(strerror(tcs));
        close(fd);
        return -1;
    }

    // возвращаем файловый дескриптор открытого файл-терминала
    return fd;
}

/** @brief Вывести int-массив в 16-ричном коде и по 16 чисел на строку */
static void hexdump(const uint8_t *buf, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%02X", buf[i]);
        if ((i + 1) % 16 == 0 || i + 1 == n) printf("%s", ((i + 1) % 16 == 0) ? "\n" : "");
        else printf(" ");
    }
    if (n % 16 != 0) printf("\n");
}

int main(int argc, char **argv) {
    // нам нужен порт и его файл-дескриптор
    const char *dev = (argc >= 2) ? argv[1] : "/dev/ttyUSB0";
    int fd = open_serial(dev);
    if (fd < 0) {
      perror(strerror(fd));
      return 1;
    }

    fprintf(stderr, "Читаем Meshtastic-фреймы с порта %s...\n", dev);
    fprintf(stderr, "Ждём заголовок: [ 94 C3 ] [ len_msb len_lsb ] [ payload ]\n");

    uint8_t hello[] = {
      0x94, 0xC3,  // код начала фрейма
      0x00, 0x00   // length = 0 (пустой ToRadio) — просто дать сигнал
    };

    // отправить пустой запрос, чтобы спровоцировать ноду ответить
    // если не работает — тогда meshtastic --info --port ... из другого терминала
    int rc = write(fd, hello, sizeof(hello));
    if(rc < 0) { perror(strerror(rc)); }
    // tcdrain(fd);

    // а вот и наш конечный автомат
    enum { SYNC0, SYNC1, LEN0, LEN1, PAYLOAD } st = SYNC0;  // состояния
    uint16_t len = 0;  // сколько байтов ждём
    uint16_t got = 0;  // сколько байтов уже получили
    uint8_t payload[512];  // больше 512 байтов не дадут

    // запускаем конечный автомат
    for (;;) {
        // прочтём байт
        uint8_t b = 0;
        ssize_t r = read(fd, &b, 1);
        if (r < 0) {
            if (errno == EINTR) continue;
            perror("read");
            break;
        }
        if (r == 0) continue;

        // среагируем в зависимости от состояния
        switch (st) {
            case SYNC0:
                if (b == 0x94) st = SYNC1;  // появился шанс на фрейм 
                break;

            case SYNC1:
                if (b == 0xC3) st = LEN0;  // точно фрейм
                else st = (b == 0x94) ? SYNC1 : SYNC0; // нет, ещё только шанс
                break;

            case LEN0:
                len = ((uint16_t)b) << 8;  // старшие биты длины фрейма
                st = LEN1;  // дальше будем читать младшие биты
                break;

            case LEN1:
                len |= (uint16_t)b;  // младшие биты длины фрейма
                got = 0;  // читать фрейм будем с начала, конечно же

                if (len == 0 || len > sizeof(payload)) {  // мало ли
                    fprintf(stderr, "Некорректная длина фрейма = %u, ищем дальше\n", (unsigned)len);
                    st = SYNC0;  // снова ищем
                } else {
                    st = PAYLOAD;  // переходим в состояние чтения фрейма
                }
                break;

            case PAYLOAD:
                payload[got++] = b;  // следующий бит
                if (got == len) {    // не последний ли?
                    printf("\nФрейм длины [ %u ]\n", (unsigned)len);
                    hexdump(payload, len);
                    fflush(stdout);  // буфер, кыш отсюда 
                    st = SYNC0;      // и снова ждём
                }
                break;
        }
    }

    close(fd);  // помните бит «И порт тоже закрыть»?
    return 0;
}
