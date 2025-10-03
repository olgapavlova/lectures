// wifi_sniffer.c
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "wifi_sniffer.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"

#include "mbedtls/sha256.h" // для простого хеша mac, если хотим

static const char *TAG = "wifi_sniffer";
static FILE *s_log_fp = NULL;

static void payload_head_hex(const uint8_t *payload, uint16_t len, char *out,
                             size_t out_len) {
  int max_print = len < 16 ? len : 16;
  int p = 0;
  for (int i = 0; i < max_print && p < (int)out_len - 2; ++i) {
    p += snprintf(out + p, out_len - p, "%02x", payload[i]);
  }
  out[p] = '\0';
}

static void mac_hash_hex(const uint8_t *mac, char *out_hex,
                         size_t out_hex_len) {
  const char *salt = "unique_salt_2025";
  uint8_t buf[6 + 32];
  size_t sl = strlen(salt);
  if (sl > 32)
    sl = 32;
  memcpy(buf, mac, 6);
  memcpy(buf + 6, salt, sl);
  uint8_t sha[32];
  mbedtls_sha256(buf, 6 + sl, sha, 0);
  int p = 0;
  for (int i = 0; i < 8 && p < (int)out_hex_len - 2; ++i) {
    p += snprintf(out_hex + p, out_hex_len - p, "%02x", sha[i]);
  }
  out_hex[p] = '\0';
}

// ===== channel hopper + hold-on-EAPOL =====
// Помести в wifi_sniffer.c (внизу) вместе с остальным кодом

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>
#include <stdint.h>

static const char *HOP_TAG = "wifi_hopper";

static TaskHandle_t s_hopper_task = NULL;
static volatile bool s_hopper_running = false;

// hopper config
typedef struct {
  uint8_t channels[32];
  int count;
  uint32_t dwell_ms;         // обычный dwell
  uint32_t hold_on_eapol_ms; // сколько удерживать текущий канал при обнаружении
                             // EAPOL
} hopper_cfg_t;

static hopper_cfg_t s_hopper_cfg = {{1, 6, 11}, 3, 800, 3000};

// Время (utc us since boot) до которого нужно удерживать текущий канал.
// если esp_timer_get_time() < s_hold_until_us => удерживаем канал и НЕ
// переключаемся.
static volatile int64_t s_hold_until_us = 0;

// Вспомогательная: установить удержание сейчас (вызывается из колбэка при
// EAPOL)
static inline void wifi_hopper_hold_now(void) {
  int64_t now = esp_timer_get_time();
  int64_t extra = (int64_t)s_hopper_cfg.hold_on_eapol_ms * 1000LL;
  int64_t new_until = now + extra;
  // Устанавливаем более длинное удержание (если уже есть удержание, берем max)
  if (new_until > s_hold_until_us) {
    s_hold_until_us = new_until;
    ESP_LOGI(HOP_TAG, "hold set until +%d ms (now=%lld)",
             s_hopper_cfg.hold_on_eapol_ms, (long long)now);
  }
}

// Hopper task
static void channel_hopper_task(void *arg) {
  (void)arg;
  int idx = 0;
  while (s_hopper_running) {
    // Если удерживаем канал — ждать пока время не истечёт (проверяем каждые 100
    // ms)
    int64_t now = esp_timer_get_time();
    if (now < s_hold_until_us) {
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    if (s_hopper_cfg.count <= 0) {
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }
    uint8_t ch = s_hopper_cfg.channels[idx % s_hopper_cfg.count];
    esp_err_t r = esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
    if (r != ESP_OK) {
      ESP_LOGW(HOP_TAG, "set_channel %d failed: %s", ch, esp_err_to_name(r));
    } else {
      ESP_LOGD(HOP_TAG, "hopped to channel %d", ch);
    }

    // dwell: но если в течение dwell появится EAPOL (colback вызовет
    // wifi_hopper_hold_now), то следующий цикл увидит s_hold_until_us и удержит
    // канал.
    vTaskDelay(pdMS_TO_TICKS(s_hopper_cfg.dwell_ms));
    idx++;
  }
  s_hopper_task = NULL;
  vTaskDelete(NULL);
}

// API: старт/стоп хоппера
esp_err_t wifi_hopper_start(const uint8_t *channels, int channels_count,
                            uint32_t dwell_ms, uint32_t hold_on_eapol_ms) {
  if (!channels || channels_count <= 0)
    return ESP_ERR_INVALID_ARG;
  if (channels_count > (int)sizeof(s_hopper_cfg.channels))
    return ESP_ERR_INVALID_SIZE;
  if (s_hopper_task != NULL)
    return ESP_ERR_INVALID_STATE;

  // копируем конфиг
  for (int i = 0; i < channels_count; ++i)
    s_hopper_cfg.channels[i] = channels[i];
  s_hopper_cfg.count = channels_count;
  s_hopper_cfg.dwell_ms = dwell_ms ? dwell_ms : 800;
  s_hopper_cfg.hold_on_eapol_ms = hold_on_eapol_ms ? hold_on_eapol_ms : 3000;

  s_hold_until_us = 0;
  s_hopper_running = true;
  BaseType_t ok = xTaskCreate(channel_hopper_task, "channel_hopper", 2048, NULL,
                              5, &s_hopper_task);
  return (ok == pdPASS) ? ESP_OK : ESP_FAIL;
}

void wifi_hopper_stop(void) {
  if (!s_hopper_running)
    return;
  s_hopper_running = false;
  // ждать завершения задачи (необязательно)
  for (int i = 0; i < 50 && s_hopper_task != NULL; ++i) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  s_hold_until_us = 0;
}

static void wifi_promiscuous_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  const wifi_pkt_rx_ctrl_t rx = ppkt->rx_ctrl;
  const uint8_t *payload = ppkt->payload;
  uint16_t len = rx.sig_len;
  int8_t rssi = rx.rssi;
  int64_t ts = esp_timer_get_time();

  uint8_t src[6] = {0};
  if (len >= 24)
    memcpy(src, payload + 10, 6);

  // eapol heuristic
  bool eapol = false;
  uint16_t scan = len < 128 ? len : 128;
  for (uint16_t i = 0; i + 1 < scan; ++i) {
    if (payload[i] == 0x88 && payload[i + 1] == 0x8e) {
      eapol = true;
      break;
    }
  }

  if (eapol) { wifi_hopper_hold_now(); }  // удерживаем канал с «интересным»

  char head[64];
  payload_head_hex(payload, len, head, sizeof(head));

  char mach[33];
  mac_hash_hex(src, mach, sizeof(mach));

  // format CSV line into small buffer
  char line[256];
  snprintf(line, sizeof(line), "%" PRIi64 ",%u,%u,%d,%s,%d,%s", ts, rx.channel,
           (unsigned)len, (int)rssi, mach, eapol ? 1 : 0, head);

  if (s_log_fp) {
    fprintf(s_log_fp, "%s\n", line);
    fflush(s_log_fp);
    fsync(fileno(s_log_fp));
    ESP_LOGI(TAG, "WROTE TO FILE: %s", line);
  }
  ESP_LOGI(TAG, "%s", line);
}

esp_err_t wifi_sniffer_start(uint8_t channel, FILE *log_fp) {
  s_log_fp = log_fp;

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_err_t r = esp_wifi_init(&cfg);
  if (r != ESP_OK) {
    ESP_LOGE(TAG, "esp_wifi_init failed: %s", esp_err_to_name(r));
    return r;
  }
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous_cb));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  ESP_LOGI(TAG, "WiFi sniffer started on channel %d", channel);
  return ESP_OK;
}

void wifi_sniffer_stop(void) {
  esp_wifi_set_promiscuous(false);
  esp_wifi_stop();
  esp_wifi_deinit();
  s_log_fp = NULL;
}
