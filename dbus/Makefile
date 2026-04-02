CC     = gcc
CFLAGS = -Wall -Wextra -Werror \
				-std=c11

CFLAGS += $(shell pkg-config --cflags gio-2.0)
LDLIBS += $(shell pkg-config --libs gio-2.0)

%.run: %.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS) 
