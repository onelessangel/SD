// Copyright 2021 Teodora Stroe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"
#include "utils.h"

#define MAX_BUCKET_SIZE 64

// key compare function
int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

// key hash function
unsigned int hash_function_key(void *a) {
	unsigned char *puchar_a = (unsigned char *) a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
					   int (*compare_function)(void*, void*))
{
	hashtable_t *hashtable = malloc(sizeof(*hashtable));
	DIE(NULL == hashtable, "hashtable malloc failed");

	linked_list_t **buckets = malloc(hmax * sizeof(*buckets));
	DIE(NULL == buckets, "buckets malloc failed");

	for (unsigned int i = 0; i < hmax; i++) {
		buckets[i] = ll_create(MAX_BUCKET_SIZE);
	}

	hashtable->buckets = buckets;
	hashtable->size = 0;
	hashtable->hmax = hmax;
	hashtable->hash_function = hash_function;
	hashtable->compare_function = compare_function;

	return hashtable;
}

static ll_node_t *find_key(linked_list_t *bucket, void *key,
	int (*compare_function)(void*, void*), unsigned int *pos)
{
	ll_node_t *node = bucket->head;

	for (unsigned int i = 0; i < bucket->size; i++) {
		if (compare_function(((info *)node->data)->key, key) == 0) {
			*pos = i;
			return node;
		}
		node = node->next;
	}

	return NULL;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
			void *value, unsigned int value_size)
{
	info *data = malloc(sizeof(*data));
	DIE(NULL == data, "data malloc failed");

	unsigned int pos;
	unsigned int index = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[index];
	ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);

	if (!node) {  // the key hasn't been found in the list
		data->key = malloc(key_size);
		DIE(NULL == data->key, "key malloc failed");

		memcpy(data->key, key, key_size);

		data->value = malloc(value_size);
		DIE(NULL == data->value, "value malloc failed");

		memcpy(data->value, value, value_size);

    	ll_node_t *new_node;

		if (bucket == NULL) {
       		return;
    	}

		new_node = malloc(sizeof(*new_node));
    	DIE(new_node == NULL, "new_node malloc failed");

		new_node->data = malloc(sizeof(*data));
		DIE(new_node->data == NULL, "new_node->data malloc failed");

		((info *)new_node->data)->key = malloc(key_size);
		DIE(NULL == ((info *)new_node->data)->key, "key malloc failed");

		((info *)new_node->data)->value = malloc(value_size);
		DIE(NULL == ((info *)new_node->data)->value, "value malloc failed");

		memcpy(((info *)new_node->data)->key, data->key, key_size);
		memcpy(((info *)new_node->data)->value, data->value, value_size);

		new_node->next = bucket->head;
		bucket->head = new_node;
		bucket->size++;
		ht->size++;

		free(data->key);
		free(data->value);
	} else {  // the key has been found
		free(((info *)node->data)->value);

		((info *)node->data)->value = malloc(value_size);
		DIE(NULL == ((info *)node->data)->value, "value malloc failed");

		memcpy(((info *)node->data)->value, value, value_size);
	}

	free(data);
}

void *ht_get(hashtable_t *ht, void *key)
{
	unsigned int index = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[index];
	ll_node_t *node;
	unsigned int pos;

	node = find_key(bucket, key, ht->compare_function, &pos);
	if (node) {
		return ((info *)node->data)->value;
	}

	return NULL;
}

// returns 1 if key has had a value associated in the hashtable, 0 otherwise
int ht_has_key(hashtable_t *ht, void *key)
{
	linked_list_t *bucket;
	ll_node_t *node;
	unsigned int pos;

	for (unsigned int i = 0; i < ht->hmax; i++) {
		bucket = ht->buckets[i];
		node = find_key(bucket, key, ht->compare_function, &pos);
		if (node) {
			return 1;
		}
	}

	return 0;
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	unsigned int pos;
	unsigned int index = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[index];
	ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);

	if (node) {
		ll_remove_nth_node(bucket, pos);
		free(((info *)node->data)->key);
		free(((info *)node->data)->value);
		free(node->data);
		free(node);
	}
}

void ht_free(hashtable_t *ht)
{
	linked_list_t *bucket;
	ll_node_t *node;

	for (unsigned int i = 0; i < ht->hmax; i++) {
		bucket = ht->buckets[i];
		while (bucket->head) {
			node = ll_remove_nth_node(bucket, 0);
			free(((info *)node->data)->key);
			free(((info *)node->data)->value);
			free(node->data);
			free(node);
		}
		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);
}

unsigned int ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
