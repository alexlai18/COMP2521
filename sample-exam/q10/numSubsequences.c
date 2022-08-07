
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
int find_same_sequence(Node curr_a, int tolerance, List listB);


// Worst case time complexity of this solution: O(n * m)
int numSubsequences(List listA, List listB, int tolerance) {
	// Given listA and listB, find the amount of occurences of B in A
	Node curr_a = listA->first;
	int count = 0;
	while (curr_a != NULL) {
		Node temp = curr_a;
		count += find_same_sequence(temp, tolerance, listB);
		curr_a = curr_a->next;
	}
	return count;
}

int find_same_sequence(Node curr_a, int tolerance, List listB) {
	Node curr_b = listB->first;
	int not_same = 0;
	while (curr_b != NULL && curr_a != NULL) {
		if (curr_b->value != curr_a->value) {
			not_same++;
		}
		curr_b = curr_b->next;
		curr_a = curr_a->next;
	}

	// Making sure tolerance is larger than actual difference and that it actually checks the whole of list b
	if (not_same <= tolerance && curr_b == NULL) {
		return 1;
	} else {
		return 0;
	}
}
