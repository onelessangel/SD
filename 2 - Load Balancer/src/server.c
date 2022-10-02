// Copyright 2021 Teodora Stroe
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "utils.h"

#define BUCKETS 10

server_memory* init_server_memory()
{
	server_memory *server = malloc(sizeof(*server));
	DIE(NULL == server, "server malloc failed");

	hashtable_t *ht = ht_create(BUCKETS, hash_function_key,
								compare_function_strings);
	server->hashtable = ht;

	return server;
}

// stores a key-value pair to the server
void server_store(server_memory* server, char* key, char* value)
{
	hashtable_t *ht = server->hashtable;

	ht_put(ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

// gets the value associated with the key
char* server_retrieve(server_memory* server, char* key)
{
	hashtable_t *ht = server->hashtable;

	return (char *)ht_get(ht, key);
}

// removes a key-pair value from the server
void server_remove(server_memory* server, char* key)
{
	ht_remove_entry(server->hashtable, key);
}

void free_server_memory(server_memory* server)
{
	if (server->hashtable) {
		ht_free(server->hashtable);
		server->hashtable = NULL;
	}

	free(server);
}
