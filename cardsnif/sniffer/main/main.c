// main.c
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "esp_timer.h"

static const char *TAG = "sniffer";

/* Простейшая функция для печати MAC в человекочитаемом виде */
static void mac_to_str(const uint8_t *mac, char *out, size_t out_len) {
    snprintf(out, out_len, "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* Промискуитетный колбэк - вызывается для каждого принятых 802.11 кадров */
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT && type != WIFI_PKT_DATA && type != WIFI_PKT_MISC) {
        // Мы всё равно обрабатываем, но фильтрация возможна
    }

    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
    const wifi_pkt_rx_ctrl_t rx_ctrl = ppkt->rx_ctrl;
    const uint8_t *payload = ppkt->payload;
    uint16_t len = rx_ctrl.sig_len; // длина принятого сигнала (в байтах, приблизительно)

    int8_t rssi = rx_ctrl.rssi;
    int64_t ts = esp_timer_get_time(); // микросекунды с запуска

    // Попытка распарсить src/dst MAC (простая эвристика — работает в большинстве случаев)
    char src[18] = "??:??:??:??:??:??";
    char dst[18] = "??:??:??:??:??:??";
    if (len >= 24) {
        // frame control - 2 байта, addresses обычно начинаются с offset 4
        const uint8_t *addr1 = payload + 4;   // DA / RA
        const uint8_t *addr2 = payload + 10;  // SA / TA
        mac_to_str(addr2, src, sizeof(src));
        mac_to_str(addr1, dst, sizeof(dst));
    }

    // Поиск EAPOL (0x88 0x8e) в первых 64 байтах — простая эвристика
    bool has_eapol = false;
    uint16_t scan_bytes = len < 64 ? len : 64;
    for (uint16_t i = 0; i + 1 < scan_bytes; ++i) {
        if (payload[i] == 0x88 && payload[i+1] == 0x8e) { has_eapol = true; break; }
    }

    // Вывод в лог (CSV-подобный)
    // ts_us, channel, len, rssi, src, dst, eapol_flag, first_16_bytes_hex
    char first_hex[128] = {0};
    int hex_written = 0;
    int max_print = scan_bytes < 16 ? scan_bytes : 16;
    for (int i = 0; i < max_print && hex_written < (int)sizeof(first_hex)-3; ++i) {
        hex_written += snprintf(first_hex + hex_written, sizeof(first_hex) - hex_written, "%02x", payload[i]);
    }

    ESP_LOGI(TAG, "%lld,chan=%d,len=%u,rssi=%d,src=%s,dst=%s,eapol=%d,head=%s",
             ts, rx_ctrl.channel, (unsigned)len, (int)rssi, src, dst, has_eapol ? 1 : 0, first_hex);
}

void app_main(void) {
    // Инициализация NVS (требуется для WiFi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    // Устанавливаем режим STA (можно использовать NULL в некоторых примерах,
    // но STA обеспечивает корректную работу радиомодуля)
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );

    // Устанавливаем фиксированный канал (пока что не хоппим) — канал 6 как пример
    uint8_t channel = 6;
    ESP_ERROR_CHECK( esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE) );
    ESP_LOGI(TAG, "Set channel %d", channel);

    // Включаем promiscuous режим
    ESP_ERROR_CHECK( esp_wifi_set_promiscuous(true) );
    ESP_ERROR_CHECK( esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler) );

    ESP_LOGI(TAG, "Promiscuous sniffer started. Listening on channel %d", channel);

    // Ничего больше не делаем в main — колбэк обрабатывает пакеты
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
