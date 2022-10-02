// Copyright 2021 Teodora Stroe
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "utils.h"

#define BUCKETS 10
#define L_INIT 4

typedef struct {
	unsigned int id;
	unsigned int hash;
	int tag;
	server_memory *server;
} replica;

struct load_balancer {
	replica *replica_array;
	int elements;
	int allocs;
};


static replica* init_replica_array();
static void init_replica(replica *rep, int server_id, int tag,
						 unsigned int hash, server_memory *server);
static void free_replica_array(load_balancer *main);

static int find_server(replica *rep_array, int a, int b, unsigned int element);
static server_memory *copy_server(server_memory *server);
static void move_obj(load_balancer *main, server_memory *server);
static void rearrange_obj(load_balancer *main, server_memory *server_copy);


// server hash function
unsigned int hash_function_servers(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

load_balancer* init_load_balancer()
{
	load_balancer *loader = malloc(sizeof(*loader));
	DIE(NULL == loader, "loader malloc failed");

	loader->elements = 0;
	loader->allocs = L_INIT;
	loader->replica_array = init_replica_array();

	return loader;
}

// adds a new server to the system
void loader_add_server(load_balancer* main, int server_id)
{
	// if necessary, increases the size of the replica array
	if (main->elements + 3 > main->allocs) {
		main->allocs *= 2;

		int size = main->allocs;

		replica *temp = realloc(main->replica_array, size * sizeof(*temp));
		DIE(NULL == temp, "temp realloc failed");

		main->replica_array = temp;
	}

	int tag;
	// hash[] - contains the hashes of the replicas of the newly added server
	unsigned int hash[4], minim, maxim;
	// neighbors[] - contains the IDs of the neighbouring server replicas from
	//				 which objects might move to the newly added server
	int neighbors[3] = {-1, -1, -1};

	server_memory *server = init_server_memory();

	// for each replica, computes the hash and adds it in the sorted
	// replica array
	for (int j = 0; j < 3; j++) {
		tag = j * 100000 + server_id;
		hash[j] = hash_function_servers(&tag);

		int i = main->elements;

		init_replica(&main->replica_array[i], server_id, tag, hash[j], server);
		main->elements++;

		while (i > 0 && \
			   main->replica_array[i - 1].hash > main->replica_array[i].hash) {
			replica aux = main->replica_array[i];
			main->replica_array[i] = main->replica_array[i - 1];
			main->replica_array[i - 1] = aux;
			i--;
		}
	}

	// sorts the hash array
	minim = MIN(MIN(hash[0], hash[1]), hash[2]);
	maxim = MAX(MAX(hash[0], hash[1]), hash[2]);
	hash[1] = hash[0] + hash[1] + hash[2] - minim - maxim;

	hash[0] = minim;
	hash[2] = maxim;
	hash[3] = -1;

	int k = 0;

	// retains the index of each replica for the newly added server
	for (int i = 0; i < main->elements; i++) {
		if (main->replica_array[i].hash == hash[k]) {
			neighbors[k] = i;
			k++;
		}
	}

	// looks at each replica's next neighbour and if it's a replica of a
	// different server, retains its index
	for (int i = 0; i < 3; i++) {
		int id = main->replica_array[(neighbors[i] + 1) % main->elements].id;
		if (id != server_id) {
			neighbors[i] = (neighbors[i] + 1) % main->elements;
		} else {
			neighbors[i] = -1;
		}
	}

	// for each neighbour, checks if it hasn't been analysed already
	// if not, moves objects to the corresponding server
	for (int i = 0; i < 3; i++) {
		int j;
		if (neighbors[i] >= 0) {
			for (j = 0; j < i; j++) {
				if (neighbors[i] == neighbors[j]) {
					break;
				}
			}

			if (j == i) {
				move_obj(main, main->replica_array[neighbors[i]].server);
			}
		}
	}
}

// stores the key-value pair inside the system
void loader_store(load_balancer* main, char* key, char* value, int* server_id)
{
	unsigned int key_hash = hash_function_key(key);
	int index;

	// retains the index of the correct server replica
	index = find_server(main->replica_array, 0, main->elements - 1, key_hash);

	*server_id = main->replica_array[index].id;
	server_store(main->replica_array[index].server, key, value);
}

// gets a value associated with the key and returns it
char* loader_retrieve(load_balancer* main, char* key, int* server_id)
{
	unsigned int key_hash = hash_function_key(key);
	int index;

	// retains the index of the correct server replica
	index = find_server(main->replica_array, 0, main->elements - 1, key_hash);

	*server_id = main->replica_array[index].id;
	return server_retrieve(main->replica_array[index].server, key);
}

// removes a specific server from the system
void loader_remove_server(load_balancer* main, int server_id)
{
	int tag, k = 0;
	// hash[] - contains the hashes of the replicas of the server that will be
	//  		removed
	unsigned int hash[4], minim, maxim;
	server_memory *server_copy;

	// computes the 3 replica hashes
	for (int i = 0; i < 3; i++) {
		tag = i * 100000 + server_id;
		hash[i] = hash_function_servers(&tag);
	}

	// sorts the hash array
	minim = MIN(MIN(hash[0], hash[1]), hash[2]);
	maxim = MAX(MAX(hash[0], hash[1]), hash[2]);
	hash[1] = hash[0] + hash[1] + hash[2] - minim - maxim;

	hash[0] = minim;
	hash[2] = maxim;
	hash[3] = -1;

	// eliminates the removed server's replicas from the replica array
	for (int i = 0; i < main->elements; i++) {
		if (main->replica_array[i].hash == hash[k]) {
			if (!k) {
				server_copy = main->replica_array[i].server;
			}
			k++;
			for (int j = i; j < main->elements - 1; j++) {
				main->replica_array[j] = main->replica_array[j + 1];
			}
			main->elements--;
			i--;
		}
	}

	// redistributes the objects from the removed server
	rearrange_obj(main, server_copy);
}

void free_load_balancer(load_balancer* main)
{
	free_replica_array(main);
	free(main);
}

// creates and returns the replica array
static replica *init_replica_array()
{
	replica *replica_array = malloc(L_INIT * sizeof(*replica_array));
	DIE(NULL == replica_array, "replica array malloc failed");

	return replica_array;
}

// fills the replica element with the necessary information
static void init_replica(replica *rep, int server_id, int tag,
						 unsigned int hash, server_memory *server)
{
	rep->id = server_id;
	rep->tag = tag;
	rep->hash = hash;
	rep->server = server;
}

static void free_replica_array(load_balancer *main)
{
	for (int i = 0; i < main->elements; i++) {
		if (!(main->replica_array[i].tag / 100000)) {
			free_server_memory(main->replica_array[i].server);
		}
	}

	free(main->replica_array);
}

// finds correct server replica index using a modified binary search
static int find_server(replica *rep_array, int a, int b, unsigned int element)
{
	int no_elem = b;

	while (a < b) {
		int mid = (a + b) / 2;

		if (rep_array[mid].hash < element) {
			a = ++mid;
		} else {
			b = --mid;
		}
	}

	if (a == no_elem) {
		if (rep_array[a].hash > element) {
			return a;
		}

		return 0;
	}

	if (a > b) {
		return a;
	}

	if (rep_array[a].hash < element) {
		if (a < no_elem - 1) {
			return a + 1;
		}

		return 0;
	}

	return a;
}

// creates a copy of the given server and returns it
static server_memory *copy_server(server_memory *server)
{
	hashtable_t *ht;
	linked_list_t *bucket;
	ll_node_t *node;
	info *node_data;
	char *key, *value;

	ht = server->hashtable;

	server_memory *server_copy = init_server_memory();

	// makes a copy of the hashtable
	for (unsigned int i = 0; i < ht->hmax; i++) {
		bucket = ht->buckets[i];
		node = bucket->head;
		for (unsigned int j = 0; j < bucket->size; j++) {
			node_data = (info *)node->data;
			key = (char*)node_data->key;
			value = (char*)node_data->value;

			server_store(server_copy, key, value);

			node = node->next;
		}
	}

	return server_copy;
}

// moves objects from a server and puts them in the right one
static void move_obj(load_balancer *main, server_memory *server)
{
	// makes a copy of the current server
	server_memory *server_copy = copy_server(server);

	// empties the current server's hashtable
	ht_free(server->hashtable);

	hashtable_t *ht = ht_create(BUCKETS, hash_function_key,
								compare_function_strings);
	server->hashtable = ht;

	int k;
	linked_list_t *bucket;
	ll_node_t *node;
	info *node_data;
	char *key, *value;

	// takes each object from the server copy and puts it in the right server
	for (unsigned int i = 0; i < server_copy->hashtable->hmax; i++) {
		bucket = server_copy->hashtable->buckets[i];
		node = bucket->head;
		for (unsigned int j = 0; j < bucket->size; j++) {
			node_data = (info *)node->data;
			key = (char*)node_data->key;
			value = (char*)node_data->value;

			loader_store(main, key, value, &k);

			node = node->next;
		}
	}

	// frees the server copy
	free_server_memory(server_copy);
}

// redistributes objects from the removed server
static void rearrange_obj(load_balancer *main, server_memory *server)
{
	int k;
	hashtable_t *ht;
	linked_list_t *bucket;
	ll_node_t *node;
	info *node_data;
	char *key, *value;

	ht = server->hashtable;

	// takes each object from the removed server and puts it in the right one
	for (unsigned int i = 0; i < ht->hmax; i++) {
		bucket = ht->buckets[i];
		while (bucket->head) {
			node = ll_remove_nth_node(bucket, 0);
			node_data = (info *)node->data;
			key = (char*)node_data->key;
			value = (char*)node_data->value;

			loader_store(main, key, value, &k);

			free(node_data->key);
			free(node_data->value);
			free(node->data);
			free(node);
		}
		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);

	free(server);
}
