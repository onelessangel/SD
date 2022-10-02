# Copyright 2021 Stroe Teodora, Bobocu Alexandra

CC=gcc
CFLAGS=-std=c11 -Wall -Wextra

TARGET=bonus_test

run: build
	./$(TARGET)

build: $(TARGET)

$(TARGET): bonus_test.c rope.c
	$(CC) $(CFLAGS) -o $@ $^

pack:
	zip -r Tema3_StroeTeodora_BobocuAlexandra.zip *.c *.h Makefile README
