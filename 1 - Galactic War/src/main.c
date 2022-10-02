// Copyright 2021 -- Stroe Teodora - 311CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./cdll.h"
#include "./commands.h"

int main(void)
{
	int n;
	char command[4];
	cdll_t *planetary_sys = cdll_create(sizeof(planet_t));

	scanf("%d ", &n);

	for (int i = 0; i < n; i++) {
		scanf("%s ", command);

		if (strcmp(command, "ADD") == 0) {
			add(planetary_sys);
		} else if (strcmp(command, "BLH") == 0) {
			black_hole(planetary_sys);
		} else if (strcmp(command, "UPG") == 0) {
			upgrade(planetary_sys);
		} else if (strcmp(command, "EXP") == 0) {
			expand(planetary_sys);
		} else if (strcmp(command, "RMV") == 0) {
			remove_shield(planetary_sys);
		} else if (strcmp(command, "COL") == 0) {
			collision(planetary_sys);
		} else if (strcmp(command, "ROT") == 0) {
			rotate(planetary_sys);
		} else if (strcmp(command, "SHW") == 0) {
			show(planetary_sys);
		}
	}

	free_everything(&planetary_sys);
	return 0;
}
