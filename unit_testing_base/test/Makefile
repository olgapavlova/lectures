.PHONY: all clean clear
.SILENT: clear

CC = gcc
CFLAGS = -Wall -Werror -Wextra
CHECK_FLAGS = -D TEST -lcheck -lpthread -lm
BUILD_DIR = ../build/
SRC_DIR = ../src/
TEST_DIR = ./

all: env black

env: $(TEST_DIR)test_env.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $^ $(CHECK_FLAGS)
	
black: $(SRC_DIR)black.c $(TEST_DIR)test_black.c
	rm -f $(BUILD_DIR)black.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)black.c -o $(BUILD_DIR)black.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $(TEST_DIR)test_black.c $(BUILD_DIR)black.o
