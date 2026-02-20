CC     = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c11 -O0
CLIBS  = -lcurl

all: main.run
	@./$^

%.run: %.c
	@$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)
	
