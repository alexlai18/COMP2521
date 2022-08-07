
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
bool in_over_out(Graph g, int src, int num_vertices);
void path_exists(Graph g, int src, int dest, int num_vertices, int* visited);
void reset_visited(int* visited, int num_vertices, int start);

// Worst case time complexity of this solution: O(N + E), where N is num of vertices and E is num of edges
void findPopularFollowers(Graph g, int src, int followers[]) {
	int num_vertices = g->nV;
	int *visited = calloc(num_vertices, sizeof(int));

	for (int i = 0; i < num_vertices; i++) {
		reset_visited(visited, num_vertices, i);
		// A node is always a popular follower of itself if in > out
		if (i == src && in_over_out(g, i, num_vertices)) {
			followers[i] = 1;
		} else if (i != src) {
			path_exists(g, i, src, num_vertices, visited);
			if (visited[src] == 1 && in_over_out(g, i, num_vertices)) {
				followers[i] = 1;
			}
		}
	}
	free(visited);
}

bool in_over_out(Graph g, int src, int num_vertices) {
	int in_count = 0;
	int out_count = 0;
	for (int i = 0; i < num_vertices; i++) {
		if (GraphIsAdjacent(g, src, i) == true) {
			out_count++;
		} 

		if (GraphIsAdjacent(g, i, src) == true) {
			in_count++;
		}
	}
	if (in_count > out_count) {
		return true;
	} else {
		return false;
	}
}

void path_exists(Graph g, int src, int dest, int num_vertices, int* visited) {
	// Using DFS to find a path
	for (int i = 0; i < num_vertices; i++) {
		if (GraphIsAdjacent(g, src, i) && visited[i] == 0) {
			visited[i] = 1;
			if (i == dest) {
				return;
			}
			path_exists(g, i, dest, num_vertices, visited);
		}
	}
}

void reset_visited(int* visited, int num_vertices, int start) {
	for (int i = 0; i < num_vertices; i++) {
		visited[i] = 0;
	}
	visited[start] = 1;
}