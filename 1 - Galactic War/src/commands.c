// Copyright 2021 -- Stroe Teodora - 311CA
#include "/home/student/commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils.h"
#include "./messages.h"

// ADD command - adds a new planet to the planetary system (and its shields)
void add(cdll_t *list)
{
	unsigned int index;

	planet_t *planet = malloc(sizeof(*planet));
	DIE(NULL == planet, "planet malloc failed");
	planet->kills = 0;

	int *default_val = malloc(sizeof(*default_val));
	DIE(NULL == default_val, "defaul_val malloc failed");
	(*default_val) = 1;

	scanf("%s%d%d", planet->name, &index, &planet->no_shields);

	if ((int)index > (int)list->size) {
		err_planet_bounds();
		free(default_val);
		free(planet);
		return;
	}

	// creates planet's shield list and loads them with the default value
	planet->shields = cdll_create(sizeof(int));
	for (unsigned int i = 0; i < planet->no_shields; i++) {
		cdll_add_nth_node(planet->shields, i, default_val);
	}

	cdll_add_nth_node(list, index, planet);

	// displays welcome message
	hello_world(planet->name);

	free(default_val);
	free(planet);
}


// frees planet's shield list and the node containing said planet
static void remove_planet(cdll_node_t *node)
{
	planet_t *planet = (planet_t *)node->data;

	cdll_free(&planet->shields);
	free_node(node);
}


// BLH command - eliminates planet from the planetary system
void black_hole(cdll_t *list)
{
	unsigned int index;
	scanf("%d", &index);

	if ((int)index > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	cdll_node_t *node = cdll_remove_nth_node(list, index);
	planet_t *planet = (planet_t *)node->data;

	vortex(planet->name);
	if (node) {
		remove_planet(node);
	}
}


// UPG command - upgrades shield
void upgrade(cdll_t *list)
{
	unsigned int planet_index, shield_index, upg_value;
	scanf("%d%d%d", &planet_index, &shield_index, &upg_value);

	if ((int)planet_index > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	cdll_node_t *node = cdll_get_nth_node_c(list, planet_index);
	planet_t *planet = (planet_t *)node->data;

	if ((int)shield_index > (int)planet->no_shields - 1) {
		err_shield_bounds();
		return;
	}

	cdll_node_t *shield = cdll_get_nth_node_c(planet->shields, shield_index);

	// changes shield value
	*(int *)shield->data += upg_value;
}


// EXP command - expands planet's shield list
void expand(cdll_t *list)
{
	unsigned int planet_index;
	int *shield_value = malloc(sizeof(*shield_value));
	DIE(NULL == shield_value, "shield_value malloc failed");

	scanf("%d%d", &planet_index, shield_value);

	if ((int)planet_index > (int)list->size - 1) {
		err_planet_bounds();
		free(shield_value);
		return;
	}

	cdll_node_t *node = cdll_get_nth_node_c(list, planet_index);
	planet_t *planet = (planet_t *)node->data;

	// adds new shield at the end of the planet's shield list and updates
	// the number of shields of said planet
	cdll_add_nth_node(planet->shields, planet->no_shields, shield_value);
	planet->no_shields++;

	free(shield_value);
}


// RMV command - removes shield
void remove_shield(cdll_t *list)
{
	unsigned int planet_index, shield_index;
	scanf("%d%d", &planet_index, &shield_index);

	if ((int)planet_index > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	cdll_node_t *node = cdll_get_nth_node_c(list, planet_index);
	planet_t *planet = (planet_t *)node->data;

	if ((int)shield_index > (int)planet->no_shields - 1) {
		err_shield_bounds();
		return;
	}

	// the command can't be executed if the planet has 4 shields or less
	if (planet->no_shields <= 4) {
		err_shield_count();
		return;
	}

	// removes shield from shield list
	cdll_node_t *shield = cdll_remove_nth_node(planet->shields, shield_index);
	if (shield) {
		free_node(shield);
	}
	planet->no_shields--;
}


// COL command - two planets collide
void collision(cdll_t *list)
{
	unsigned int index1, index2;
	scanf("%d%d", &index1, &index2);

	if ((int)index1 > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	if ((int)index2 > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	cdll_node_t *node1 = cdll_get_nth_node_c(list, index1);
	planet_t *planet1 = (planet_t *)node1->data;

	cdll_node_t *node2 = node1->next;
	planet_t *planet2 = (planet_t *)node2->data;

	// sets the right collision points in the shield lists
	cdll_node_t *shield1, *shield2;
	unsigned int index_shield1 = planet1->no_shields / 4;
	unsigned int index_shield2 = (planet2->no_shields / 4) * 3;
	shield1 = cdll_get_nth_node_c(planet1->shields, index_shield1);
	shield2 = cdll_get_nth_node_c(planet2->shields, index_shield2);
	int death1 = 0, death2 = 0;

	// if the shield of a planet already has a zero value, the planet
	// implodes and is removed from the planet list

	if (!*(int *)shield1->data) {
		planet_kill(planet1->name);
		cdll_remove_node(list, index1, node1);
		remove_planet(node1);
		death1 = 1;
	}

	if (!*(int *)shield2->data) {
		planet_kill(planet2->name);
		cdll_remove_node(list, index2 - death1, node2);
		remove_planet(node2);
		death2 = 1;
	}

	if (death1 && death2) {
		return;
	}

	// if a planet still remains, its shield's value decreases and gains
	// another kill point if the other has imploded

	if (!death1) {
		(*(int *)shield1->data)--;
		if (death2) {
			planet1->kills++;
			return;
		}
	}

	if (!death2) {
		(*(int *)shield2->data)--;
		if (death1) {
			planet2->kills++;
			return;
		}
	}
}


// ROT command - rotates planet (rotates shield list)
void rotate(cdll_t *list)
{
	unsigned int index, units;
	char direction;

	scanf("%d %c%d", &index, &direction, &units);

	if ((int)index > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	if (direction == 't' || direction == 'c') {
		cdll_node_t *node = cdll_get_nth_node_c(list, index);
		planet_t *planet = (planet_t *)node->data;
		cdll_node_t *new_head;

		units = units % planet->no_shields;

		// moves head of the planet's shield list clockwise/counterclockwise
		if (direction == 't') {
			new_head = cdll_get_nth_node_c(planet->shields, units);
		} else {
			new_head = cdll_get_nth_node_t(planet->shields, units);
		}
		planet->shields->head = new_head;
	} else {
		err_direction();
		return;
	}
}


// SHW command - displays planet details
void show(cdll_t *list)
{
	unsigned int index;
	scanf("%d", &index);

	if ((int)index > (int)list->size - 1) {
		err_planet_bounds();
		return;
	}

	cdll_node_t *node = cdll_get_nth_node_c(list, index);
	planet_t *planet = (planet_t *)node->data;

	// displays planet details
	printf("NAME: %s\n", planet->name);
	if (list->size == 1) {
		printf("CLOSEST: none\n");
	} else if (list->size == 2) {
		printf("CLOSEST: %s\n", ((planet_t *)node->next->data)->name);
	} else {
		planet_t *planet_prev = (planet_t *)node->prev->data;
		planet_t *planet_next = (planet_t *)node->next->data;
		printf("CLOSEST: %s and %s\n", planet_prev->name, planet_next->name);
	}
	printf("SHIELDS: "); cdll_print_int_list(planet->shields);
	printf("KILLED: %d\n", planet->kills);
}


// frees both the shield and the planet lists
void free_everything(cdll_t **list)
{
	if (!(*list)) {
		return;
	}

	cdll_node_t *node = (*list)->head;

	for (unsigned int i = 0; i < (*list)->size; i++) {
		cdll_t *shields = ((planet_t *)node->data)->shields;

		cdll_free(&shields);
		node = node->next;
	}

	cdll_free(list);
}
