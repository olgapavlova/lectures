// sd_test_main.c
#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "sd_card.h"

#include "freertos/FreeRTOS.h"   // добавлено
#include "freertos/task.h"       // добавлено

void app_main(void)
{
    nvs_flash_init();
    if (sd_init() == ESP_OK) {
        FILE *f = sd_open_log("/sdcard/test_sd.csv");
        if (f) {
            sd_write_line(f, "hello sd");
            sd_write_line(f, "line2");
            fclose(f);
            ESP_LOGI("SDTEST", "Wrote test_sd.csv");
        } else {
            ESP_LOGE("SDTEST", "open failed");
        }
    } else {
        ESP_LOGE("SDTEST", "sd_init failed");
    }
    while(1) vTaskDelay(pdMS_TO_TICKS(10000));
}
