// Program to test the Dijkstra API
// COMP2521 Assignment 2

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "GraphRead.h"

static void showNodeData(NodeData *d, int nV);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./testDijkstra [file]\n");
		return EXIT_FAILURE;
	}

	Graph g = readGraph(argv[1]);
	int nV = GraphNumVertices(g);
	for (int i = 0; i < nV; i++) {
		printf("Source node: %d\n", i);
		NodeData *d = dijkstra(g, i);
		showNodeData(d, nV);
		printf("\n");
		freeNodeData(d, nV);
	}
	GraphFree(g);
}

static void showNodeData(NodeData *d, int nV) {
	if (d == NULL) {
		printf("  error: node data array is NULL\n");
	}

	printf("  Distances\n");
	for (int i = 0; i < nV; i++) {
		if (d[i].dist == INFINITY) {
			printf("    %d: INF\n", i);
		} else {
			printf("    %d: %d\n", i, d[i].dist);
		}
	}

	printf("  Predecessors\n");
	for (int i = 0; i < nV; i++) {
		printf("    %d: ", i);
		for (PredNode *curr = d[i].pred; curr != NULL; curr = curr->next) {		
			printf("[%d] -> ", curr->v);
		}
		printf("NULL\n");
	}
}

