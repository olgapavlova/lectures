.PHONY = all clean

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g -O0
LOGFLAG = -DLOG_ON

all: clean errtouch.run

%.run: %.o
	$(CC) $(CFLAGS) $(LOGFLAG) $^ -o $@
	@rm -rf *.o

%.o: %.c
	$(CC) $(CFLAGS) $(LOGFLAG) -c $^ -o $(TARGET)$@

clean:
	-rm -rf *.o
	-rm -fr *.run
