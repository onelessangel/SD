// Copyright 2021 Stroe Teodora, Bobocu Alexandra

#include "./rope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 16
#define MAXX 1000

char *strdup(const char *s);

RopeNode* rnallocated[MAXX];
RopeTree* rtallocated[MAXX];
int numrn = 0;
int numrt = 0;

RopeNode * checkerMakeRopeNode(const char *str) {
	rnallocated[numrn] = makeRopeNode(str);
	return rnallocated[numrn++];
}

RopeTree * checkerMakeRopeTree(RopeNode * rn) {
	rtallocated[numrt] = makeRopeTree(rn);
	return rtallocated[numrt++];
}

void freeRopeNode(RopeNode* rn) {
	if (rn == NULL)
		return;
	for (int i = 0; i < numrn; ++i)
		if (rn == rnallocated[i])
			return;

	free((void*)rn->str);
	freeRopeNode(rn->left);
	freeRopeNode(rn->right);
	free(rn);
}

int main(void)
{
	RopeTree *rt, *rt1, *rt2;
	SplitPair pair;
	int idx1 = 19, idx2 = 27, idx3 = 43, idx4 = 26;

	rt = checkerMakeRopeTree(checkerMakeRopeNode(strdup("An")));
	checkerMakeRopeNode(strdup("a "));
	checkerMakeRopeNode(strdup("a "));
	checkerMakeRopeNode(strdup("m"));
	checkerMakeRopeNode(strdup("anc"));
	checkerMakeRopeNode(strdup("at"));
	checkerMakeRopeNode(strdup(" mult"));
	checkerMakeRopeNode(strdup("e "));
	checkerMakeRopeNode(strdup("pra"));
	checkerMakeRopeNode(strdup("ji"));
	checkerMakeRopeNode(strdup("turi"));
	checkerMakeRopeNode(strdup(" si "));
	checkerMakeRopeNode(strdup("inghetata "));
	checkerMakeRopeNode(strdup("de "));
	checkerMakeRopeNode(strdup("va"));
	checkerMakeRopeNode(strdup("nilie."));

	for (int i = 1; i < NMAX; i++) {
		rt = concat(rt, makeRopeTree(rnallocated[i]));
	}

	printf("***********************Test 1***********************\n");
	printf("Expected output:\n");
	printf("Ana a mancat multe prajituri si inghetata de vanilie.\n\n");
	printf("Your output:\n");
	printRopeTree(rt);
	printf("\n");

	printf("***********************Test 2***********************\n");
	printf("Expected output:\nCharacter at index 19 is 'p'.\n\n");
	printf("Your output:\n");
	printf("Character at index %d is '%c'.\n", idx1, indexRope(rt, 19));
	printf("\n");

	printf("***********************Test 3***********************\n");
	printf("Expected output:\nCharacter at index 27 is 'i'.\n\n");
	printf("Your output:\n");
	printf("Character at index %d is '%c'.\n", idx2, indexRope(rt, 27));
	printf("\n");

	printf("***********************Test 4***********************\n");
	printf("Expected output:\nThe word is 'prajituri'.\n\n");
	printf("Your output:\n");
	printf("The word is '%s'.\n", search(rt, idx1, ++idx2));
	printf("\n");

	printf("***********************Test 5***********************\n");
	printf("Expected output:\n");
	printf("Ana a mancat multe prajituri, torturi si inghetata de vanilie.\n\n");
	printf("Your output:\n");
	rt = insert(rt, idx2--, ", torturi");
	printRopeTree(rt);
	printf("\n");

	printf("***********************Test 5***********************\n");
	printf("Expected output:\n");
	printf("Ana a mancat multe torturi si inghetata de vanilie.\n\n");
	printf("Your output:\n");
	rt = delete(rt, idx1, strlen("prajituri, "));
	printRopeTree(rt);
	printf("\n");

	printf("***********************Test 6***********************\n");
	printf("Expected output:\n");
	printf("Ana a mancat multe torturi si inghetata de ciocolata.\n\n");
	printf("Your output:\n");
	rt = delete(rt, idx3, strlen("vanilie."));
	rt = insert(rt, 1000, "ciocolata.");
	printRopeTree(rt);
	printf("\n");

	printf("***********************Test 7***********************\n");
	printf("Expected output:\n");
	printf("Ana a mancat multe torturi\nsi inghetata de ciocolata.\n\n");
	printf("Your output:\n");
	pair = split(rt, idx4);
	rt1 = makeRopeTree(pair.left);
	printRopeTree(rt1);
	rt2 = makeRopeTree(pair.right);
	rt2 = delete(rt2, 0, 1);
	printRopeTree(rt2);
	printf("\n");

	freeRopeNode(pair.left);
	free(rt1);
	freeRopeNode(pair.right);
	free(rt2);

	for (int i = 0; i < numrn; i++) {
		free((void*)rnallocated[i]->str);
		free(rnallocated[i]);
	}

	for (int i = 0; i < numrt; i++) {
		free(rtallocated[i]);
	}

	return 0;
}
