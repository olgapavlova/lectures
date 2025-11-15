#include <stdio.h>
#include <stdint.h>
#include <gattlib.h>

#define RX_HANDLE 0x0010

int main() {
    uint8_t cmd[] = { 'k', 'u', 'p' };

    // Пишем в Petoi RX handle
    int ret = gattlib_write_char_by_handle(NULL, RX_HANDLE, cmd, sizeof(cmd));

    if (ret != 0) {
        printf("Write failed: %d\n", ret);
        return 1;
    }

    printf("Write OK\n");
    return 0;
}

