CC=gcc
CFLAGS=-Wall -Wextra -std=c99

EXEC=main

run: build
	./$(EXEC)

build: $(EXEC)

main: *.c
	$(CC) $(CFLAGS) -o $@ $^

pack:
	zip -r 311CA_StroeTeodora_Tema1.zip *.c *.h Makefile README

clean:
	rm -f $(EXEC)

.PHONY: clean main build