// main.c
#include "esp_log.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include <dirent.h>
#include <stdio.h>

#include "sd_card.h"
#include "wifi_sniffer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <sys/time.h>
#include <time.h>

static const char *TAG = "main_app";

static void build_log_filename(char *out, size_t out_len) {
  uint32_t r = esp_random();
  snprintf(out, out_len, "/sdcard/%08lx.csv", r);
  ESP_LOGI(TAG, "Log filename: %s", out);
}

void app_main(void) {
  // 1) NVS (необходимо для WiFi)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    ret = nvs_flash_init();
  }
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "nvs_flash_init failed: %s", esp_err_to_name(ret));
    // можно продолжить (только если готовы потерять WiFi), но лучше
    // остановиться
  }
  // --- 2. SD init ---
  if (sd_init() != ESP_OK) {
    ESP_LOGW(TAG, "sd_init failed, continue without SD");
  }

  // 3) Запускаем WiFi сниффер, логируем в файл если открыт
  char fname[128];
  build_log_filename(fname, sizeof(fname));
  ESP_LOGI(TAG, "Opening log file: %s", fname);

  FILE *log = sd_open_log(fname); // может вернуть NULL — okay
  if (log == NULL) {
    ESP_LOGE(TAG, "log file is NULL");
  }
  if (wifi_sniffer_start(6, log) != ESP_OK) {
    ESP_LOGW(TAG, "wifi_sniffer_start failed");
  }

  // список каналов, которые будем крутить
  const uint8_t chans[] = {1, 6, 11};

  // запуск channel hopper
  wifi_hopper_start(chans, sizeof(chans) / sizeof(chans[0]),
                    800,   // dwell_ms: 800 мс на канал
                    3000); // hold_on_eapol_ms: удерживать 3 сек при EAPOL

  // главный цикл — ничего не делать: сниффер работает в callback'е
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}
