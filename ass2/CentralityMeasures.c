// Centrality Measures API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"


// Helper functions for closenessCentrality

int dist_reachable(NodeData *node_list, int num_verticies);
int vertex_reachable(NodeData *node_list, int num_verticies);
double wasserman_faust(int total_dist, int vertex_reached, int num_verticies);


// Helper functions for betweennessCentrality
void initialise_betweenness(double *betweenness, int num_verticies);
double betweenness_calculation(Graph g, int num_verticies, int src);
int count_paths(NodeData *node_list, PredNode *curr_pred, int start, int num_paths);
int count_involvement(NodeData *node_list, PredNode *curr_pred, int v, int involved_paths, int start);


double *closenessCentrality(Graph g) {
	int num_verticies = GraphNumVertices(g);
	double *closeness = malloc(sizeof(double) * num_verticies);

	// Going through each source node and adding the distance between source and other vertexes
	for (int i = 0; i < num_verticies; i++) {
		NodeData *node_list = dijkstra(g, i);

		// Finding the total distance of all paths from vertex i to others
		int total_dist = dist_reachable(node_list, num_verticies);

		// Finding the amount of vertices connected to i
		int vertex_reached = vertex_reachable(node_list, num_verticies);

		// Applying the Wasserman and Faust equation and inputting value in the respective position
		double close_centrality = wasserman_faust(total_dist, vertex_reached, num_verticies);
		closeness[i] = close_centrality;
	}
	return closeness;
}

double *betweennessCentrality(Graph g) {
	// You have 2 vertex, so you need to do the equation for every pair of vertices
	// To check if a shortest path contains a certain vertex, backtrace from predecessor nodes
	// Recursion to look back at each predecessor for most recent vertex you look at
	int num_verticies = GraphNumVertices(g);
	double *betweenness = malloc(sizeof(double) * num_verticies);
	// Setting the original betweeness of every vertex to 0
	initialise_betweenness(betweenness, num_verticies);

	for (int i = 0; i < num_verticies; i++) {
		// Applying the betweenness formula
		double betweenness_node = betweenness_calculation(g, num_verticies, i);
		betweenness[i] = betweenness_node;
	}
	return betweenness;
}


////////////////////////////////////////////////////////
//====================================================//
//================ HELPER FUNCTIONS	==================//
//====================================================//
////////////////////////////////////////////////////////



////////////////////////////////////////////////////////
//====================================================//
//========== closenessCentrality FUNCTIONS ===========//
//====================================================//
////////////////////////////////////////////////////////


// dist_reachable finds the total distance between a vertex and all the other vertices

int dist_reachable(NodeData *node_list, int num_verticies) {
	int total = 0;
	for (int i = 0; i < num_verticies; i++) {
		if (node_list[i].dist != INFINITY) {
			total += node_list[i].dist;
		}
	}
	return total;
}


// vertex_reachable counts how many vertices are reachable in the graph structure

int vertex_reachable(NodeData *node_list, int num_verticies) {
	int counter = 0;
	for (int i = 0; i < num_verticies; i++) {
		if (node_list[i].dist != INFINITY) {
			counter++;
		}
	}
	return counter;
}


// wasserman_faust implements the closeness centrality formula

double wasserman_faust(int total_dist, int vertex_reached, int num_verticies) {
	// If vertex_reached == 1, it has no connections with other vertexes
	if (vertex_reached == 1) {
		double res = 0;
		return res;
	} else {
		double left_eq = (double) (vertex_reached - 1) / (num_verticies - 1);
		double right_eq = (double) (vertex_reached - 1) / total_dist;
		double res = left_eq * right_eq;
		return res;
	}
}


////////////////////////////////////////////////////////
//====================================================//
//========= betweennessCentrality FUNCTIONS ==========//
//====================================================//
////////////////////////////////////////////////////////


// initialise_betweennness initialises an array of doubles to be filled with 0s

void initialise_betweenness(double *betweenness, int num_verticies) {
	for (int i = 0; i < num_verticies; i++) {
		betweenness[i] = (double) 0;
	}
}


// betweenness_calculation calculates the betweenness for a certain vertex

double betweenness_calculation(Graph g, int num_verticies, int src) {
	double betweenness = 0;

	// According to the betweenness formula:
	// We exclude the selected vertex from being endpoints of paths
	// i represents the start of the path
	// j represents the end of the path
	for (int i = 0; i < num_verticies; i++) {
		NodeData *node_list = dijkstra(g, i);
		for (int j = 0; j < num_verticies; j++) {
			if (i != src && j != src && i != j) {
				PredNode *curr_pred = node_list[j].pred;

				// Finding the amount of shortest paths between 2 vertices
				int num_paths = 0;
				num_paths = count_paths(node_list, curr_pred, i, num_paths);

				// Finding the amount of paths that src is involved with between 2 vertices
				int involved_paths = 0;
				involved_paths = count_involvement(node_list, curr_pred, src, involved_paths, i);

				// If there is a path between the two vertexes, add to the betweenness
				if (num_paths > 0) {
					betweenness += (double) involved_paths / num_paths;
				}
			}
		}
	}
	return betweenness;
}


// count_paths counts the amount of paths between 2 vertices

int count_paths(NodeData *node_list, PredNode *curr_pred, int start, int num_paths) {
	// Treat the paths like a tree and traverse using recursion
	// Count the leaf nodes

	if (curr_pred != NULL) {
		int vertex = curr_pred->v;

		if (vertex == start) {
			num_paths++;
		} else {
			// If the vertex is not equal to the start, look at the preds of the pred (searching path)
			num_paths = count_paths(node_list, node_list[vertex].pred, start, num_paths);
		}

		// After searching a path, look at the next path
		if (curr_pred->next != NULL) {
			num_paths = count_paths(node_list, curr_pred->next, start, num_paths);
		}
	}
	return num_paths;
}


// count_involvement counts the amount of paths that a source vertex is seen between 2 vertices

int count_involvement(NodeData *node_list, PredNode *curr_pred, int v, int involved_paths, int start) {
	// Treat the paths like a tree and traverse using recursion
	// Count how many times this has appeared in the tree
	if (curr_pred != NULL) {
		int vertex = curr_pred->v;

		if (vertex == v) {
			// Count how many paths are spanned off this node
			involved_paths += count_paths(node_list, node_list[vertex].pred, start, 0);
		} else {
			// If the vertex is not equal to v, look at the preds of the pred (searching path)
			involved_paths = count_involvement(node_list, node_list[vertex].pred, v, involved_paths, start);
		}

		// After searching a path, look at the next path
		if (curr_pred->next != NULL) {
			involved_paths = count_involvement(node_list, curr_pred->next, v, involved_paths, start);
		}
	}
	return involved_paths;
}
