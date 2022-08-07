// Program to test the LanceWilliamsHAC API
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"
#include "GraphRead.h"
#include "LanceWilliamsHAC.h"

static Tree printDendrogram(Dendrogram d, int depth);
static void printClusters(Tree t, int depth);
static void freeDendrogram(Dendrogram d);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: ./testLanceWilliamsHAC [file]\n");
		return EXIT_FAILURE;
	}

	Graph g = readGraph(argv[1]);

	Dendrogram d = LanceWilliamsHAC(g, SINGLE_LINKAGE);
	Tree allTree = printDendrogram(d, 1);
	printClusters(allTree, 0);
	TreeFree(allTree);
	freeDendrogram(d);

	GraphFree(g);
}

static Tree printDendrogram(Dendrogram d, int depth) {
	// To avoid infinite looping, due to a possible  
	// incorrect logic in the program being tested!
	assert(depth < 200);

	Tree tr = NULL;
	Tree tl = NULL;

	if (d == NULL) {
		return NULL;
	}

	if (d->left == NULL && d->right == NULL) {
		Tree t = TreeNew();
		t = TreeInsert(t, d->vertex);
		return t;
	}

	if (d->left != NULL) {
		tl = printDendrogram(d->left, depth + 1);
		printClusters(tl, depth);
	}

	if (d->right != NULL) {
		tr = printDendrogram(d->right, depth + 1);
		printClusters(tr, depth);
	}

	Tree bothTrees = TreeAdd(tl, tr);
	TreeFree(tr);
	return bothTrees;
}

static void printClusters(Tree t, int depth) {
	// To avoid infinite looping, due to a possible  
	// incorrect logic in the program being tested!
	assert(depth < 200);

	printf("%d: {", depth);
	TreePrint(t);
	printf("}");

	if ((TreeGetLeft(t) == NULL) && (TreeGetRight(t) == NULL)) {
		printf(" (leaf)");
	}
	printf("\n");
}

static void freeDendrogram(Dendrogram d) {
	if (d != NULL) {
		freeDendrogram(d->left);
		freeDendrogram(d->right);
		free(d);
	}
}

