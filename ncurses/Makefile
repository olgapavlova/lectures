CC     = gcc
CFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 
CLIBS  = -lncursesw

all: colors.run
	./$^

%.run: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)
