// Copyright 2021 -- Stroe Teodora - 311CA
#include "/home/student/messages.h"
#include <stdio.h>

void hello_world(char *planet_name)
{
	printf("The planet %s has joined the galaxy.\n", planet_name);
}

void err_planet_bounds()
{
	printf("Planet out of bounds!\n");
}

void vortex(char *planet_name)
{
	printf("The planet %s has been eaten by the vortex.\n", planet_name);
}

void err_shield_bounds()
{
	printf("Shield out of bounds!\n");
}

void err_shield_count()
{
	printf("A planet cannot have less than 4 shields!\n");
}

void planet_kill(char *planet_name)
{
	printf("The planet %s has imploded.\n", planet_name);
}

void err_direction()
{
	printf("Not a valid direction!\n");
}
