// Dijkstra API
// COMP2521 Assignment 2

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#include "Graph.h"

#define INFINITY INT_MAX

typedef struct PredNode {
	Vertex v;
	struct PredNode *next;
} PredNode;

typedef struct NodeData {
	int dist;
	PredNode *pred;
} NodeData;

/**
 * Finds  the shortest paths from a given source node to all other nodes
 * in the given graph. See the spec for more details.
 *
 * The  function  returns  a  NodeData  array,  with length equal to the
 * number of nodes in the graph, where index v of the array contains the
 * distance and predecessor list for node v. The predecessor lists  must
 * be in ascending order of vertex number.
 */
NodeData *dijkstra(Graph g, Vertex src);

/**
 * Frees  all  memory associated with the given NodeData array. The size
 * of the array is 'nV'.
 */
void freeNodeData(NodeData *data, int nV);

#endif

