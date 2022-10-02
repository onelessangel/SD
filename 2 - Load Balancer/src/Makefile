# Copyright 2021 Teodora Stroe
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

LOAD=load_balancer
SERVER=server
REPLICA=replica
HASHMAP=Hashtable
LINKEDLIST=LinkedList

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(HASHMAP).o $(LINKEDLIST).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(HASHMAP).o: $(HASHMAP).c $(HASHMAP).h
	$(CC) $(CFLAGS) $^ -c

$(LINKEDLIST).o: $(LINKEDLIST).c $(LINKEDLIST).h
	$(CC) $(CFLAGS) $^ -c

pack:
	zip -r 311CA_StroeTeodora_Tema2.zip *.c *.h Makefile README

clean:
	rm -f *.o tema2 *.h.gch