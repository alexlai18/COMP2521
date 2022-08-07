
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fs.h"
int find_total_size(FileList curr);

int totalTextSize(Fs fs) {
	// Setting the initial file system
	File curr = fs->root;
	FileList curr_list = curr->files;
	return find_total_size(curr_list);
}

int find_total_size (FileList curr) {
	int total_size = 0;
	// While FileList is not empty (or text file)
	while (curr != NULL) {
		// If the file is a text file
		if (curr->file->type == TEXT_FILE) {
			total_size += strlen(curr->file->text);
		} else if (curr->file->files != NULL) {
			// If the file is a directory and has a list of files inside
			total_size += find_total_size(curr->file->files);
		}
		curr = curr->next;
	}
	return total_size;
}
