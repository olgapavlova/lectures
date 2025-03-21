.PHONY: all clean clear
.SILENT: all clean clear

CC = gcc
CFLAGS = -std=c11 -O2

all: counter.c
	$(CC) $(CFLAGS) $< -o $(subst .c,.run,$<) -lpthread
	./$(subst .c,.run,$<)

clear: clean

clean:
	rm -rf *.run
