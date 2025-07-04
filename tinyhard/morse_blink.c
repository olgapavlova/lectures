#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define LED_DEVICE "/dev/led_uart"
#define DOT_DURATION 200
#define DASH_DURATION 600
#define PAUSE_BETWEEN_SYMBOLS 200
#define PAUSE_BETWEEN_LETTERS 600
#define PAUSE_BETWEEN_WORDS 1000

const char *morse_table[128] = {
    ['A'] = ".-",
    ['B'] = "-...",
    ['C'] = "-.-.",
    ['D'] = "-..",
    ['E'] = ".",
    ['F'] = "..-.",
    ['G'] = "--.",
    ['H'] = "....",
    ['I'] = "..",
    ['J'] = ".---",
    ['K'] = "-.-",
    ['L'] = ".-..",
    ['M'] = "--",
    ['N'] = "-.",
    ['O'] = "---",
    ['P'] = ".--.",
    ['Q'] = "--.-",
    ['R'] = ".-.",
    ['S'] = "...",
    ['T'] = "-",
    ['U'] = "..-",
    ['V'] = "...-",
    ['W'] = ".--",
    ['X'] = "-..-",
    ['Y'] = "-.--",
    ['Z'] = "--..",
    ['0'] = "-----",
    ['1'] = ".----",
    ['2'] = "..---",
    ['3'] = "...--",
    ['4'] = "....-",
    ['5'] = ".....",
    ['6'] = "-....",
    ['7'] = "--...",
    ['8'] = "---..",
    ['9'] = "----.",
    [' '] = " "
};

void led_on(int fd) {
    write(fd, "1", 1);
}

void led_off(int fd) {
    write(fd, "0", 1);
}

void morse_symbol(int fd, char symbol) {
    if (symbol == '.') {
        led_on(fd);
        usleep(DOT_DURATION * 1000);
        led_off(fd);
    } else if (symbol == '-') {
        led_on(fd);
        usleep(DASH_DURATION * 1000);
        led_off(fd);
    }
    usleep(PAUSE_BETWEEN_SYMBOLS * 1000);
}

void morse_letter(int fd, char c) {
    const char *code = morse_table[(unsigned char)c];
    if (!code) return;

    if (c == ' ') {
        usleep(PAUSE_BETWEEN_WORDS * 1000);
        return;
    }

    for (int i = 0; code[i]; i++) {
        morse_symbol(fd, code[i]);
    }

    usleep(PAUSE_BETWEEN_LETTERS * 1000);
}

int main(int argc, char **argv) {
    char buffer[1024];

    if (argc >= 2) {
        strncpy(buffer, argv[1], sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
    } else {
        printf("Введите сообщение для мигания (до 1023 символов):\n");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Ошибка чтения ввода\n");
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // удалить \n
    }

    int fd = open(LED_DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    for (int i = 0; buffer[i]; i++) {
        char c = toupper((unsigned char)buffer[i]);
        morse_letter(fd, c);
    }

    close(fd);
    return 0;
}
