// sd_card.h
#pragma once
#include "esp_err.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Инициализировать SD (SPI). Возвращает ESP_OK или ошибку.
esp_err_t sd_init(void);

// Открыть лог-файл на SD (append). Возвращает FILE* или NULL.
FILE *sd_open_log(const char *path);

// Записать строку (с '\n') в лог-файл (если fp==NULL — ничего не делает).
// Возвращает 0 при успехе, <0 при ошибке.
int sd_write_line(FILE *fp, const char *line);

// Закрыть файл и размонтировать SD (может быть noop если fp==NULL)
esp_err_t sd_close(FILE *fp);

#ifdef __cplusplus
}
#endif
