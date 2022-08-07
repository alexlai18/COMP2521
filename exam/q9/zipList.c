
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
List merge_list(Node l1_curr, Node l2_curr, List new_list, int x, int y);
Node create_node(Node l1_curr);

// Worst case time complexity of this solution: O(n^2), given n is the max number of nodes in l1 or l2
List zipList(List l1, int x, List l2, int y) {
    // Initialising curr pointers as first
    Node l1_curr = l1->first;
    Node l2_curr = l2->first;

    List new_list = ListNew();
    new_list = merge_list(l1_curr, l2_curr, new_list, x, y);
    return new_list;
}

List merge_list(Node l1_curr, Node l2_curr, List new_list, int x, int y) {
    Node new_curr = new_list->first;

    // Merging list while both list nodes aren't NULL
    while (l1_curr != NULL && l2_curr != NULL) {
        // For l1, add x numbers into list
        for (int i = 0; (i < x) && (l1_curr != NULL); i++) {
            if (new_list->first == NULL) {
                new_curr = create_node(l1_curr);
                new_list->first = new_curr;
            } else {
                new_curr->next = create_node(l1_curr);
                new_curr = new_curr->next;
            }
            l1_curr = l1_curr->next;
        }

        // For l2, add y numbers into list
        for (int i = 0; (i < y) && (l2_curr != NULL); i++) {
            if (new_list->first == NULL) {
                new_curr = create_node(l2_curr);
                new_list->first = new_curr;
            } else {
                new_curr->next = create_node(l2_curr);
                new_curr = new_curr->next;
            }
            l2_curr = l2_curr->next;
        }
    }

    // When 1 list is fully added, but the other one isn't
    if (l1_curr == NULL && y > 0) {
        while (l2_curr->next != NULL) {
            new_curr->next = create_node(l2_curr);
            new_curr = new_curr->next;
            l2_curr = l2_curr->next;
        }
        new_curr->next = create_node(l2_curr);
        new_list->last = new_curr->next;
    } else if (x > 0 && l1_curr != NULL) {
        while (l1_curr->next != NULL) {
            new_curr->next = create_node(l1_curr);
            new_curr = new_curr->next;
            l1_curr = l1_curr->next;
        }
        new_curr->next = create_node(l1_curr);
        new_list->last = new_curr->next;
    }

    return new_list;
}


Node create_node(Node l1_curr) {
    Node new_node = malloc(sizeof(Node));
    new_node->value = l1_curr->value;
    new_node->next = NULL;
    return new_node;
}