CFLAGS = -Wall -Wextra -Werror \
		 -std=c11

%.cl: CC := clang
%.cl: %.c
	$(CC) $(CFLAGS) $^ -o $*.cl

%.g: CC := gcc-15
%.g: %.c
	$(CC) $(CFLAGS) $^ -o $*.g

clean:
	rm -f *.g
	rm -f *.cl
	rm -f *.ll
	rm -f *.s
	rm -f *.o
