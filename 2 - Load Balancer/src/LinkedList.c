// Copyright 2021 Teodora Stroe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t *linkedList = malloc(sizeof(*linkedList));
	DIE(NULL == linkedList, "list malloc failed");

	linkedList->head = NULL;
	linkedList->data_size = data_size;
	linkedList->size = 0;

	return linkedList;
}

ll_node_t *create_node(const void *new_data, unsigned int data_size)
{
	ll_node_t *node;

	node = malloc(sizeof(*node));
	DIE(NULL == node, "node malloc failed");

	node->data = malloc(data_size);
	DIE(NULL == node->data, "node data malloc failed");

	memcpy(node->data, new_data, data_size);

	return node;
}

ll_node_t *get_nth_node(linked_list_t *list, unsigned int n)
{
	ll_node_t *node = list->head;

	n = MIN(n, list->size - 1);

	for (unsigned int i = 0; i < n; i++) {
		node = node->next;
	}

	return node;
}

void free_node(ll_node_t *node)
{
	free(node->data);
	free(node);
}

void ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data)
{
	if (!list) {
		return;
	}

	ll_node_t *new_node = create_node(new_data, list->data_size);

	if (!n || list->size == 0) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		ll_node_t *prev_node = get_nth_node(list, n - 1);
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}
	list->size++;
}

ll_node_t *ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
	ll_node_t *removed_node;

	if (!list || !list->size) {
		return NULL;
	}

	n = MIN(list->size - 1, n);

	if (!n) {
		removed_node = list->head;
		list->head = list->head->next;
	} else {
		ll_node_t *prev_node = get_nth_node(list, n - 1);
		removed_node = prev_node->next;
		prev_node->next = removed_node->next;
	}
	list->size--;

	return removed_node;
}

unsigned int ll_get_size(linked_list_t *list)
{
	return list->size;
}

void ll_free(linked_list_t **pp_list)
{
	if (!pp_list || !*pp_list) {
		return;
	}

	while ((*pp_list)->size) {
		free_node(ll_remove_nth_node(*pp_list, 0));
	}

	free(*pp_list);
	*pp_list = NULL;
}

void ll_print_int(linked_list_t *list)
{
	ll_node_t *node = list->head;
	for (unsigned int i = 0; i < list->size; i++) {
		printf("%d ", *(int *)node->data);
		node = node->next;
	}
	printf("\n");
}

void ll_print_string(linked_list_t *list)
{
	ll_node_t *node = list->head;
	for (unsigned int i = 0; i < list->size; i++) {
		printf("%s ", (char *)node->data);
		node = node->next;
	}
	printf("\n");
}
