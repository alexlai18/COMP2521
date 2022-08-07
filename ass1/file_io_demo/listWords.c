// Program to demonstrate the main file IO functions

// Reads a file called sample_input.txt and writes a numbered
// list of the words in that file to sample_output.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

int main(int argc, char *argv[]) {
	// files can be opened with fopen
	// fopen takes in a filename and mode
	// the main modes are "r" for reading and "w" for writing
	FILE *in = fopen("sample_input.txt", "r");

	// fopen can return NULL for a number of reasons, for example:
	// - the mode is "r" but the file doesn't exist
	// - the user does not have permission to open the file in that mode
	// - the program has reached the open file limit
	// so you should always check the return value of fopen
	if (in == NULL) {
		fprintf(stderr, "error: failed to open sample_input.txt\n");
		exit(EXIT_FAILURE);
	}

	FILE *out = fopen("sample_output.txt", "w");
	if (out == NULL) {
		fprintf(stderr, "error: failed to open sample_output.txt\n");
		exit(EXIT_FAILURE);
	}

	// words are assumed to be at most 100 characters long
	char word[MAX_WORD + 1];
	int count = 0;
	while (fscanf(in, "%s", word) == 1) {
		count++;
		// fprintf is like printf but prints to a specified file
		fprintf(out, "%d. %s\n", count, word);
	}

	// any files that you open must be closed using fclose
	// leaving files open will result in memory leaks
	fclose(out);
	fclose(in);
}

