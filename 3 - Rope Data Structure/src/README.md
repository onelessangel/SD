Copyright Teodora Stroe, Alexandra Bobocu 311CA 2021

# The following operations have been implemented:

## 1. Concat - concat()    - concatenates two rope trees

## 2. Index  - indexRope() - finds character located at given index in rope tree

- determine in which subtree is the character searched
- finds leaf node containing searched character

## 3. Search - search()    - finds string in rope tree, contained in given interval

## 4. Split  - split()     - splits rope tree at the provided split point

- makes copy of the original tree using dfsCopyRopeTree() function
- determine in which subtree is the split going to take place
- finds leaf node containing the split point
- computes the index of the split point in the current string
- if the split point is not at the end of the string, the current node's
	   string splits into two, and the right part of it concatenates to the
	   right rope tree.
	   This step uses the splitNode() and concatRightTree() functions.
- Uses makeSplitPair() function to generate the output.

## 5. Insert - insert()    - inserts a new string in the rope tree

## 6. Delete - delete()    - deletes string in given interval from rope tree

# Feedback:

### PROS:

- helped in understanding better tree operations
- provided a very helpful real life example of tree data structure usage
- medium dificulty level

### CONS:

- given tests lack any kind of comments
- no documentation provided besides the assignment's description
	  	-- strdup()'s behaviour raised some difficulties in understanding