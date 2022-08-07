// Program to test the CentralityMeasures API
// COMP2521 Assignment 2

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CentralityMeasures.h"
#include "Graph.h"
#include "GraphRead.h"

static void printUsage(void);
static void showCentralityMeasures(double *measures, int nV);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printUsage();
		return EXIT_FAILURE;
	}

	double *(*fn)(Graph) = NULL;

	Graph g = readGraph(argv[1]);

	if (strcmp(argv[2], "c") == 0) {
		fn = closenessCentrality;
	} else if (strcmp(argv[2], "b") == 0) {
		fn = betweennessCentrality;
	} else {
		printUsage();
	}

	if (fn != NULL) {
		double *measures = fn(g);
		showCentralityMeasures(measures, GraphNumVertices(g));
		free(measures);
	}

	GraphFree(g);
}

static void printUsage(void) {
	printf("Usage: ./testCentralityMeasures [file] [flag]\n");
	printf("Valid Flags:\n");
	printf("    c    closeness centrality\n");
	printf("    b    betweenness centrality\n");
}

static void showCentralityMeasures(double *measures, int nV) {
	if (measures == NULL) {
		printf("error: measures array is NULL\n");
		return;
	}
	
	for (int i = 0; i < nV; i++) {
		printf("%d: %lf\n", i, measures[i]);
	}
}

