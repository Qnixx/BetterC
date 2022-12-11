CFLAGS = -Werror=implicit -Werror=implicit-function-declaration -Werror=implicit-int -Werror=int-conversion -Werror=incompatible-pointer-types -Werror=int-to-pointer-cast -Werror=return-type -Wunused -Iinclude/
CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/bcc: $(CFILES)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $^ -o $@
