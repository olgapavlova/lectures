CC=gcc-14
CFLAGS=-Wall -Wextra -Wextra -g -std=gnu11 -O0
BUILD_DIR=../build/

all: luma

luma: luma.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $^

clean:
	rm -rf ../build/*
