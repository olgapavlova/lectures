#include "driver/ledc.h"
#include "esp_rom_sys.h"

#define G1_GPIO 1
#define G2_GPIO 2
#define DELAY_MS 1  // задержка между точками

void set_pwm(uint8_t ch, uint8_t duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, ch, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, ch);
}

void app_main(void) {
    // Настройка таймера
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Канал X (GPIO1)
    ledc_channel_config_t chx = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = G1_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&chx);

    // Канал Y (GPIO2)
    ledc_channel_config_t chy = {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = G2_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&chy);

    while (1) {
        // обход по углам квадрата
        set_pwm(LEDC_CHANNEL_0, 0);    // X = 0
        set_pwm(LEDC_CHANNEL_1, 0);    // Y = 0
        esp_rom_delay_us(DELAY_MS * 1000);  // задержка в микросекундах

        set_pwm(LEDC_CHANNEL_0, 255);  // X = 255
        set_pwm(LEDC_CHANNEL_1, 0);    // Y = 0
        esp_rom_delay_us(DELAY_MS * 1000);  // задержка в микросекундах

        set_pwm(LEDC_CHANNEL_0, 255);  // X = 255
        set_pwm(LEDC_CHANNEL_1, 255);  // Y = 255
        esp_rom_delay_us(DELAY_MS * 1000);  // задержка в микросекундах

        set_pwm(LEDC_CHANNEL_0, 0);    // X = 0
        set_pwm(LEDC_CHANNEL_1, 255);  // Y = 255
        esp_rom_delay_us(DELAY_MS * 1000);  // задержка в микросекундах
    }
}
