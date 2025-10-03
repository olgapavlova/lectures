// main/main.c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "driver/sdmmc_host.h"    // содержит SDSPI_HOST_DEFAULT
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

static const char *TAG = "sdtest";

static esp_err_t mount_sd_spi_and_open_example(void)
{
    esp_err_t ret;

    // 1) Настройка SPI host (M5 Cardputer обычно на SPI2_HOST)
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;

    // 2) Пины для M5 Cardputer (проверь ревизию — для большинства карт эти значения корректны)
    const gpio_num_t PIN_NUM_CLK  = GPIO_NUM_40;
    const gpio_num_t PIN_NUM_MISO = GPIO_NUM_39;
    const gpio_num_t PIN_NUM_MOSI = GPIO_NUM_14;
    const gpio_num_t PIN_NUM_CS   = GPIO_NUM_12;

    // 3) Инициализация SPI bus
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // 4) Конфиг устройства SD на SPI
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    // 5) Монтирование FAT
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_vfs_fat_sdspi_mount failed: %s (0x%x)", esp_err_to_name(ret), ret);
        return ret;
    }

    ESP_LOGI(TAG, "SD mounted (name=%s, capacity=%lluMB)", card->cid.name, (unsigned long long)(card->csd.capacity / (1024ULL*1024ULL)));

    // 6) Запись тестового файла
    const char *path = "/sdcard/hello.txt";
    FILE *f = fopen(path, "w");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        // не размонтируем здесь — пусть вызывающий решит
        return ESP_FAIL;
    }
    fprintf(f, "hello from Cardputer\n");
    fflush(f);
    fclose(f);

    ESP_LOGI(TAG, "Wrote file: %s", path);
    return ESP_OK;
}

void app_main(void)
{
    esp_err_t err = mount_sd_spi_and_open_example();
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "SD test OK");
    } else {
        ESP_LOGW(TAG, "SD test failed (%s). Check card, format (FAT32) and pins.", esp_err_to_name(err));
    }

    // Оставим задачу спящую — можно перезагрузить/подключиться по монитор-порту и посмотреть файл на карте
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
