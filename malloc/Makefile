CC = gcc
CFLAGS = -std=c11 \
				 -O0 -g

%.r: %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.out
	rm -f *.r
