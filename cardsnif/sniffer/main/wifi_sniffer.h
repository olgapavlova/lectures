// wifi_sniffer.h
#pragma once
#include "esp_err.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Инициализация сниффера: channel — канал 1..13, log_fp — файл для записи
// (может быть NULL -> вывод только в консоль)
esp_err_t wifi_sniffer_start(uint8_t channel, FILE *log_fp);

esp_err_t wifi_hopper_start(const uint8_t *, int, uint32_t, uint32_t);

// Остановить сниффер (опционально)
void wifi_sniffer_stop(void);

#ifdef __cplusplus
}
#endif
