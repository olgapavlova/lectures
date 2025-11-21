#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void send(FILE * fp, char * command) {
    fprintf(fp, "%s\n", command);
    fflush(fp);
    sleep(5);
}

int main() {
    FILE *fp = popen("gatttool -b 96:90:16:63:2D:25 -I", "w");
    if (fp == NULL) return 1;
    sleep(10);

    send(fp, "connect");
    send(fp, "char-write-req 0x0010 6b7570");
    send(fp, "disconnect");
    send(fp, "exit");

    // Можно оставить gatttool открытым или закрыть
    pclose(fp);
    return 0;
}

