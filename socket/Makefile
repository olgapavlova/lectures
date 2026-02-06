all: reader.run
	@make server --no-print-directory

%.run: %.c
	@gcc -o $@ $^

server:
	@python3 router

msg:
	./reader.run /tmp/mesh-msg.sock

gps:
	./reader.run /tmp/mesh-gps.sock
