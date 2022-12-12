CFLAGS = -Werror=implicit -Werror=implicit-function-declaration -Werror=implicit-int -Werror=int-conversion -Werror=incompatible-pointer-types -Werror=int-to-pointer-cast -Werror=return-type -Wunused -Iinclude/
CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/bcc: $(CFILES)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: install
install:
	sudo mkdir /usr/lib/bcc
	sudo nasm -felf64 platform/linux/amd64/_start.asm -o /lib/bcc/crt0.o
	sudo cp bin/bcc /usr/local/bin/bcc
