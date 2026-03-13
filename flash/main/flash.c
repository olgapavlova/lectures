#include "esp_log.h"
#include "esp_flash_spi_init.h"
#include "esp_flash.h"
#include "esp_hmac.h"
#include "esp_random.h"
#include "driver/spi_master.h"
#include "mbedtls/aes.h"

// Настройка SPI-подключения к GPIO
#define PIN_NUM_MISO 13
#define PIN_NUM_MOSI 11
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10

// Метка для логов прошивки
static const char *TAG = "\033[93mflash\033[0m";

// Подготовить flash-модуль как обычную SPI-железку
spi_device_handle_t spi_prepare(void) {
    spi_device_handle_t result = {0};

    // Конфигурация SPI-шины
    spi_bus_config_t spi_bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,  // обычный SPI без добавок
        .quadhd_io_num = -1,
    };

    // Конфигурация flash-модуля
    spi_device_interface_config_t flash_module_cfg = {
        .clock_speed_hz = 5 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    spi_bus_initialize(SPI3_HOST, &spi_bus_cfg, SPI_DMA_CH_AUTO);  // инициируем SPI-шину
    spi_bus_add_device(SPI3_HOST, &flash_module_cfg, &result);     // подключаем наше устройство — модуль памяти

    return result;
}

// Узнать свойства flash-модуля как железа
void describe_flash_module(spi_device_handle_t * spi) {
    uint8_t tx[4] = { 0x9F, 0, 0, 0 };
    uint8_t rx[4] = { 0 };

    spi_transaction_t t = {
        .length = 32,
        .tx_buffer = tx,
        .rx_buffer = rx,
    };

    spi_device_transmit(*spi, &t);

    ESP_LOGI(TAG, "JEDEC ID: %02X / %02X / %02X", rx[1], rx[2], rx[3]);
}


// Точка входа
void app_main(void) {

    // Через эту структуру работаем с SPI
    spi_device_handle_t spi = spi_prepare();

    // Давайте хранить данные во внешнем flash-модуле
    describe_flash_module(&spi);
}
