.PHONY: all
.SILENT: all main.run

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: main.run
	./main.run

main.run: main.c main.h
	$(CC) $(CFLAGS) $< -o $@
