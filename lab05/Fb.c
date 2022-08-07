// Implementation of the FriendBook ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "List.h"
#include "Map.h"
#include "Queue.h"

#define DEFAULT_CAPACITY 1 // !!! DO NOT CHANGE THIS !!!

struct fb {
    int    numPeople;
    int    capacity;

    char **names;    // the id of a person is simply the index
                     // that contains their name in this array
    
    Map    nameToId; // maps names to ids

    bool **friends;
};

static void  increaseCapacity(Fb fb);
static char *myStrdup(char *s);
static int   nameToId(Fb fb, char *name);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb   FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->capacity = DEFAULT_CAPACITY;
    
    fb->names = calloc(fb->capacity, sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    fb->nameToId = MapNew();

    fb->friends = malloc(fb->capacity * sizeof(bool *));
    if (fb->friends == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < fb->capacity; i++) {
        fb->friends[i] = calloc(fb->capacity, sizeof(bool));
        if (fb->friends[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->capacity; i++) {
        free(fb->friends[i]);
    }
    free(fb->friends);

    MapFree(fb->nameToId);

    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }
    free(fb->names);
    
    free(fb);
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == fb->capacity) {
        increaseCapacity(fb);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = myStrdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

static void increaseCapacity(Fb fb) {
    int newCapacity = fb->capacity * 2;
    
    fb->names = realloc(fb->names, newCapacity * sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->names[i] = NULL;
    }
    
    fb->friends = realloc(fb->friends, newCapacity * sizeof(bool *));
    if (fb->friends == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < fb->capacity; i++) {
        fb->friends[i] = realloc(fb->friends[i], newCapacity * sizeof(bool));
        if (fb->friends[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
        for (int j = fb->capacity; j < newCapacity; j++) {
            fb->friends[i][j] = false;
        }
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->friends[i] = calloc(newCapacity, sizeof(bool));
        if (fb->friends[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    
    fb->capacity = newCapacity;
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (!fb->friends[id1][id2]) {
        fb->friends[id1][id2] = true;
        fb->friends[id2][id1] = true;
        return true;
    } else {
        return false;
    }
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return fb->friends[id1][id2];
}

int  FbNumFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    int numFriends = 0;
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            numFriends++;
        }
    }
    return numFriends;
}

////////////////////////////////////////////////////////////////////////
// Your tasks

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    // TODO: Complete this function

    // First we need to check if the names have an existing FB account
    assert (FbHasPerson(fb, name1) == true);
    assert (FbHasPerson(fb, name2) == true);

    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);

    if (FbIsFriend(fb, name1, name2) == true){
        assert(id1 != id2);
        fb->friends[id1][id2] = false;
        fb->friends[id2][id1] = false;
        return true;
    }
    return false;
}

List FbMutualFriends(Fb fb, char *name1, char *name2) {
    // TODO: Complete this function
    List l = ListNew();

    // First we need to check if the names have an existing FB account
    assert (FbHasPerson(fb, name1) == true);
    assert (FbHasPerson(fb, name2) == true);

    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);


    int count = fb->capacity;
    
    for (int i = 0; i < count; i++) {
        if (fb->friends[id2][i] == true && fb->friends[id1][i] == true) {
            char *mutual = fb->names[i];
            ListAppend(l, mutual);
        }
    }

    return l;
}

void FbFriendRecs1(Fb fb, char *name) {
    // TODO: Complete this function
    assert (FbHasPerson(fb, name) == true);
    int count = fb->capacity;
    
    char *mutuals[count];
    int mutual_count[count];

    int list_counter = 0;

    int id = nameToId(fb, name);

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (fb->friends[id][j] == true && fb->friends[j][i]) {
                mutuals[list_counter] = fb->names[i];
                mutual_count[list_counter]++;
            }
        }
        list_counter++;
    }

	// Ordering mutuals in descending order
   for (int i = 0; i < list_counter; i++) {
		for (int j = i + 1; j < list_counter; j++) {
			if (mutual_count[i] < mutual_count[j]) {
				// Swap positions 
				int temp = mutual_count[i];
				mutual_count[i] = mutual_count[j];
				mutual_count[j] = temp;

                char *friend = mutuals[i];
                mutuals[i] = mutuals[j];
                mutuals[j] = friend;
         	}
		}
	}
    printf("%s's friend reccomendations\n", name);
    for (int i = 0; i < list_counter; i++) {
        printf("\t%-20s%4d mutual friends\n", mutuals[i], mutual_count[i]);
    }
}

////////////////////////////////////////////////////////////////////////
// Optional task

void FbFriendRecs2(Fb fb, char *name) {
    // TODO: Complete this function
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}

