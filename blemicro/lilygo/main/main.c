#include <stdio.h>

#include "esp_check.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"

static const char* TAG = "LILYGO";

void app_main(void) {
  ESP_LOGI(TAG, "== START ==");

  esp_err_t res = nvs_flash_init();
  if (res != ESP_OK) {
    ESP_LOGI(TAG, "Wrong flash initiation: %s.\n", esp_err_to_name(res));
  } else {
    ESP_LOGI(TAG, "Flash init successfully.");
  }

  esp_err_t res_port = nimble_port_init();
  if (res_port != ESP_OK) {
    ESP_LOGI(TAG, "Wrong port initiation: %s.\n", esp_err_to_name(res_port));
  } else {
    ESP_LOGI(TAG, "Port init successfully.");

    ESP_LOGI(TAG, "== FINISH ==");
  }
}
