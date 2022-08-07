
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"
int BST_search(BSTree t, int max_diff);
int count_diff(BSTree t, int max_diff);
int tree_traversal(BSTree t, int max_diff);
int TreeNumNodes (BSTree t);

int maxDiff(BSTree t) {
    int max_diff = 0;
    max_diff = tree_traversal(t, max_diff);
    return max_diff;
}

int tree_traversal(BSTree t, int max_diff) {
    if (t != NULL) {
        BSTree temp = t;
        int diff = BST_search(temp, max_diff);
        if (max_diff < diff) {
            max_diff = diff;
        }
        max_diff = tree_traversal(t->left, max_diff);
        max_diff = tree_traversal(t->right, max_diff);
    }
    return max_diff;
}



int BST_search(BSTree t, int max_diff) {
    if (t != NULL) {
		BST_search(t->left, max_diff);
		
        int left = TreeNumNodes(t->left);
        int right = TreeNumNodes(t->right);
        int diff = abs(left - right);

        if (max_diff < diff) {
            max_diff = diff;
        }

		BST_search(t->right, max_diff);
   	}
    return max_diff;
}

int TreeNumNodes (BSTree t) {
    if (t == NULL) return 0;
    return 1 + TreeNumNodes (t->left) + TreeNumNodes (t->right);
}
