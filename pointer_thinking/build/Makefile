.PHONY: all clear clean
.SILENT:

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
LOG_FLAG = -DCLOG
DEBUG_FLAG = -g -O0

SRC_DIR = ../src/

all: main.run clean

%.run: %.o
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) $^ -o $@

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -c $^ -o $@

clear:
	-rm -rf *.o

clean: clear
	ls -la
