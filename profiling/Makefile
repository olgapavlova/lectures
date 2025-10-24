CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 \
				 -O0 \
				 -g -pg

SRC = matrix main
SRC_O = $(addsuffix .o,$(SRC))
TARGET = m.run
CALLGRIND_OUT = callgrind.out

default: clean compile

compile: $(SRC_O)
	$(CC) $(CFLAGS) $(SRC_O) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

v:
	valgrind --tool=callgrind --callgrind-out-file=$(CALLGRIND_OUT) ./$(TARGET)

c:
	callgrind_annotate --inclusive=yes --tree=calling --threshold=10 $(CALLGRIND_OUT)

pl:
	sudo perf stat -e L1-dcache-loads,L1-dcache-load-misses ./$(TARGET)

pp:
	sudo perf stat ./$(TARGET)

p:
	sudo perf record -g ./$(TARGET)
	sudo perf annotate

clean:
	rm -f $(TARGET)
	rm -f gmon.out
	rm -f perf.data
	rm -f callgrind.out.*
	rm -f callgrind.out
	rm -f *.o
	rm -f *.run
