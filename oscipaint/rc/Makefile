# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
# Кросс-компиляция и заливка кода в ATmega328p (Arduino UNO R3)
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

# Общие настройки make
.PHONY: all flash clean
.SILENT: clean

# Что компилируем
TARGET  = main

# Настройки микроконтроллера
MCU     = atmega328p
F_CPU   = 16000000UL

# Настройки кросс-компилятора и других инструментов
CC      = avr-gcc
OBJCOPY = avr-objcopy
DRIVER  = avrdude
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99 -Os -Wall
LDFLAGS = -mmcu=$(MCU) -Wl,--no-gc-sections
BAUD    = 115200
PORT    = /dev/ttyACM0

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  

# Цель по умолчанию
all: clean $(TARGET).hex flash

# Сборка исполняемых файлов формата компьютера
%.elf: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Конвертация формата компьютера в формат микроконтроллера
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Запись кода в микроконтроллер
flash: $(TARGET).hex
	$(DRIVER) -v -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -D -U flash:w:$<

# Очистка лишнего
clean:
	rm -f *.elf *.hex *.o
	clear
	ls -alF --color=auto
