idf_component_register(SRCS "flash.c"
                    INCLUDE_DIRS "."
										REQUIRES
											esp_driver_spi
											spi_flash
											mbedtls
											esp_partition
											driver
											nvs_flash
											nvs_sec_provider
										)
