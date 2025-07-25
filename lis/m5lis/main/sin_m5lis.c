#include "driver/ledc.h"
#include "esp_rom_sys.h"
#include <math.h>

#define G1_GPIO    1
#define G2_GPIO    2
#define TABLE_LEN  256
#define DELAY_US   100

uint8_t sin_table[TABLE_LEN];

void app_main(void) {
    // Заполнить таблицу синусоиды (8 бит: 0–255)
    for (int i = 0; i < TABLE_LEN; i++) {
        float angle = 2.0f * M_PI * i / TABLE_LEN;
        sin_table[i] = (uint8_t)(127.5f + 127.5f * sinf(angle));
    }

    // Настройка PWM таймера
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Канал 1 (GPIO1)
    ledc_channel_config_t ch0 = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = G1_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ch0);

    // Канал 2 (GPIO2)
    ledc_channel_config_t ch1 = {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = G2_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ch1);

    int i = 0;
    while (1) {
        uint8_t v1 = sin_table[i];
        uint8_t v2 = sin_table[(i + TABLE_LEN / 2) % TABLE_LEN];  // 180° сдвиг

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, v1);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, v2);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        i = (i + 1) % TABLE_LEN;
        esp_rom_delay_us(DELAY_US);
    }
}
