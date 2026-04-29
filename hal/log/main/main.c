#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "hal_probe";

#define PROBE_GPIO      GPIO_NUM_2

#define SPI_MOSI_GPIO   GPIO_NUM_1
#define SPI_SCLK_GPIO   GPIO_NUM_2
#define SPI_MISO_GPIO   GPIO_NUM_NC
#define SPI_CS_GPIO     GPIO_NUM_NC

static void init_log_levels(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    esp_log_level_set("gpio", ESP_LOG_DEBUG);
    esp_log_level_set("spi", ESP_LOG_DEBUG);
    esp_log_level_set("spi_master", ESP_LOG_DEBUG);
    esp_log_level_set("spi_hal", ESP_LOG_DEBUG);
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
}

static void probe_gpio_once(void)
{
    ESP_LOGI(TAG, "GPIO config: pin=%d as OUTPUT", PROBE_GPIO);

    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << PROBE_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&cfg));

    ESP_LOGI(TAG, "GPIO set HIGH");
    ESP_ERROR_CHECK(gpio_set_level(PROBE_GPIO, 1));
    vTaskDelay(pdMS_TO_TICKS(200));

    ESP_LOGI(TAG, "GPIO set LOW");
    ESP_ERROR_CHECK(gpio_set_level(PROBE_GPIO, 0));
    vTaskDelay(pdMS_TO_TICKS(200));
}

static void probe_spi_once(int clock_hz)
{
    ESP_LOGI(TAG, "SPI init: MOSI=%d SCLK=%d clock=%d Hz",
             SPI_MOSI_GPIO, SPI_SCLK_GPIO, clock_hz);

    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI_GPIO,
        .miso_io_num = SPI_MISO_GPIO,
        .sclk_io_num = SPI_SCLK_GPIO,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = 32,
    };

    esp_err_t err = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(err));
        return;
    }

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = clock_hz,
        .mode = 0,
        .spics_io_num = SPI_CS_GPIO,
        .queue_size = 1,
    };

    spi_device_handle_t spi = NULL;

    err = spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_add_device failed: %s", esp_err_to_name(err));
        spi_bus_free(SPI2_HOST);
        return;
    }

    uint8_t tx[8] = {0x00, 0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC, 0xDD};

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = sizeof(tx) * 8;
    t.tx_buffer = tx;

    ESP_LOGI(TAG, "SPI transmit: %d bytes", sizeof(tx));

    err = spi_device_transmit(spi, &t);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_device_transmit failed: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "SPI cleanup");

    ESP_ERROR_CHECK(spi_bus_remove_device(spi));
    ESP_ERROR_CHECK(spi_bus_free(SPI2_HOST));
}

void app_main(void)
{
    init_log_levels();

    ESP_LOGI(TAG, "HAL probe started");

    const int clocks[] = {
        1000000,
        5000000,
        10000000,
        20000000,
    };

    size_t i = 0;

    while (1) {
        ESP_LOGI(TAG, "---- HAL probe cycle ----");

        probe_gpio_once();
        probe_spi_once(clocks[i]);

        i = (i + 1) % (sizeof(clocks) / sizeof(clocks[0]));

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
