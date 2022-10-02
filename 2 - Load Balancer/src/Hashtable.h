// Copyright 2021 Teodora Stroe
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "LinkedList.h"

typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;  // array of liked list
	unsigned int size;  // number of nodes in all of the buckets
	unsigned int hmax;  // number of buckets
	unsigned int (*hash_function)(void*);  // pointer to key hash function
	int (*compare_function)(void*, void*);  // pointer to key compare function
};

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*));

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
			void *value, unsigned int value_size);

void *ht_get(hashtable_t *ht, void *key);

int ht_has_key(hashtable_t *ht, void *key);

void ht_remove_entry(hashtable_t *ht, void *key);

unsigned int ht_get_size(hashtable_t *ht);

unsigned int ht_get_hmax(hashtable_t *ht);

void ht_free(hashtable_t *ht);

int compare_function_strings(void *a, void *b);

unsigned int hash_function_key(void *a);

#endif  // HASHTABLE_H_
