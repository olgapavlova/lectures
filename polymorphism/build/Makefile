NY: all clear clean
.SILENT:

CC = gcc-14
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = ../build/
LOG_FLAG = -DLOG_ON
DEBUG_FLAG = -g -O0

all: explain

hello: hello.o
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -o $(TARGET)$@ $^

explain: explain.o
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -o $(TARGET)$@ $^

mult: mult.o
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -o $(TARGET)$@ $^

%.o: ../src/%.c
	$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -o $(TARGET)$@ -c $^

clear:
	-rm -rf *.o

clean: clear
	ls -la
#include "error.h"

