// COMP2521 Assignment 1
// Written by Alexander Lai (z5363776) on 20-03-2022
// This program reads in a collection of files with words within them, and creates an inverted index with the information
// This program can then search for these words, and give us the results based on their relevance

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"

#define MAX_COLLECTION_SIZE 200

// generateInvertedIndex functions
void normalise_word(char *word);
int open_collection(char *collectionFilename, char name[MAX_COLLECTION_SIZE], char *file_array[MAX_COLLECTION_SIZE]);
InvertedIndexBST create_new_node(char *word);
int check_and_store_words(char *words[MAX_COLLECTION_SIZE], int counter[MAX_COLLECTION_SIZE], int unique_words, char *word, int word_count);
InvertedIndexBST insert_in_tree(InvertedIndexBST curr, InvertedIndexBST new_node);
void input_data_node(InvertedIndexBST curr, int counter, int word_count, char *filename);

// printInvertedIndex functions
InvertedIndexBST BST_search(InvertedIndexBST tree, char *word);
void print_tree_traversal(InvertedIndexBST tree, FILE *stream);

// freeInvertedIndex functions
void free_file_node(InvertedIndexBST tree);
void free_filename_v1(FileList file_list);
void free_word_and_filelist(InvertedIndexBST tree);


// searchOne functions
InvertedIndexBST word_tree_search(InvertedIndexBST tree, char *searchWord);
int store_file_information(InvertedIndexBST branch, char *file_contains[MAX_COLLECTION_SIZE], double tf_list[MAX_COLLECTION_SIZE]);
void tf_to_tfidf(double tf_list[MAX_COLLECTION_SIZE], int num_docs, int D, double tfidf_list[MAX_COLLECTION_SIZE]);
void order_by_tfidf(double tfidf_list[MAX_COLLECTION_SIZE], char *file_contains[MAX_COLLECTION_SIZE], int num_docs);
TfIdfList create_new_tf_node(char *filename, double tfidf);
TfIdfList list_append (double tfidf_list[MAX_COLLECTION_SIZE], char *file_contains[MAX_COLLECTION_SIZE], int num_docs);


// searchMany functions
// searchMany uses all functions for searchOne
// functions under this heading are exclusive/unique to searchMany
int insert_in_global(char *all_file_contains[MAX_COLLECTION_SIZE], double all_tfidf_list[MAX_COLLECTION_SIZE], 
					char *file_contains[MAX_COLLECTION_SIZE], double tfidf_list[MAX_COLLECTION_SIZE], int num_docs, int global_num_docs);

int file_in_global(char *all_file_contains[MAX_COLLECTION_SIZE], double all_tfidf_list[MAX_COLLECTION_SIZE], 
					char *file_contains[MAX_COLLECTION_SIZE], double tfidf_list[MAX_COLLECTION_SIZE], int index, int global_num_docs);


// freeTfIdfList functions
void free_tfidf_node(TfIdfList list);


// Part 1

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
	char name[MAX_COLLECTION_SIZE];
	char *file_array[MAX_COLLECTION_SIZE];

	// Initialising blank tree
	InvertedIndexBST tree = NULL;

	// Opening the collection of files and storing them into an array
	int num_files = open_collection(collectionFilename, name, file_array);

	// Initialising the tree in which the words will be put in
	// Creates an array with words sorted in alphabetical order
	int file_counter = 0;
	for (int i = 0; i < num_files; i++) {
		int counter[MAX_COLLECTION_SIZE] = {};
		char word[MAX_COLLECTION_SIZE] = {0};
		int word_count = 0;
		int unique_words = 0;
		char *words[MAX_COLLECTION_SIZE] = {};

		// Opening the files and reading the words.
		// The functions normalise the words read, and store them into an array of strings containing unique words
		FILE *file = fopen(file_array[file_counter], "r");
		while (fscanf(file, "%s", word) != EOF) {
			word_count++;
			normalise_word(word);
			if (word[0] != '\0') {
				unique_words = check_and_store_words(words, counter, unique_words, word, word_count);
			} else {
				word_count--;
			}
		}

		for (int i = 0; i < unique_words; i++) {
			InvertedIndexBST new_node = create_new_node(words[i]);
			// Insert the node in order (make a separate function)
			// After doing so add filename to linked list
			tree = insert_in_tree(tree, new_node);

			// BINARY SEARCH FOR WORD
			InvertedIndexBST branch = BST_search(tree, words[i]);

			input_data_node(branch, counter[i], word_count, file_array[file_counter]);
		}


		// MEMORY AND FILE MAINTENANCE: FREEING USED FILES AND CLOSING USED FILES (NOT FOUND IN TREE)

		// Freeing the file after being used
		free(file_array[file_counter]);

		// Freeing the array of unique words after being used
		for (int i = 0; i < unique_words; i++) {
			free(words[i]);
		}

		file_counter++;
		fclose(file);
	}
	return tree;
}

void printInvertedIndex(InvertedIndexBST tree, char *filename) {
	FILE *stream = fopen(filename, "w");
	print_tree_traversal(tree, stream);
	fclose(stream);
}

void freeInvertedIndex(InvertedIndexBST tree) {
	free_file_node(tree);
}







// Part 2

TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
	char *file_contains[MAX_COLLECTION_SIZE];
	double tf_list[MAX_COLLECTION_SIZE];
	double tfidf_list[MAX_COLLECTION_SIZE];


	// Find the branch of the tree with the desired word
	InvertedIndexBST branch = word_tree_search(tree, searchWord);

	// Finding the information in the tree and storing into arrays
	int num_docs = store_file_information(branch, file_contains, tf_list);
	tf_to_tfidf(tf_list, num_docs, D, tfidf_list);
	order_by_tfidf(tfidf_list, file_contains, num_docs);
	
	// Inputting info into the TfIdfList struct, and forming a list
	TfIdfList List = list_append(tfidf_list, file_contains, num_docs);
	return List;
}


TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
	char *all_file_contains[MAX_COLLECTION_SIZE];
	double all_tfidf_list[MAX_COLLECTION_SIZE];
	int global_num_docs = 0;

	// Looking at the words one by one, and storing their information
	for (int i = 0; searchWords[i] != NULL; i++) {
		double tf_list[MAX_COLLECTION_SIZE];
		char *file_contains[MAX_COLLECTION_SIZE] = {NULL};
		double tfidf_list[MAX_COLLECTION_SIZE];


		// Finding the branch of the tree with the desire word
		InvertedIndexBST branch = word_tree_search(tree, searchWords[i]);

		// Finding the information in the tree and storing into the array
		// Utilising functions referenced in searchOne function
		int num_docs = store_file_information(branch, file_contains, tf_list);
		tf_to_tfidf(tf_list, num_docs, D, tfidf_list);

		// Inserting the filenames and the tfidf into the global array
		global_num_docs = insert_in_global(all_file_contains, all_tfidf_list, file_contains, tfidf_list, num_docs, global_num_docs);
	}

	// Now, we have to order the global list in terms of tfidf order first, then alphabetical order
	order_by_tfidf(all_tfidf_list, all_file_contains, global_num_docs);


	// Inputting info into the TfIdfList struct, and forming a list (copied from searchOne)
	TfIdfList List = list_append(all_tfidf_list, all_file_contains, global_num_docs);

	return List;
}

void freeTfIdfList(TfIdfList list) {
	free_tfidf_node(list);
}


////////////////////////////////////////////////////////
//====================================================//
//================ HELPER FUNCTIONS	==================//
//====================================================//
////////////////////////////////////////////////////////




////////////////////////////////////////////////////////
//====================================================//
//======== generateInvertedIndex FUNCTIONS ===========//
//====================================================//
////////////////////////////////////////////////////////


// normalise_word function essentially changes words so that they are normalised (all lowercase and some punctuation rules)

void normalise_word(char *word) {
	// To normalise the search word, they must all be lowercase and have no punctuation

	// Changing to all lowercase
	for (int j = 0; word[j] != '\0'; j++) {
		if(word[j] >= 'A' && word[j] <= 'Z') {
			word[j] = word[j] + 32;
		}
	}

	// Removing the punctuation of the word
	int length = strlen(word);
	char punct_array[6] = {'.', ',', ':', ';', '?', '*'};
	for (int len_count = length; len_count > 0; len_count--) {
		int normalised = 0;
		for (int i = 0; i < 6; i++) {
			if (word[len_count - 1] == punct_array[i]) {
				word[len_count - 1] = '\0';
				normalised = 1;
				break;
			}
		}
		// Making sure we only check the last character (can only normalise again if it had already been normalised)
		if (normalised == 0) {
			return;
		}
	}
}


// open_collection function reads the collectionFilename file and sorts into an array alphabetically

int open_collection(char *collectionFilename, char name[MAX_COLLECTION_SIZE], char *file_array[MAX_COLLECTION_SIZE]) {
	// Accessing data in char *collectionFilename
	FILE* filename = fopen(collectionFilename, "r");

	// Puts all the files inside collectionFilename into one array
	int num_files = 0;
	while (fscanf(filename, "%s", name) != EOF) {
		file_array[num_files] = malloc(strlen(name) + 1);
    	strcpy(file_array[num_files], name);
		num_files++;
	}

	// Ordering the files in alphabetical order
   for (int i = 0; i < num_files; i++) {
		for (int j = i + 1; j < num_files; j++) {
			if (strcmp(file_array[i], file_array[j]) > 0) {
				// Swap num_filess of the files
				char *temp = file_array[i];
				file_array[i] = file_array[j];
				file_array[j] = temp;
         	} 
		}
	}
	fclose(filename);
	return num_files;
}


/* check_and_store_words function checks if the word is unique in the file and stores it in an array.
 It also keeps count of how many times the word occurs in the file */

int check_and_store_words(char *words[MAX_COLLECTION_SIZE], int counter[MAX_COLLECTION_SIZE], int unique_words, char *word, int word_count) {
	// Checking if the word is new in the file
	int new_word = 1;
	int i;

	for (i = 0; i < unique_words; i++) {
		if (strcmp(words[i], word) == 0) {
			new_word = 0;
			break;
		}
	}

	// Storing the word into array and adding to counter
	if (new_word == 1) {
		int str_len = strlen(word);
		words[unique_words] = malloc((str_len + 1) * sizeof(char)); 
		strncpy(words[unique_words], word, str_len + 1);
		counter[unique_words]++;
		unique_words++;
	} else {
		counter[i]++;
	}
	return unique_words;
}


// create_new_node function creates a new node for the tree and inputs the word into the node

InvertedIndexBST create_new_node(char *word) {
	InvertedIndexBST new_node = malloc(sizeof(struct InvertedIndexNode));

	// Allocation memory for the new word
	new_node->word = malloc(strlen(word) + 1);

	// Copying the word and pasting it into the struct
	strcpy(new_node->word, word);

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->fileList = NULL;
	return new_node;
}


// insert_in_tree function inserts the node in the tree in alphabetical order 

InvertedIndexBST insert_in_tree(InvertedIndexBST curr, InvertedIndexBST new_node) {
	// Ignoring duplicates and inserts the node in order
	if (curr == NULL) {
		// If leaf is empty, then we insert the new_node in it
		return new_node;
	}

	// Traversing through the tree to place it in an appropriate place (determined by strcmp)
	// THIS IS TAKEN FROM LECTURE SLIDES
	int cmp = strcmp(curr->word, new_node->word);
	if (cmp > 0) {
		curr->left = insert_in_tree(curr->left, new_node);
	} else if (cmp < 0) {
		curr->right = insert_in_tree(curr->right, new_node);
	} else {
		// Avoiding duplicates
		// Freeing the memory for the new_node since it doesn't belong in the tree
		free_file_node(new_node);
	}
	return curr;
}


// input_data_node function accesses the current node and inputs data into it

void input_data_node(InvertedIndexBST curr, int counter, int word_count, char *filename) {
	// Initialising a new file node
	FileList new_file = malloc(sizeof(struct FileNode));
	new_file->next = NULL;
	new_file->filename = strdup(filename);
	double tf = (double) counter / word_count;
	new_file->tf = tf;

	// If the list is empty
	FileList curr_list = curr->fileList;
	if (curr_list == NULL) {
		curr->fileList = new_file;
	} else {
		while (curr_list->next != NULL) {
			curr_list = curr_list->next;
		}
		curr_list->next = new_file;
	}
}


// BST_search function searches the tree for the word, and returns that branch

InvertedIndexBST BST_search(InvertedIndexBST tree, char *word) {
	int cmp = strcmp(word, tree->word);

	if (tree != NULL) {
		if (cmp < 0) {
			return BST_search(tree->left, word);
		} else if (cmp > 0) {
			return BST_search(tree->right, word);
		}
		else {
			return tree;
		}
	}
	return NULL;
}



////////////////////////////////////////////////////////
//====================================================//
//========== printInvertedIndex FUNCTIONS ============//
//====================================================//
////////////////////////////////////////////////////////


// print_tree_traveral function traverses the tree and prints out each word and their tf

void print_tree_traversal(InvertedIndexBST tree, FILE *stream) {
	if (tree != NULL) {
		print_tree_traversal(tree->left, stream);
		

		// After finding the wanted branch, we write the word found in the files
		FileList file_list = tree->fileList;
		fprintf(stream, "%s", tree->word);

		// Then we write the filenames and the corresponding tf into the file
		while (file_list != NULL) {
			fprintf(stream, " %s (%.7lf)", file_list->filename, file_list->tf);
			file_list = file_list->next;
		}
		fprintf(stream, "\n");


		print_tree_traversal(tree->right, stream);
   	}
}



////////////////////////////////////////////////////////
//====================================================//
//=========== freeInvertedIndex FUNCTIONS ============//
//====================================================//
////////////////////////////////////////////////////////


// free_file_node function uses post-order traversal and frees the filenames of a filelist, and then the inverted index node

void free_file_node(InvertedIndexBST tree) {
	if (tree == NULL) {
		return;
	}


	// First traverse both subtrees in post-order
	free_file_node(tree->left);
	free_file_node(tree->right);


	// Then delete the node
	free_word_and_filelist(tree);
	free(tree);
	tree = NULL;
}


// free_filename_v1 function frees the filenames inside a file list

void free_filename_v1(FileList file_list) {
	while (file_list != NULL) {
       free(file_list->filename);
	   FileList tmp = file_list;
       file_list = file_list->next;
	   free(tmp);
    }
}


// free_word_and_filelist free the word and filelist of a branch in the tree

void free_word_and_filelist(InvertedIndexBST tree) {
	free_filename_v1(tree->fileList);
	// Already freed all contents of fileList
	free(tree->word);
}



////////////////////////////////////////////////////////
//====================================================//
//================ searchOne FUNCTIONS ===============//
//====================================================//
////////////////////////////////////////////////////////


// word_tree_search function finds the searchWord we are looking for and returns the branch

InvertedIndexBST word_tree_search(InvertedIndexBST tree, char *searchWord) {
	int cmp = strcmp(searchWord, tree->word);
	
	if (tree != NULL) {
		if (cmp < 0) {
			return word_tree_search(tree->left, searchWord);
		} else if (cmp > 0) {
			return word_tree_search(tree->right, searchWord);
		}
		else {
			return tree;
		}
	}
	return NULL;
}


// store_file_information function looks into a branch and stores its filename and tf into separate arrays

int store_file_information(InvertedIndexBST branch, char *file_contains[MAX_COLLECTION_SIZE], double tf_list[MAX_COLLECTION_SIZE]) {
	// Storing the filenames and tf in an array so it can be manipulated
	FileList curr = branch->fileList;
	int index = 0;
	while (curr != NULL) {
		file_contains[index] = curr->filename;
		tf_list[index] = curr->tf;
		curr = curr->next;
		index++;
	}
	return index;
}


// tf_to_tfidf function converts the tf to the tfidf

void tf_to_tfidf(double tf_list[MAX_COLLECTION_SIZE], int num_docs, int D, double tfidf_list[MAX_COLLECTION_SIZE]) {
	// idf = log10(num docs / num docs that has word in it)
	double idf = log10((double) D / num_docs);
	
	// tfidf = tf * idf
	for (int i = 0; i < num_docs; i++) {
		tfidf_list[i] = tf_list[i] * idf;
	}
}


/* order_by_tfidf function orders the tfidf in descending order (or alphabetical if equal)
It changes the tfidf_list and file_contains array correspondingly */

void order_by_tfidf(double tfidf_list[MAX_COLLECTION_SIZE], char *file_contains[MAX_COLLECTION_SIZE], int num_docs) {
	for (int index = 0; index < num_docs; index++) {
		for (int index_2 = index + 1; index_2 < num_docs; index_2++) {
			// To order the array in descending order
			if (tfidf_list[index] < tfidf_list[index_2]) {
				// Swap tfidf
				double temp = tfidf_list[index];
				tfidf_list[index] = tfidf_list[index_2];
				tfidf_list[index_2] = temp;

				// When swapping tfidf, we must do the same for the files
				char *temp2 = file_contains[index];
				file_contains[index] = file_contains[index_2];
				file_contains[index_2] = temp2;
			}

			// If tfidf are equal, order alphabetically in ascending order
			if (tfidf_list[index] == tfidf_list[index_2]) {
				int cmp = strcmp(file_contains[index], file_contains[index_2]);
				if (cmp > 0) {
					char *temp = file_contains[index];
					file_contains[index] = file_contains[index_2];
					file_contains[index_2] = temp;
				}
			}
		} 
	
	}
}


// create_new_tf_node function creates a new node of TfIdfList type

TfIdfList create_new_tf_node(char *filename, double tfidf) {
	TfIdfList new_node = malloc(sizeof(struct TfIdfNode));
	new_node->next = NULL;
	new_node->filename = malloc(strlen(filename) + 1);
	strcpy(new_node->filename, filename);
	new_node->tfIdfSum = tfidf;
	return new_node;
}


// list_append function creates adds a new node to the end of the list of type TfIdfList

TfIdfList list_append(double tfidf_list[MAX_COLLECTION_SIZE], char *file_contains[MAX_COLLECTION_SIZE], int num_docs) {
	// Inputting info into the TfIdfList struct
	TfIdfList List = NULL;
	for (int i = 0; i < num_docs; i++) {
		// As the tfidf_list is already ordered correctly via spec, we just append
		TfIdfList new_node = create_new_tf_node(file_contains[i], tfidf_list[i]);

		// If the List is empty, we just make the new_node the head of the list
		if (List == NULL) {
			List = new_node;
		} else {
			// Otherwise, we iterate through the list and add it to the end
			TfIdfList curr = List;
			while (curr->next != NULL) {
				curr = curr->next;
			}
			curr->next = new_node;
		}
	}
	return List;
}



////////////////////////////////////////////////////////
//====================================================//
//=============== searchMany FUNCTIONS ===============//
//====================================================//
////////////////////////////////////////////////////////

// searchMany uses all functions found in searchOne
// The functions found in this section are exclusive to searchMany


// insert_in_global function inputs new files and tfidfs into the global array

int insert_in_global(char *all_file_contains[MAX_COLLECTION_SIZE], double all_tfidf_list[MAX_COLLECTION_SIZE], 
					char *file_contains[MAX_COLLECTION_SIZE], double tfidf_list[MAX_COLLECTION_SIZE], int num_docs, int global_num_docs) {

	// Iterating through each file for a specific searchWord
	for (int index = 0; index < num_docs; index++) {
		// Iterating through the (all) arrays to see whether it has been discovered already
		global_num_docs = file_in_global(all_file_contains, all_tfidf_list, file_contains, tfidf_list, index, global_num_docs);
	}
	return global_num_docs;
}


// file_in_global function searches the array to see if the given filename is new in the global array

int file_in_global(char *all_file_contains[MAX_COLLECTION_SIZE], double all_tfidf_list[MAX_COLLECTION_SIZE], 
					char *file_contains[MAX_COLLECTION_SIZE], double tfidf_list[MAX_COLLECTION_SIZE], int index, int global_num_docs) {

	int new = 1;
	int j = 0;
	while (j < global_num_docs) {
		int cmp = strcmp(all_file_contains[j], file_contains[index]);

		// If the filename is already in the global filenames
		if (cmp == 0) {
			all_tfidf_list[j] = all_tfidf_list[j] + tfidf_list[index];
			new = 0;
			break;
		}
		j++;
	}

	// If it is new, add it to the array
	if (new == 1) {
		all_file_contains[global_num_docs] = file_contains[index];
		all_tfidf_list[global_num_docs] = tfidf_list[index];
		global_num_docs++;
		return global_num_docs;
	}

	return global_num_docs;
}


////////////////////////////////////////////////////////
//====================================================//
//============== freeTfIdfList FUNCTIONS =============//
//====================================================//
////////////////////////////////////////////////////////


// free_tfidf_node function frees the filename and the node of the struct

void free_tfidf_node(TfIdfList list) {
	// Frees filename first and then sets temp to free the node
	while (list != NULL) {
       free(list->filename);
	   TfIdfList tmp = list;
       list = list->next;
	   free(tmp);
    }
}