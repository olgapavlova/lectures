idf_component_register(SRCS "flash.c"
                    INCLUDE_DIRS "."
										REQUIRES
											esp_driver_spi
											spi_flash
											mbedtls
										)
