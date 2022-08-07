// Main program for testing nodesNotInCycle

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

void nodesNotInCycle(Graph g, int notInCycle[]);

int main(void) {
    int nV;
    if (scanf("nV: %d", &nV) != 1) {
        printf("error: failed to read nV\n");
        return 1;
    }
    
    printf("nV: %d\n\n", nV);
    
    Graph g = GraphNew(nV);
    int s, d;
    while (scanf("%d %d", &s, &d) == 2) {
        GraphInsertEdge(g, s, d);
        printf("Edge inserted: %d -> %d\n", s, d);
    }
    printf("\n");

    int *notInCycle = malloc(nV * sizeof(int));
    assert(notInCycle != NULL);
    for (int i = 0; i < nV; i++) {
        notInCycle[i] = -1;
    }

    nodesNotInCycle(g, notInCycle);

    printf("notInCycle array:\n");
    for (int i = 0; i < nV; i++) {
        printf(" node %d: %d\n", i, notInCycle[i]);
    }

    free(notInCycle);
    GraphFree(g);
}

