/**
 * @file
 * @brief Читалка из потокового сокета
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>  // особенности сокетов из домена UNIX (то есть не сетевых)
#include <string.h>

int main(int argc, char *argv[]) {
    int sock;                 // обычный файловый дескриптор
    struct sockaddr_un addr;  // адрес сокета — затейливая структура
    char buf[1024];
    ssize_t n;

    // Считываем адрес сокета в файловой системе
    if (argc != 2) {
        fprintf(stderr, "Запуск читалки: %s <путь_к_сокету>\n", argv[0]);
        return 1;
    }
    const char *socket_path = argv[1];

    // Создаём дескриптор сокета
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror(strerror(sock));
        return 1;
    }

    // Структурируем адрес сокета
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;  // это socket unit, не солнышко
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // Подключаем дескриптор сокета к адресу
    int rs = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rs < 0) {
        perror(strerror(rs));
        close(sock);
        return 1;
    }

    // Читаем поток, пока он не кончится
    while ((n = read(sock, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);  // да сразу порциями и печатаем — и без \0, и без буферизации
        // TODO Именно тут надо сохранить данные на будущее, если хотим их обрабатывать
    }
    if (n < 0) {
        perror(strerror(n));
    }

    close(sock);  // как и файл, сокет надо закрыть
    return 0;
}
