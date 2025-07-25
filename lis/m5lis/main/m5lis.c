#include "driver/ledc.h"
#include "esp_rom_sys.h"
#include <math.h>

#define G1_GPIO    1
#define G2_GPIO    2
#define TABLE_LEN  256
#define DELAY_US   100

uint8_t sin_table[TABLE_LEN];

void app_main(void) {
    // Заполнение таблицы синуса (0–255)
    for (int i = 0; i < TABLE_LEN; i++) {
        float angle = 2.0f * M_PI * i / TABLE_LEN;
        sin_table[i] = (uint8_t)(127.5f + 127.5f * sinf(angle));
    }

    // Настройка таймера PWM
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .freq_hz          = 5000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Канал G1 (X)
    ledc_channel_config_t ch0 = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = G1_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ch0);

    // Канал G2 (Y)
    ledc_channel_config_t ch1 = {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = G2_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ch1);

    // 🔁 Главный цикл — сдвиг фаз на π/2 (четверть круга)
    int i = 0;
    while (1) {
        int idx1 = i % TABLE_LEN;
        int idx2 = (i + TABLE_LEN / 4) % TABLE_LEN;  // +90°

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, sin_table[idx1]);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, sin_table[idx2]);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        i = (i + 1) % TABLE_LEN;
        esp_rom_delay_us(DELAY_US);
    }
}
