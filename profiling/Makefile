CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 \
				 -O0 \
				 -pg -g

SRC = main.c
TARGET = m

default: clean compile

compile:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)


clean:
	rm -f $(TARGET)
	rm -f gmon.out
	rm -f perf.data
