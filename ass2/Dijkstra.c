// Dijkstra API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"


// Helper functions
void initialise_counters(int num_verticies, int *distance, Vertex src, bool *visited, NodeData *node_list);
PredNode *create_pred(int vertex);
void graph_traverse(Graph g, int extracted_vertex, PQ q, int *distance, NodeData *node_list);
void insert_in_pred(int distance, NodeData *node_list, int extracted_vertex, AdjList curr_adjacent, Graph g);
PredNode *insert_in_ascending(PredNode *curr_pred, PredNode *new_pred);


NodeData *dijkstra(Graph g, Vertex src) {
	// Initialising counters and lists needed to perform BFS
	int num_verticies = GraphNumVertices(g);
	int distance[num_verticies];
	bool visited[num_verticies];
	PQ q = PQNew();
	NodeData *node_list = malloc(sizeof(NodeData) * num_verticies);
	initialise_counters(num_verticies, distance, src, visited, node_list);

	// Setting the distance from src to src to 0 and inserting it into PQ
	node_list[src].dist = 0;
	PQInsert(q, src, 0);


	// Using BFS to look at paths
	while (PQIsEmpty(q) != true) {
		int extracted_vertex = PQDequeue(q);

		// If the vertex has already been visited, continue through
		if (visited[extracted_vertex] == true) {
			continue;
		}

		visited[extracted_vertex] = true;
		graph_traverse(g, extracted_vertex, q, distance, node_list);

		// Dealing with vertices not visited
		for (int i = 0; i < num_verticies; i++) {
			node_list[i].dist = distance[i];
		}
	}

	// Freeing the PQ
	PQFree(q);
	return node_list;
}

void freeNodeData(NodeData *data, int nV) {
	for (int i = 0; i < nV; i++) {
		PredNode *pred_data = data[i].pred;
		while (pred_data != NULL) {
			PredNode *temp = pred_data;
			pred_data = pred_data->next;
			free(temp);
		}
	}
	free(data);
}


////////////////////////////////////////////////////////
//====================================================//
//================ HELPER FUNCTIONS	==================//
//====================================================//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
//====================================================//
//================ dijkstra FUNCTIONS ================//
//====================================================//
////////////////////////////////////////////////////////


// initialise_counters sets the baseline for all counters that get updated

void initialise_counters(int num_verticies, int *distance, Vertex src, bool *visited, NodeData *node_list) {
	// Set all path lengths as infinite and all the previous path lengths as NULL
	// Also, we initialise all the node_list predecessor lists as NULL
	for (int i = 0; i < num_verticies; i++) {
		distance[i] = INFINITY;
		visited[i] = false;
		node_list[i].pred = NULL;
	}
	distance[src] = 0;
	return;
}


// create_pred creates a predecessor list

PredNode *create_pred(int vertex) {
	PredNode *pred_curr = malloc(sizeof(PredNode));
	pred_curr->v = vertex;
	pred_curr->next = NULL;
	return pred_curr;
}


// graph_traverse traverses through the graph using BSF, relaxing edges to update distance

void graph_traverse(Graph g, int extracted_vertex, PQ q, int *distance, NodeData *node_list) {
	AdjList curr_adjacent = GraphOutIncident(g, extracted_vertex);
	while (curr_adjacent != NULL) {
		// Relaxing the edges and replacing the distance for a vertex with a shorter path distance
		// Have to update the pred if you replace
		int curr_vertex = curr_adjacent->v;
		if (distance[extracted_vertex] + curr_adjacent->weight <= distance[curr_vertex]) {
			distance[curr_vertex] = distance[extracted_vertex] + curr_adjacent->weight;
			
			// Write a list insert function to malloc space for prednode if it hasn't been mallocd already
			insert_in_pred(distance[extracted_vertex], node_list, extracted_vertex, curr_adjacent, g);
			
			// Update the distance in node_list
			node_list[curr_vertex].dist = distance[curr_vertex];

			PQInsert(q, curr_vertex, curr_adjacent->weight);
		}

		// Do nothing else if it isn't being updated
		curr_adjacent = curr_adjacent->next;
	}
}


// insert_in_pred looks at the node_list and inserts a predecessor list

void insert_in_pred(int distance, NodeData *node_list, int extracted_vertex, AdjList curr_adjacent, Graph g) {
	int curr_vertex = curr_adjacent->v;
	// If there is no path recorded yet
	if (node_list[curr_vertex].pred == NULL) {
		node_list[curr_vertex].pred = create_pred(extracted_vertex);
	} else {
		if (node_list[curr_vertex].dist == curr_adjacent->weight + distance) {
			// Creating a new predecessor node
			PredNode *new_pred = create_pred(extracted_vertex);

			// Adding that node to the list of predecessor nodes
			PredNode *curr_pred = node_list[curr_vertex].pred;
			node_list[curr_vertex].pred = insert_in_ascending(curr_pred, new_pred);
		} else {
			// If the new path distance is smaller than the previous ones, we clear the list
			// We then create a new list from scratch with just the head
			PredNode *pred_list = node_list[curr_vertex].pred;
			while (pred_list != NULL) {
				PredNode *temp = pred_list;
				pred_list = pred_list->next;
				free(temp);
			}
			node_list[curr_vertex].pred = create_pred(extracted_vertex);
		}
	}
}


// insert_in_ascending function inserts a node in ascending order (vertex no.) and returns a list

PredNode *insert_in_ascending(PredNode *curr_pred, PredNode *new_pred) {
	// First case is if the new pred belongs at the beggining of the list
	if (new_pred->v < curr_pred->v) {
		new_pred->next = curr_pred;
		return new_pred;
	}

	// If it belongs in between 2 nodes
	PredNode *p = curr_pred;
	PredNode *prev;
	while (p->next != NULL) {
		if (new_pred->v < p->v) {
			new_pred->next = prev->next;
			prev->next = new_pred;
		} else {
			prev = curr_pred;
			p = p->next;
		}
	}

	// Belongs at either end of second last
	if (new_pred->v < p->v) {
		prev->next = new_pred;
		new_pred->next = p;
	} else {
		p->next = new_pred;
	}
	return curr_pred;
}
