.PHONY: all clear clean

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
COMMON_DIR = ../
BUILD_DIR = ../../../build/
LOG_FLAG = -DLOG_ON
DEBUG_FLAG = -g -O0
TEST_FLAG = -DCHECK_LEVEL=CK_NORMAL -lcheck -lpthread -lm

STR_C = test_str.c
STR_O = $(BUILD_DIR)callback.o $(BUILD_DIR)log.o $(BUILD_DIR)iter.o $(BUILD_DIR)regmem.o $(BUILD_DIR)str.o

all: clear check_array.run

check_array.run: $(BUILD_DIR)array.o $(BUILD_DIR)str.o $(BUILD_DIR)regmem.o $(BUILD_DIR)callback.o $(BUILD_DIR)log.o $(BUILD_DIR)iter.o $(BUILD_DIR)dict.o test_array.c
	@$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) $^ -o $@ $(TEST_FLAG)

check_dict.run: $(BUILD_DIR)str.o $(BUILD_DIR)regmem.o $(BUILD_DIR)callback.o $(BUILD_DIR)log.o $(BUILD_DIR)iter.o $(BUILD_DIR)dict.o test_dict.c
	@$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) $^ -o $@ $(TEST_FLAG)

check_str.run: $(STR_O) test_str.c
	@$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) $^ -o $@ $(TEST_FLAG)

$(BUILD_DIR)%.o: $(COMMON_DIR)%.c
	@$(CC) $(CFLAGS) $(LOG_FLAG) $(DEBUG_FLAG) -c $^ -o $@

clear:
	@rm -rf $(BUILD_DIR)*.o
	@rm -rf check_*

clean: clear
	@ls -la
