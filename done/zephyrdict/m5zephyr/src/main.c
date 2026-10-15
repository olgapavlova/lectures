#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* Контроллер GPIO */
const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

/* Пины дешифратора строк 74HC138 (A0, A1, A2) */
const uint8_t row_addr_pins[] = {8, 9, 11};

/* Пины колонок клавиатуры */
const uint8_t col_pins[] = {13, 15, 3, 4, 5, 6, 7};

void init_keyboard(void) {
    // Настраиваем адресные пины дешифратора на выход
    for (int i = 0; i < 3; i++) {
        gpio_pin_configure(gpio_dev, row_addr_pins[i], GPIO_OUTPUT_LOW);
    }

    // Настраиваем колонки на вход со стандартной подтяжкой вверх
    for (int i = 0; i < 7; i++) {
        gpio_pin_configure(gpio_dev, col_pins[i], GPIO_INPUT | GPIO_PULL_UP);
    }
}

/* Установка адреса строки на дешифраторе */
void set_row_address(int r) {
    if (r < 0) {
        // Спец-режим: выключаем все линии (подаем 1), чтобы разрядить шину
        gpio_pin_set(gpio_dev, row_addr_pins[0], 1);
        gpio_pin_set(gpio_dev, row_addr_pins[1], 1);
        gpio_pin_set(gpio_dev, row_addr_pins[2], 1);
        return;
    }
    // Побитово выставляем адрес строки
    gpio_pin_set(gpio_dev, row_addr_pins[0], (r & 0x01) ? 1 : 0);
    gpio_pin_set(gpio_dev, row_addr_pins[1], (r & 0x02) ? 1 : 0);
    gpio_pin_set(gpio_dev, row_addr_pins[2], (r & 0x04) ? 1 : 0);
}

int main(void)
{
    k_sleep(K_MSEC(1500));
    printk("\n\n=== Cardputer Matrix Scanner Started ===\n");
    
    init_keyboard();

    while (1) {
        // Перебираем все 8 строк (0 - 7)
        for (int r = 0; r < 8; r++) {
            
            // 1. Принудительно сбрасываем дешифратор перед установкой новой строки
            set_row_address(-1);
            k_busy_wait(5);

            // 2. Выставляем адрес текущей строки
            set_row_address(r);
            
            // Даем линиям 20 микросекунд на стабилизацию уровней
            k_busy_wait(20);

            // 3. Читаем наши 7 колонок
            for (int c = 0; c < 7; c++) {
                int val = gpio_pin_get(gpio_dev, col_pins[c]);
                
                // Если считали 0 — кнопка нажата физически
                if (val == 0) {
                    printk("Нажата клавиша! Строка (R): %d,  Колонка (C): %d\n", r, c);
                    
                    // Задержка от дребезга контактов
                    k_sleep(K_MSEC(200)); 
                }
            }
        }
        // Пауза 15 мс между полными циклами сканирования всей клавиатуры
        k_sleep(K_MSEC(15)); 
    }
    return 0;
}

