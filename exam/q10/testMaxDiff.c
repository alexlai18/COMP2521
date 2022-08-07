// Main program for testing maxDiff

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

int maxDiff(BSTree t);

int main(int argc, char *argv[]) {
    char buffer[1024];

    char *line1 = fgets(buffer, sizeof(buffer), stdin);
    BSTree t = BSTreeRead(line1);
    printf("\nDisplaying tree (sideways)\n\n");
    BSTreeShow(t);

    int ans = maxDiff(t);
    printf("\nmaxDiff returned: %d\n", ans);

    BSTreeFree(t);
}

