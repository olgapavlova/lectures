// main.c
#include <stdio.h>
#include <dirent.h>
#include "esp_log.h"
#include "nvs_flash.h"

#include "sd_card.h"
#include "wifi_sniffer.h"

#include "freertos/FreeRTOS.h"   // добавлено
#include "freertos/task.h"       // добавлено

static const char *TAG = "main_app";

void app_main(void)
{
    // 1) NVS (необходимо для WiFi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "nvs_flash_init failed: %s", esp_err_to_name(ret));
        // можно продолжить (только если готовы потерять WiFi), но лучше остановиться
    }
    // --- 2. SD init ---
    if (sd_init() != ESP_OK) {
        ESP_LOGW(TAG, "sd_init failed, continue without SD");
    }

    // 3) Запускаем WiFi сниффер, логируем в файл если открыт
    FILE *log = sd_open_log("/sdcard/sflog.csv"); // может вернуть NULL — okay
    if(log == NULL) {ESP_LOGE(TAG, "log file is NULL"); }
    if (wifi_sniffer_start(6, log) != ESP_OK) {
        ESP_LOGW(TAG, "wifi_sniffer_start failed");
    }

    // главный цикл — ничего не делать: сниффер работает в callback'е
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
