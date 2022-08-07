// BST definition and interface to utility functions

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef BSTREE_H
#define BSTREE_H

#define key(tree)   ((tree)->key)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

typedef int Key;

typedef struct node *BSTree;
struct node {
    int key;
    BSTree left;
    BSTree right;
};

// Creates a new empty BST
BSTree BSTreeNew(void);

// Frees all memory associated with the given BST
void BSTreeFree(BSTree);

// Prints a BST (sideways) to stdout
void BSTreeShow(BSTree);

// Inserts a new key into a BST
BSTree BSTreeInsert(BSTree, Key k);

// Creates a BST by reading integer values from a line 
BSTree BSTreeRead(char *line);

#endif

