.PHONY: all clean clear
.SILENT: all clean clear

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -O2

all: play.c
	$(CC) $(CFLAGS) $< -o $(subst .c,.run,$<)
	./$(subst .c,.run,$<)

clear: clean

clean:
	rm -rf *.run
