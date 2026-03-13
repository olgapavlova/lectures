#include "esp_log.h"
#include "esp_flash_spi_init.h"
#include "esp_hmac.h"
#include "esp_random.h"
#include "driver/spi_master.h"
#include "mbedtls/aes.h"

// Настройка SPI-подключения к GPIO
#define PIN_NUM_MISO 13
#define PIN_NUM_MOSI 11
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10

// Параметры шифрования
#define KEY_LEN 32
#define AES_BLOCK 16

// Метка для логов прошивки
static const char *TAG = "\033[93mflash\033[0m";

// Подготовить flash-модуль к работе
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

// Сохранить HEX-данные в строку
static void _hex_to_str(uint8_t * h, char * s, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        sprintf(&s[i * 2], "%02X", h[i]);
    }
    s[len * 2 - 1] = '\0';
}

// Вычислить ключ для контекста на основе eFuse
void calculate_key_with_efuse(uint8_t * dk) {
    const uint8_t context[] = "demo context to show eFuse root key usage";

    esp_err_t err = esp_hmac_calculate(
        HMAC_KEY0,             // key block in eFuse
        context,
        sizeof(context) - 1,
        dk
    );

    if (err != ESP_OK) {
        ESP_LOGI(TAG, "esp_hmac_calculate failed: %s\n", esp_err_to_name(err));
        return;
    }

    char dk_str[KEY_LEN * 2 + 1] = { 0 };
    _hex_to_str(dk, dk_str, KEY_LEN);

    ESP_LOGI(TAG, "Ключ AES-256:\t%s", dk_str);
    ESP_LOGI(TAG, "Контекст ключа:\t%s", context);
}

// Продемонстрировать шифрование и дешифрование
void cipher_decipher(uint8_t * dk) {
    // Один блок AES = 16 байт
    uint8_t plaintext[AES_BLOCK]  = "Hello, AES demo";
    uint8_t ciphertext[AES_BLOCK] = { '\0' };
    uint8_t decrypted[AES_BLOCK]  = { '\0' };

    uint8_t plaintext2[AES_BLOCK]  = "Demo comes back";
    uint8_t ciphertext2[AES_BLOCK] = { '\0' };
    uint8_t decrypted2[AES_BLOCK]  = { '\0' };

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    int rc = mbedtls_aes_setkey_enc(&aes, dk, KEY_LEN * 8);
    if (rc != 0) {
        ESP_LOGI(TAG, "mbedtls_aes_setkey_enc не сработал: %d", rc);
        mbedtls_aes_free(&aes);
        return;
    }

    rc = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plaintext, ciphertext);
    rc |= mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plaintext2, ciphertext2);
    if (rc != 0) {
        ESP_LOGI(TAG, "mbedtls_aes_crypt_ecb не сработал: %d", rc);
        mbedtls_aes_free(&aes);
        return;
    }

    rc = mbedtls_aes_setkey_dec(&aes, dk, KEY_LEN * 8);
    if (rc != 0) {
        ESP_LOGI(TAG, "mbedtls_aes_setkey_dec не сработал: %d", rc);
        mbedtls_aes_free(&aes);
        return;
    }

    rc = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, ciphertext, decrypted);
    rc |= mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, ciphertext2, decrypted2);
    if (rc != 0) {
        ESP_LOGI(TAG, "mbedtls_aes_crypt_ecb не сработал: %d", rc);
        mbedtls_aes_free(&aes);
        return;
    }

    mbedtls_aes_free(&aes);

    char ct_hex[AES_BLOCK * 2 + 1]  = { '\0' };  // 16 байт  -> 32 hex-символов + '\0'
    _hex_to_str(ciphertext, ct_hex, AES_BLOCK);

    char ct_hex2[AES_BLOCK * 2 + 1]  = { '\0' };  // 16 байт  -> 32 hex-символов + '\0'
    _hex_to_str(ciphertext2, ct_hex2, AES_BLOCK);


    decrypted[AES_BLOCK] = '\0';  // конец строки

    ESP_LOGI(TAG, "Текст:\t%s", (char *)plaintext);
    ESP_LOGI(TAG, "Шифровка:\t%s", ct_hex);
    ESP_LOGI(TAG, "И обратно:\t%s", (char *)decrypted);

    ESP_LOGI(TAG, "Текст 2:\t%s", (char *)plaintext2);
    ESP_LOGI(TAG, "Шифровка 2:\t%s", ct_hex2);
    ESP_LOGI(TAG, "И обратно 2:\t%s", (char *)decrypted2);

}

// Точка входа
void app_main(void) {

    // Прикладной ключ на основе root-ключа
    uint8_t dk[KEY_LEN] = { 0 };
    calculate_key_with_efuse(dk);

    // Посмотрим на шифрование
    cipher_decipher(dk);

    // Через эту структуру работаем с SPI
    spi_device_handle_t spi = spi_prepare();

    // Давайте хранить данные во внешнем flash-модуле
    describe_flash_module(&spi);

}
