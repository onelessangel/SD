// Copyright 2021 -- Stroe Teodora - 311CA
#include "/home/student/cdll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils.h"

// creates new generic circular doubly linked list
cdll_t *cdll_create(unsigned int data_size)
{
	cdll_t *list = malloc(sizeof(*list));
	DIE(NULL == list, "list malloc failed");

	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;

	return list;
}


// creates new node containing given data
cdll_node_t *create_node(const void *new_data, unsigned int data_size)
{
	cdll_node_t *node = malloc(sizeof(*node));
	DIE(NULL == node, "node malloc failed");

	node->data = malloc(data_size);
	DIE(NULL == node->data, "node data malloc failed");

	memcpy(node->data, new_data, data_size);

	return node;
}


// returns the node located on the nth position in given list, going clockwise
cdll_node_t *cdll_get_nth_node_c(cdll_t *list, unsigned int n)
{
	n = n % list->size;
	cdll_node_t *node = list->head;

	if (n <= list->size / 2) {
		for (unsigned int i = 0; i < n; i++) {
			node = node->next;
		}
	} else {
		for (unsigned int i = 0; i < list->size - n; i++) {
			node = node->prev;
		}
	}

	return node;
}


// returns the nth node in given list, going counterclockwise
cdll_node_t *cdll_get_nth_node_t(cdll_t *list, unsigned int n)
{
	n = n % list->size;
	cdll_node_t *node = list->head;

	for (unsigned int i = 0; i < n; i++) {
		node = node->prev;
	}

	return node;
}


// frees given node
void free_node(cdll_node_t *node)
{
	free(node->data);
	free(node);
}


// adds node on the nth position in given list
void cdll_add_nth_node(cdll_t *list, unsigned int n, const void *data)
{
	if (!list) {
		return;
	}

	cdll_node_t *new_node = create_node(data, list->data_size);

	if (!list->size) {
		new_node->next = new_node;
		new_node->prev = new_node;
		list->head = new_node;
	} else if (!n) {
		new_node->next = list->head;
		new_node->prev = list->head->prev;
		list->head->prev->next = new_node;
		list->head->prev = new_node;
		list->head = new_node;
	} else {
		cdll_node_t *prev_node;
		prev_node = cdll_get_nth_node_c(list, MIN(n - 1, list->size - 1));

		new_node->next = prev_node->next;
		new_node->prev = prev_node;
		prev_node->next->prev = new_node;
		prev_node->next = new_node;
	}
	list->size++;
}


// removes given node from the list
void cdll_remove_node(cdll_t* list, unsigned int n, cdll_node_t *node)
{
	if (!n) {
		list->head = node->next;
	}
	list->size--;

	if (!list->size) {
		list->head = NULL;
		return;
	}

	node->prev->next = node->next;
	node->next->prev = node->prev;
}


// removes node located on the nth position in list
cdll_node_t *cdll_remove_nth_node(cdll_t* list, unsigned int n)
{
	if (!list || !list->size) {
		return NULL;
	}

	cdll_node_t *removed_node;
	removed_node = cdll_get_nth_node_c(list, MIN(n, list->size - 1));

	cdll_remove_node(list, n, removed_node);

	return removed_node;
}


// frees list
void cdll_free(cdll_t **pp_list)
{
	if (!pp_list || !*pp_list) {
		return;
	}

	cdll_node_t *node;

	while ((*pp_list)->size) {
		node = cdll_remove_nth_node(*pp_list, 0);
		if (node) {
			free_node(node);
		}
	}

	if (*pp_list) {
		free(*pp_list);
	}
}


// prints to stdout a list of int type data, used in displaying the shields
void cdll_print_int_list(cdll_t *list)
{
	cdll_node_t *node = list->head;

	for (unsigned int i = 0; i < list->size; i++) {
		printf("%d ", *(int *)node->data);
		node = node->next;
	}
	printf("\n");
}
