// Implementation of BST utility functions

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BSTree.h"

/*
#define key(tree)   ((tree)->key)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

struct node {
    int key;
    BSTree left;
    BSTree right;
};
*/

static BSTree newNode(int k);
static void doBSTreeShow(BSTree t, int depth);

// Creates a new empty BST
BSTree BSTreeNew(void) {
    return NULL;
}

// Frees all memory associated with the given BST
void BSTreeFree(BSTree t) {
    if (t != NULL) {
        BSTreeFree(left(t));
        BSTreeFree(right(t));
        free(t);
    }
}

// Prints a BST (sideways) to stdout
void BSTreeShow(BSTree t) {
    doBSTreeShow(t, 0);
}

static void doBSTreeShow(BSTree t, int depth) {
    if (t != NULL) {
        doBSTreeShow(right(t), depth + 1);
        for (int i = 0; i < depth; i++) {
            putchar('\t');
        }
        printf("%d\n", key(t));
        doBSTreeShow(left(t), depth + 1);
    }
}

// Inserts a new key into a BST
BSTree BSTreeInsert(BSTree t, int k) {
    if (t == NULL) {
        t = newNode(k);
    } else if (k < key(t)) {
        left(t) = BSTreeInsert(left(t), k);
    } else if (k > key(t)) {
        right(t) = BSTreeInsert(right(t), k);
    }
    return t;
}

// Creates a new node containing the given key
static BSTree newNode(int k) {
    BSTree new = malloc(sizeof(*new));
    assert(new != NULL);
    key(new) = k;
    left(new) = NULL;
    right(new) = NULL;
    return new;
}

// Creates a BST by reading integer values from a line 
BSTree BSTreeRead(char *line) {
    char delim[] = ", ";
    int key;

    BSTree t = BSTreeNew(); 

    char *tkn = strtok(line, delim);

    while (tkn != NULL) {
        int count = sscanf(tkn, "%d", &key);
        if (count == 1) {
            t = BSTreeInsert(t, key);
        }
        tkn = strtok(NULL, delim);
    }

    return t;
}

