MCU=attiny85
F_CPU=16500000
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os

all: main.hex

main.elf: main.c
	$(CC) $(CFLAGS) main.c -o main.elf

main.hex: main.elf
	$(OBJCOPY) -O ihex main.elf main.hex

flash: main.hex
	micronucleus main.hex

clean:
	rm -f *.elf *.hex	
