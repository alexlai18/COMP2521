
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

bool check_cycle(Graph g, int index, int prev, int *visited, int num_vertices);
void reset_visited(int* visited, int num_vertices);

// Worst case time complexity of this solution: O(n^2)
void nodesNotInCycle(Graph g, int notInCycle[]) {
    int num_vertices = GraphNumVertices(g);
    int *visited = calloc(num_vertices, sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        if (check_cycle(g, i, i, visited, num_vertices)) {
            notInCycle[i] = 0;
        } else {
            notInCycle[i] = 1;
        }
        reset_visited(visited, num_vertices);
    }

}

bool check_cycle(Graph g, int index, int prev, int *visited, int num_vertices) {
    visited[index] = 1;
    for (int i = 0; i < num_vertices; i++) {
        if (GraphIsAdjacent(g, index, i)) {
            if (visited[i]) {
                if (i != prev) {
                    return true;
                }
            } else {
                if (check_cycle(g, i, index, visited, num_vertices)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void reset_visited(int* visited, int num_vertices) {
	for (int i = 0; i < num_vertices; i++) {
		visited[i] = 0;
	}

}