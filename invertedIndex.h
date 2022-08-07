// COMP2521 Assignment 1

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

struct FileNode {
	char *filename;
	double tf;
	struct FileNode *next;
};

typedef struct FileNode *FileList;

struct InvertedIndexNode {
	char *word;
	struct FileNode *fileList;
	struct InvertedIndexNode *left;
	struct InvertedIndexNode *right;
};

typedef struct InvertedIndexNode *InvertedIndexBST;

struct TfIdfNode {
	char *filename;
	double tfIdfSum; // if only one search term, this is just the tf-idf
	struct TfIdfNode *next;
};

typedef struct TfIdfNode *TfIdfList;

// Part 1

/**
 * This function opens the collection file with the given name, and then
 * generates  an  inverted index from the files listed in the collection
 * file, as described in the spec. It  returns  the  generated  inverted
 * index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename);

/**
 * Outputs an inverted index to a file with the given name, as described
 * in the spec.
 */
void printInvertedIndex(InvertedIndexBST tree, char *filename);

/**
 * Frees all memory associated with the given inverted index.
 */
void freeInvertedIndex(InvertedIndexBST tree);

// Part 2

/**
 * Returns  an  ordered list where each node contains a filename and the
 * corresponding tf-idf value for a given searchWord. You only  need  to
 * include documents (files) that contain the given searchWord. The list
 * must  be  in  descending order of tf-idf value. If there are multiple
 * files with same  tf-idf  value,  order  them  by  their  filename  in
 * ascending order.
*/
TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D);

/**
 * Returns  an  ordered list where each node contains a filename and the
 * summation of tf-idf values of all the matching search words for  that
 * file.  You only need to include documents (files) that contain one or
 * more of the given search words. The list must be in descending  order
 * of summation of tf-idf values (tfIdfSum). If there are multiple files
 * with  the  same tf-idf sum, order them by their filename in ascending
 * order.
 */
TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D);

/**
 * Frees all memory associated with the given tf-idf list.
 */
void freeTfIdfList(TfIdfList list);

#endif

