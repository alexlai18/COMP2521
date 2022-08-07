
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../invertedIndex.h"

static void test1(void);
static void test2(void);
static void test3(void);
static void test4(void);
static void test5(void);

static void printTfIdfList(TfIdfList list, char *filename);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <test number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	switch (atoi(argv[1])) {
		case 1: test1(); break;
		case 2: test2(); break;
		case 3: test3(); break;
		case 4: test4(); break;
		case 5: test5(); break;
		default:
			fprintf(stderr, "%s: unknown test number '%s'\n",
			        argv[0], argv[1]);
			exit(EXIT_FAILURE);
	}
}

static void test1(void) {
	InvertedIndexBST tree = generateInvertedIndex("collection.txt");
	// expected result is in invertedIndex.exp
	printInvertedIndex(tree, "invertedIndex.out");
	freeInvertedIndex(tree);
}

static void test2(void) {
	InvertedIndexBST tree = generateInvertedIndex("collection.txt");
	TfIdfList list = searchOne(tree, "mars", 7);
	// expected result is in mars.exp
	printTfIdfList(list, "mars.out");
	freeTfIdfList(list);
	freeInvertedIndex(tree);
}

static void test3(void) {
	InvertedIndexBST tree = generateInvertedIndex("collection.txt");
	TfIdfList list = searchOne(tree, "sun", 7);
	// expected result is in sun.exp
	printTfIdfList(list, "sun.out");
	freeTfIdfList(list);
	freeInvertedIndex(tree);
}

static void test4(void) {
	InvertedIndexBST tree = generateInvertedIndex("collection.txt");
	TfIdfList list = searchOne(tree, "moon", 7);
	// expected result is in moon.exp
	printTfIdfList(list, "moon.out");
	freeTfIdfList(list);
	freeInvertedIndex(tree);
}

static void test5(void) {
	InvertedIndexBST tree = generateInvertedIndex("collection.txt");
	char *words[] = { "nasa", "mars", "moon", NULL };
	TfIdfList list = searchMany(tree, words, 7);
	// expected result is in nasa_mars_moon.exp
	printTfIdfList(list, "nasa_mars_moon.out");
	freeTfIdfList(list);
	freeInvertedIndex(tree);
}

static void printTfIdfList(TfIdfList list, char *filename) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "error: failed to open %s\n", filename);
		return;
	}

	TfIdfList curr = list;
	while (curr != NULL) {
		fprintf(fp, "%.7lf %s\n", curr->tfIdfSum, curr->filename);
		curr = curr->next;
	}
	fclose(fp);
}

