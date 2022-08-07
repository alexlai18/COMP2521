
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
	Queue q = QueueNew();

	// TODO
	// Write a benchmark test to demonstrate the difference between
	// ArrayQueue and CircularArrayQueue
	
	// From queue_answers.txt, dequeue is O(n) for ArrayQueue and O(1) for CircularArrayQueue. 
	// Therefore, the main difference is in the dequeue operation

	// Enqueue for 1 to 20000
	for (int i = 0; i < 20000; i++) {
		QueueEnqueue(q, i);
	}

	// Dequeue from 1 to 20000
	for (int i = 0; i < 20000; i++) {
		QueueDequeue(q);
	}

	QueueFree(q);
}

