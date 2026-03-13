.PHONY: build flash

PORT=/dev/ttyACM0


all:
	$(MAKE) b --no-print-directory
	$(MAKE) f --no-print-directory
	$(MAKE) m --no-print-directory

b:
	idf.py build

f:
	idf.py -p $(PORT) flash

m:
	idf.py -p $(PORT) monitor

clean:
	idf.py fullclean
