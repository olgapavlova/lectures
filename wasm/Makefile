fib: fib.c
	clang \
			--target=wasm32 \
			-nostdlib \
			-Wl,--no-entry \
			-Wl,--export-all \
			-o fib.wasm \
			fib.c

s:
	python3 -m http.server 8000


hello: hello.c
	emcc \
		-O0 \
		-s STANDALONE_WASM=1 \
		-s ERROR_ON_UNDEFINED_SYMBOLS=0 \
		-Wl,--export=hello \
		-o hello.wasm \
		hello.c
