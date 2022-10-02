// Copyright 2021 -- Stroe Teodora - 311CA
#ifndef _HOME_STUDENT_CDLL_H_
#define _HOME_STUDENT_CDLL_H_

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define NMAX 30

typedef struct cdll_node_t cdll_node_t;
struct cdll_node_t {
	void *data;
	cdll_node_t *prev, *next;
};


typedef struct cdll_t cdll_t;
struct cdll_t {
	cdll_node_t *head;
	unsigned int data_size;
	unsigned int size;
};


typedef struct {
	char name[NMAX];
	unsigned int no_shields;
	cdll_t *shields;
	unsigned int kills;
} planet_t;


cdll_t *cdll_create(unsigned int data_size);

cdll_node_t *create_node(const void *new_data, unsigned int data_size);

cdll_node_t *cdll_get_nth_node_c(cdll_t *list, unsigned int n);

cdll_node_t *cdll_get_nth_node_t(cdll_t *list, unsigned int n);

void free_node(cdll_node_t *node);

void cdll_add_nth_node(cdll_t *list, unsigned int n, const void *data);

void cdll_remove_node(cdll_t* list, unsigned int n, cdll_node_t *node);

cdll_node_t *cdll_remove_nth_node(cdll_t *list, unsigned int n);

void cdll_free(cdll_t **pp_list);

void cdll_print_int_list(cdll_t *list);

#endif  // _HOME_STUDENT_CDLL_H_
