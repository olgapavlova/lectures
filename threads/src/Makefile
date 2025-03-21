.PHONY: all clean clear
.SILENT: all clean clear

CC = gcc
CFLAGS = -std=c11 -O2

all: wait_sym.c
	$(CC) $(CFLAGS) $< -o $(subst .c,.run,$<)
	./$(subst .c,.run,$<)

clear: clean

clean:
	rm -rf *.run
