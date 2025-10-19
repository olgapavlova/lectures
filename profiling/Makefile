CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 \
				 -O0 \
				 -g -pg

SRC = main.c
TARGET = m
CALLGRIND_OUT = callgrind.out

default: clean compile

compile:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

v:
	valgrind --tool=callgrind --callgrind-out-file=$(CALLGRIND_OUT) ./$(TARGET)
	callgrind_annotate --inclusive=yes --tree=calling --threshold=10 $(CALLGRIND_OUT)

clean:
	rm -f $(TARGET)
	rm -f gmon.out
	rm -f perf.data
	rm -f callgrind.out.*
	rm -f callgrind.out
