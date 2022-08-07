// Centrality Measures API
// COMP2521 Assignment 2

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef CENTRALITY_MEASURES_H
#define CENTRALITY_MEASURES_H

#include "Graph.h"

/**
 * Finds  the  closeness centrality for each node in the given graph and
 * returns the results in an array where index v of the  array  contains
 * the closeness centrality of node v.
 */
double *closenessCentrality(Graph g);

/**
 * Finds the betweenness centrality for each node in the given graph and
 * returns the results in an array where index v of the  array  contains
 * the betweenness centrality of node v.
 */
double *betweennessCentrality(Graph g);

#endif

