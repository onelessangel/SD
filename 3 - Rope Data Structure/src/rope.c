// Copyright 2021 Stroe Teodora, Bobocu Alexandra

#include "./rope.h"
#include "./utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""
#define MAX_NODES 1000
#define MIN(a, b) (a) < (b) ? (a) : (b)

char *strdup(const char *s);

RopeNode* makeRopeNode(const char* str)
{
	RopeNode *node = malloc(sizeof(*node));
	DIE(NULL == node, "node malloc failed");

	node->left = node->right = NULL;
	node->str = str;
	node->weight = strlen(str);

	return node;
}

RopeTree* makeRopeTree(RopeNode* root)
{
	RopeTree *tree = malloc(sizeof(*tree));
	DIE(NULL == tree, "tree malloc failed");

	tree->root = root;

	return tree;
}

SplitPair makeSplitPair(RopeNode *left, RopeNode *right)
{
	SplitPair sp;

	sp.left = left;
	sp.right = right;

	return sp;
}

void printRopeNode(RopeNode* rn) {
	if (!rn)
		return;

	if (!(rn->left) && !(rn->right)) {
		printf("%s", rn->str);
		return;
	}

	printRopeNode(rn->left);
	printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
	if (rt && rt->root) {
		printRopeNode(rt->root);
		printf("%s", "\n");
	}
}

void debugRopeNode(RopeNode* rn, int indent) {
	if (!rn)
		return;

	for (int i = 0; i < indent; ++i)
		printf("%s", " ");

	if (!strcmp(rn->str, EMPTY))
		printf("# %d\n", rn->weight);
	else
		printf("%s %d\n", rn->str, rn->weight);

	debugRopeNode(rn->left, indent+2);
	debugRopeNode(rn->right, indent+2);
}

int getTotalWeight(RopeNode* rt) {
	if (!rt)
		return 0;

	return rt->weight + getTotalWeight(rt->right);
}

/// Concatenates two rope trees
/// @arg: the rope trees that need to be concatenated
/// @return: rope tree having the @arg as subtrees
RopeTree* concat(RopeTree* rt1, RopeTree* rt2)
{
	if (!rt1 && !rt2) {
		return NULL;
	}

	if (!rt1 || !rt2) {
		if (rt1) {
			return rt1;
		}

		return rt2;
	}

	RopeNode *new_root = makeRopeNode(strdup(EMPTY));
	new_root->left = rt1->root;
	new_root->right = rt2->root;
	new_root->weight = getTotalWeight(rt1->root);

	RopeTree *new_tree = makeRopeTree(new_root);

	return new_tree;
}

/// Finds character located at given index in rope tree
/// @rt: original rope tree
/// @idx: searching index
/// @return: searched character
char indexRope(RopeTree* rt, int idx)
{
	RopeNode *node = rt->root;
	int count, last_weight = 0;

	// determine in which subtree is the character searched
	if (idx < node->weight) {
		count = 0;
		node = node->left;
	} else {
		count = node->weight;
		node = node->right;
	}

	// finds leaf node containing searched character
	while (node->left || node->right) {
		count += node->weight;
		last_weight = node->weight;

		if (idx >= count) {
			node = node->right;
		} else {
			count -= last_weight;
			node = node->left;
		}
	}

	return node->str[abs(idx - count)];
}

/// Searches string in rope tree, contained in given interval
/// @rt: original rope tree
/// @start: starting point of the interval
/// @end: ending point of the interval
/// @return: searched string
char* search(RopeTree* rt, int start, int end)
{
	int length = end - start;

	char *string = malloc((length + 1) * sizeof(*string));
	DIE(NULL == string, "string malloc failed");

	for (int i = start; i < end; i++) {
		string[i - start] = indexRope(rt, i);
	}
	string[length] = '\0';

	return string;
}

/// Makes copy of a rope tree using a DFS recursive algorithm
/// @arg: root node of original tree
/// @return: root node of tree copy
static RopeNode *dfsCopyRopeTree(RopeNode *rn1)
{
	if (rn1) {
		RopeNode *node = makeRopeNode(strdup(rn1->str));
		node->weight = rn1->weight;

		node->left = dfsCopyRopeTree(rn1->left);
		node->right = dfsCopyRopeTree(rn1->right);

		return node;
	}

	return NULL;
}

/// Concatenates the right subtree of current node to given tree
/// @node: parent node
/// @rn: root of right tree
/// @parent: array of parent nodes
/// @j: current index for parent array
static void concatRightTree(RopeNode *node, RopeNode **rn,
							RopeNode *parent[MAX_NODES], int j)
{
	RopeNode *rn1 = node->right;
	RopeTree *tree1 = makeRopeTree(rn1);
	RopeTree *tree2 = makeRopeTree(*rn);
	RopeTree *tree_concat = concat(tree1, tree2);
	int weight = getTotalWeight(rn1);

	*rn = tree_concat->root;

	for (int i = 0; i < j - 1; i++) {
		parent[i]->weight -= weight;
	}

	node->right = NULL;

	free(tree1);
	free(tree2);
	free(tree_concat);
}

/// Gives leaf node 2 children and splits containing string
///	@node: parent node
/// @index: index of split point of the node's containing string
static void splitNode(RopeNode *node, int index)
{
	int length = strlen(node->str);

	char *string1 = malloc ((index + 2) * sizeof(*string1));
	DIE(NULL == string1, "string1 malloc failed");

	char *string2 = malloc ((length - index) * sizeof(*string2));
	DIE(NULL == string2, "string2 malloc failed");

	strncpy(string1, node->str, index + 1);
	string1[index + 1] = '\0';

	strncpy(string2, node->str + index + 1, length - index - 1);
	string2[length - index - 1] = '\0';

	node->left = makeRopeNode(string1);
	node->right = makeRopeNode(string2);

	free((void *)node->str);
	node->str = strdup(EMPTY);

	node->weight = node->left->weight;
}

/// Splits rope tree at the provided split point, preserving original tree
/// @rt: original rope tree
/// @idx: split point
/// @return: pair of roots to computed ropes
SplitPair split(RopeTree* rt, int idx)
{
	// makes copy of the original tree
	RopeNode *copy = dfsCopyRopeTree(rt->root);

	// if split point is at the beginning/end of the string,
	// left/right rope is empty, the other rope contains the whole string
	if (!idx) {
		return makeSplitPair(makeRopeNode(strdup(EMPTY)), copy);
	} else if (idx >= getTotalWeight(rt->root)) {
		return makeSplitPair(copy, makeRopeNode(strdup(EMPTY)));
	}

	// rn2 - root of right rope tree
	// parent[] - array of parent nodes
	RopeNode *node = copy, *rn2 = NULL, *parent[MAX_NODES];
	int count, last_weight = 0, index, j = 0;

	// determine in which subtree is the split going to take place
	if (idx - 1 < node->weight) {
		parent[j++] = node;
		rn2 = node->right;
		node->right = NULL;

		count = 0;
		node = node->left;
	} else {
		count = node->weight;
		node = node->right;
	}

	// finds leaf node containing the split point
	while (node->left || node->right) {
		count += node->weight;
		last_weight = node->weight;

		if (idx - 1 >= count) {
			node = node->right;
		} else {
			parent[j++] = node;
			concatRightTree(node, &rn2, parent, j);

			count -= last_weight;
			node = node->left;
		}
	}

	// computes the index of the split point in the current string
	index = abs(idx - 1 - count);

	// if the split point is not at the end of the string,
	// the current node's string splits into two, and the right part of it
	// concatenates to the right rope tree
	if ((unsigned int)index != strlen(node->str) - 1) {
		splitNode(node, index);
		parent[j++] = node;
		concatRightTree(node, &rn2, parent, j);
	}

	return makeSplitPair(copy, rn2);
}

/// Inserts a new string in the rope tree
/// @rt: orignal rope tree
/// @idx: index of where the new string gets inserted
/// @str: new string to be inserted
/// @return: modified rope tree
RopeTree* insert(RopeTree* rt, int idx, const char* str)
{
	RopeTree *rt1, *rt2, *rt_concat_with_left, *rt_concat_final;
	RopeTree *new_tree = makeRopeTree(makeRopeNode(str));
	SplitPair sp = split(rt, idx);

	rt1 = makeRopeTree(sp.left);
	rt2 = makeRopeTree(sp.right);

	rt_concat_with_left = concat(rt1, new_tree);
	rt_concat_final = concat(rt_concat_with_left, rt2);

	free(rt1);
	free(new_tree);
	free(rt_concat_with_left);
	free(rt2);

	return rt_concat_final;
}

/// Deletes string in given interval from rope tree
/// @rt: original rope tree
/// @start: starting point of deletion
/// @len: length of string to be deleted
/// @return: modified rope tree
RopeTree* delete(RopeTree* rt, int start, int len)
{
	RopeTree *rt1, *rt2, *rt_concat_final;
	SplitPair sp_start = split(rt, start);
	SplitPair sp_end = split(rt, MIN(start + len, getTotalWeight(rt->root)));

	rt1 = makeRopeTree(sp_start.left);
	rt2 = makeRopeTree(sp_end.right);

	rt_concat_final = concat(rt1, rt2);

	free(rt1);
	free(rt2);

	return rt_concat_final;
}

// BONUS test in bonus_test.c
