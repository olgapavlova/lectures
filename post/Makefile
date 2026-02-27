CC     = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c11 -O0
CLIBS  = -lcurl

all: main.run
	@./$^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.run: parson.o %.c
	@$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean:
	@rm *.o
	@rm *.run
	
