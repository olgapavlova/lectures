#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("gatttool -b 96:90:16:63:2D:25 -I", "w");
    if (fp == NULL) return 1;

    // Команда "write RX"
    fprintf(fp, "char-write-req 0x0010 6b7570\n");
    fflush(fp);

    // Можно оставить gatttool открытым или закрыть
    fclose(fp);
    return 0;
}

