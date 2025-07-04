#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define DEVICE_PATH "/dev/servo90"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <градусы от 0 до 90>\n", argv[0]);
        return 1;
    }

    int degrees = atoi(argv[1]);
    if (degrees < 0 || degrees > 90 || degrees % 10 != 0) {
        fprintf(stderr, "Градусы должны быть кратны 10 и в пределах 0–90.\n");
        return 1;
    }

    int steps = degrees / 10;

    int fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Не удалось открыть " DEVICE_PATH);
        return 1;
    }

    for (int i = 0; i < steps; i++) {
        if (write(fd, "1", 1) != 1) {
            perror("Ошибка записи");
            close(fd);
            return 1;
        }
        usleep(300000); // подождать 300 мс между командами
    }

    close(fd);
    return 0;
}
