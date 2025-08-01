# ─────────────────────────────────────
# Конфигурация (можно переопределять)
# ─────────────────────────────────────
MCU     ?= attiny13
F_CPU   ?= 9600000UL
TARGET  ?= main

BUILD_DIR = build
SRC_DIR   = src

CC       = avr-gcc
OBJCOPY  = avr-objcopy
SIZE     = avr-size
CFLAGS   = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=c99
LDFLAGS  = -mmcu=$(MCU)

SRC      = $(wildcard $(SRC_DIR)/*.c)
OBJ      = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
ELF      = $(BUILD_DIR)/$(TARGET).elf
HEX      = $(BUILD_DIR)/$(TARGET).hex

DOCKER_IMAGE ?= attiny13a-builder
DOCKER_TAG   ?= latest

all: docker-run

# ─────────────────────────────────────
# Цели для обслуживания Docker
# ─────────────────────────────────────
docker-build:
	sudo docker build -t $(DOCKER_IMAGE):$(DOCKER_TAG) .


# ─────────────────────────────────────
# Цели компиляции и загрузки
# ─────────────────────────────────────
compile: $(HEX)
	$(SIZE) $(ELF)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(HEX)
	sudo avrdude -c usbasp -p t13 -U flash:w:$(HEX)


docker-run:
	docker run --rm \
		-v "$$(pwd)":/project \
		-e HOST_UID=$$(id -u) \
		-e HOST_GID=$$(id -g) \
		$(DOCKER_IMAGE):$(DOCKER_TAG) $(MAKE) compile


# ─────────────────────────────────────
# Служебные цели 
# ─────────────────────────────────────
clean:
	rm -rf $(BUILD_DIR)

report: $(ELF)
	@echo "──────────────────────────────"
	@echo "🔍 Размер прошивки:"
	$(SIZE) -C --mcu=$(MCU) $(ELF)

	@echo
	@echo "🔎 Символы в ELF:"
	avr-nm -n $(ELF) | head -20

	@echo
	@echo "📦 Секции ELF:"
	avr-objdump -h $(ELF)

	@echo
	@echo "🧠 Дизассемблирование:"
	avr-objdump -d $(ELF) | head -40

.PHONY: all docker-run compile report clean flash
