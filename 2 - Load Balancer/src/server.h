// Copyright 2021 Teodora Stroe
#ifndef SERVER_H_
#define SERVER_H_

#include "Hashtable.h"

typedef struct server_memory server_memory;
struct server_memory {
	hashtable_t *hashtable;
};

server_memory* init_server_memory();

void free_server_memory(server_memory* server);

void server_store(server_memory* server, char* key, char* value);

void server_remove(server_memory* server, char* key);

char* server_retrieve(server_memory* server, char* key);

#endif  // SERVER_H_
