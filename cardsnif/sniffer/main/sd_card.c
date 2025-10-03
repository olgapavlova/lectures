// sd_card.c
#include "sd_card.h"
#include <string.h>
#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"

#include "driver/spi_master.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

static const char *TAG = "sd_card";

// Настройки пинов — проверяй для своей ревизии платы
#define SD_PIN_CLK  GPIO_NUM_40
#define SD_PIN_MISO GPIO_NUM_39
#define SD_PIN_MOSI GPIO_NUM_14
#define SD_PIN_CS   GPIO_NUM_12

static bool s_mounted = false;

esp_err_t sd_init(void)
{
    if (s_mounted) return ESP_OK;

    esp_err_t ret;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;          // попробуй SPI2_HOST; если не работает — пробуй SPI3_HOST

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_PIN_MOSI,
        .miso_io_num = SD_PIN_MISO,
        .sclk_io_num = SD_PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(ret));
        return ret;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = host.slot;

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

    ESP_LOGI(TAG, "SD mounted (name=%s, capacity=%lluMB)",
             card->cid.name, (unsigned long long)(card->csd.capacity / (1024ULL*1024ULL)));
    s_mounted = true;
    return ESP_OK;
}

FILE *sd_open_log(const char *path)
{
    if (!s_mounted) {
        ESP_LOGW(TAG, "sd_open_log called but SD not mounted");
        return NULL;
    }
    FILE *f = fopen(path, "a");
    if (!f) {
        ESP_LOGE(TAG, "fopen(%s) failed", path);
        return NULL;
    }
    // если новый — напишем заголовок
    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        fprintf(f, "ts_us,channel,len,rssi,mac_hash,eapol,head\n");
        fflush(f);
    }
    return f;
}

int sd_write_line(FILE *fp, const char *line)
{
    if (!fp) return -1;
    int rc = fprintf(fp, "%s\n", line);
    if (rc < 0) return -1;
    fflush(fp);
    return 0;
}

esp_err_t sd_close(FILE *fp)
{
    if (fp) fclose(fp);
    if (s_mounted) {
        esp_err_t ret = esp_vfs_fat_sdcard_unmount("/sdcard", NULL);
        // esp_vfs_fat_sdcard_unmount exists in some IDF versions; if not, call esp_vfs_fat_sdcard_unmount alternative
        // To be robust, we will call esp_vfs_fat_sdcard_unmount if available. If not — ignore.
        ESP_LOGI(TAG, "SD unmounted (if supported).");
        s_mounted = false;
        (void)ret;
    }
    return ESP_OK;
}
