#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define DEVICE_ADDR "96:90:16:63:2D:25"
#define RX_HANDLE   0x0010

int main() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);
    if (sock < 0) {
        perror("HCI open failed");
        return 1;
    }

    bdaddr_t bdaddr;
    str2ba(DEVICE_ADDR, &bdaddr);

    le_create_conn_cp cp;
    memset(&cp, 0, sizeof(cp));
    bacpy(&cp.bdaddr, &bdaddr);
    cp.le_scan_interval = htobs(0x0010);
    cp.le_scan_window   = htobs(0x0010);
    cp.conn_interval_min = htobs(0x0010);
    cp.conn_interval_max = htobs(0x0020);
    cp.conn_latency = htobs(0);
    cp.supervision_timeout = htobs(0x200);
    cp.min_ce_length = htobs(0);
    cp.max_ce_length = htobs(0);

    printf("Creating LE connection...\n");
    if (hci_le_create_conn(sock, &cp, 1000) < 0) {
        perror("LE create conn failed");
        close(sock);
        return 1;
    }

    printf("LE connection created.\n");

    // ATT Write Command
    uint8_t pkt[5];
    pkt[0] = 0x52;             // ATT Write Command opcode
    pkt[1] = RX_HANDLE & 0xFF; // handle LSB
    pkt[2] = RX_HANDLE >> 8;   // handle MSB
    pkt[3] = 'k';               // payload
    pkt[4] = 'u';
    // Можно добавить pkt[5] = 'p';

    // Для реальной отправки нужно формировать HCI ACL data packet,
    // здесь показан концепт, детально требует low-level HCI API.
    printf("Would send ATT Write Command here.\n");

    close(sock);
    return 0;
}

