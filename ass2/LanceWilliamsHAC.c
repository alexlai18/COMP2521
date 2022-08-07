// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INFINITY DBL_MAX

// Helper functions for LanceWilliamsHAC
// Initialising functions
void initialise_dendA(Dendrogram *dendA, int num_clusters);
void initialise_dist(double (*dist), int num_clusters, Graph g, int row);

// Calculating functions
int find_edge_weight(Graph g, int src, int dest);

// Adding functions
void add_cluster_single(double (*dist), int num_clusters, int cluster_1, int cluster_2);
void add_cluster_complete(double (*dist), int num_clusters, int cluster_1, int cluster_2);
void create_dendrogram(Dendrogram *dendA, int num_clusters, int cluster_1, int cluster_2);

// Altering functions
void alter_dendA(Dendrogram *dendA, int cluster_1, int cluster_2, int num_clusters);

// Removing functions
void remove_cluster_col(double *(dist), int cluster_1, int cluster_2, int num_clusters);

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int num_clusters = GraphNumVertices(g);

    // Initialise clusters as just an array of Dendogram nodes with vertex 0 - num_clusters
    Dendrogram dendA[num_clusters + 1];
    initialise_dendA(dendA, num_clusters);

    // Initialises an array which contains the distance between 2 clusters
    double dist[num_clusters + 1][num_clusters + 1];
    for (int i = 0; i < num_clusters; i++) {
        initialise_dist(dist[i], num_clusters, g, i);
    }

    while (num_clusters > 2) {

        // Find the shortest distance between clusters, and the cluster numbers
        double min_dist = INFINITY;
        int cluster_1;
        int cluster_2;
        for (int i = 0; i < num_clusters; i++) {
            for (int j = 0; j < num_clusters; j++) {
                if (dist[i][j] < min_dist) {
                    min_dist = dist[i][j];
                    cluster_1 = i;
                    cluster_2 = j;
                }
            }
        }


        // Adding a new cluster to the end
        if (method == SINGLE_LINKAGE) {
            // Adding a new column for new cluster
            for (int i = 0; i < num_clusters; i++) {
                add_cluster_single(dist[i], num_clusters, cluster_1, cluster_2);
            }

            // Adding a new row for the new cluster
            for (int i = 0; i < num_clusters; i++) {
                dist[num_clusters][i] = dist[i][num_clusters];
            }
            dist[num_clusters][num_clusters] = INFINITY;

        } else if (method == COMPLETE_LINKAGE) {
            // Adding a new column for new cluster
            for (int i = 0; i < num_clusters; i++) {
                add_cluster_complete(dist[i], num_clusters, cluster_1, cluster_2);
            }
            // Adding a new row for the new cluster
            for (int i = 0; i < num_clusters; i++) {
                dist[num_clusters][i] = dist[i][num_clusters];
            }
            dist[num_clusters][num_clusters] = INFINITY;
        }
        num_clusters++;

        // Removing merged cluster rows and columns
        for (int i = 0; i < num_clusters; i++) {
            remove_cluster_col(dist[i], cluster_1, cluster_2, num_clusters);
        }

        if (cluster_2 != cluster_1 + 1) {
            // Shifting all rows up
            for (int i = cluster_1; i < num_clusters - 1; i++) {
                for (int j = 0; j < num_clusters; j++) {
                    dist[i][j] = dist[i + 1][j];
                }
            }
            for (int i = cluster_2; i < num_clusters - 1; i++) {
                for (int j = 0; j < num_clusters; j++) {
                    dist[i][j] = dist[i + 1][j];
                }
            }
            
            // For the last row
            for (int i = 0; i < num_clusters; i++) {
                dist[num_clusters - 3][i] = dist[num_clusters - 2][i];
            }

        } else {
            for (int i = cluster_1; i < num_clusters; i++) {
                for (int j = 0; j < num_clusters; j++) {
                    dist[i][j] = dist[i + 2][j];
                }
            }
        }

        // Changing the dendA array
        alter_dendA(dendA, cluster_1, cluster_2, num_clusters);

        num_clusters -= 2;
    }


    // Joining the last 2 remaining clusters
    dendA[2] = malloc(sizeof(DNode));
    dendA[2]->left = dendA[0];
    dendA[2]->right = dendA[1];
    return dendA[2];
}


// initialise_dendA initialises the starting dendrogram array

void initialise_dendA(Dendrogram *dendA, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        dendA[i] = malloc(sizeof(DNode));
        dendA[i]->left = NULL;
        dendA[i]->right = NULL;
        dendA[i]->vertex = i;
    }
    dendA[num_clusters] = malloc(sizeof(DNode));
    dendA[num_clusters]->left = NULL;
    dendA[num_clusters]->right = NULL;
}


// initialise_dist initialises the starting distance array

void initialise_dist(double (*dist), int num_clusters, Graph g, int row) {
    for (int col = 0; col < num_clusters; col++) {
        if (!GraphIsAdjacent(g, row, col) && !GraphIsAdjacent(g, col, row)) {
            dist[col] = INFINITY;
            dist[row] = INFINITY;
        }
        else if (GraphIsAdjacent(g, row, col) && GraphIsAdjacent(g, col, row)) {
            // If both are true, we need to compare to see the smaller value
            if (find_edge_weight(g, row, col) > find_edge_weight(g, col, row)) {
                dist[col] = (double) 1 / find_edge_weight(g, row, col);
            } else {
                dist[col] = (double) 1 / find_edge_weight(g, col, row);
            }
        } else if (GraphIsAdjacent(g, row, col)) {
            dist[col] = (double) 1 / find_edge_weight(g, row, col);
        } else {
            dist[col] = (double) 1 / find_edge_weight(g, col, row);
        }
    }
}


// find_edge_weight finds the weight of an edge between 2 particular vertices

int find_edge_weight(Graph g, int src, int dest) {
    AdjList curr_adjacent = GraphOutIncident(g, src);
    int weight = 0;
    while (curr_adjacent != NULL) {
        if (curr_adjacent->v == dest) {
            weight = curr_adjacent->weight;
            break;
        } else {
            curr_adjacent = curr_adjacent->next;
        }
    }
    return weight;
}


// add_cluster_single adds a new row and column for the new cluster (single link method)

void add_cluster_single(double (*dist), int num_clusters, int cluster_1, int cluster_2) {
    for (int i = 0; i < num_clusters; i++) {
        if (dist[cluster_1] > dist[cluster_2]) {
            dist[num_clusters] = dist[cluster_2];
        } else {
            dist[num_clusters] = dist[cluster_1];
        }
    }
}


// add_cluster_complete adds a new row and column for the new cluster (complete link method)

void add_cluster_complete(double (*dist), int num_clusters, int cluster_1, int cluster_2) {
    for (int i = 0; i < num_clusters; i++) {
        if (dist[cluster_1] > dist[cluster_2]) {
            dist[num_clusters] = dist[cluster_1];
        } else {
            dist[num_clusters] = dist[cluster_2];
        }
    }
}


// remove_cluster removes the cols of merged clusters
void remove_cluster_col(double *(dist), int cluster_1, int cluster_2, int num_clusters) {
    if (cluster_2 != cluster_1 + 1) {
        dist[cluster_1] = dist[cluster_1 + 1];
        dist[cluster_2] = dist[cluster_2 + 1];
        for (int i = cluster_1; i < num_clusters - 1; i++) {
            dist[i] = dist[i + 1];
        }
        for (int i = cluster_2; i < num_clusters - 1; i++) {
            dist[i] = dist[i + 1];
        }
    } else {
        for (int i = cluster_1; i < num_clusters - 1; i++) {
            dist[i] = dist[i + 2];
        }
    }
}


// alter_dendA changes the dendA array corresponding to merged clusters

void alter_dendA(Dendrogram *dendA, int cluster_1, int cluster_2, int num_clusters) {
    // Adding new dendrogram into dendA array
    create_dendrogram(dendA, num_clusters, cluster_1, cluster_2);

    // Shifting arrays back so that it removes the two merged clusters
    // The shifting method has 2 methods which occur when either the merged clusters are adjacent in array or not
    if (cluster_2 != cluster_1 + 1) {
        dendA[cluster_1] = dendA[cluster_1 + 1];
        dendA[cluster_2] = dendA[cluster_2 + 1];
        for (int i = cluster_1; i < num_clusters - 1; i++) {
            dendA[i] = dendA[i + 1];
        }
        for (int i = cluster_2; i < num_clusters - 1; i++) {
            dendA[i] = dendA[i + 1];
        }

    } else {
        for (int i = cluster_1; i < num_clusters; i++) {
            dendA[i] = dendA[i + 2];
        }
    }
}


// create_dendrogram creates a new dendrogram that represents merged clusters

void create_dendrogram(Dendrogram *dendA, int num_clusters, int cluster_1, int cluster_2) {
    // If it only runs through the algo once (faster)
    if (dendA[num_clusters - 1] != dendA[cluster_2]) {
        dendA[num_clusters - 1]->right = dendA[cluster_2];
        dendA[num_clusters - 1]->left = dendA[cluster_1];
    } else {
        // If there needs to be a new node created (you are merging something that is already merged)
        Dendrogram d_node = malloc(sizeof(DNode));
        d_node->left = dendA[cluster_1];
        d_node->right = dendA[cluster_2];
        dendA[num_clusters - 1] = d_node;
    }
}
