/*
 * circular_queue.c
 *
 *  Created on: 14 de abr de 2022
 *      Author: munin
 */

// C program for array implementation of queue
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <circular_queue.h>

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
queue* createQueue(unsigned capacity) {
	queue aux[1];
	queue *Queue = aux;
	Queue->capacity = capacity;
	Queue->front = Queue->size = 0;

	// This is important, see the enQueue
	Queue->rear = capacity - 1;
	uint8_t pl[Queue->capacity];
	Queue->array = pl;
	return Queue;
}

// Queue is full when size becomes
// equal to the capacity
uint8_t isFull(queue *Queue) {
	return (Queue->size == Queue->capacity);
}

// Queue is empty when size is 0
uint8_t isEmpty(queue *Queue) {
	return (Queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enQueue(queue *Queue, uint8_t item) {
	if (isFull(Queue))
		return;
	Queue->rear = (Queue->rear + 1) % Queue->capacity;
	Queue->array[Queue->rear] = item;
	Queue->size = Queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
uint8_t deQueue(queue *Queue) {
	uint8_t item;
	if (isEmpty(Queue))
		return false;
	item = Queue->array[Queue->front];
	Queue->front = (Queue->front + 1) % Queue->capacity;
	Queue->size = Queue->size - 1;
	return item;
}

// Function to get front of queue
uint8_t front(queue *Queue) {
	if (isEmpty(Queue))
		return false;
	return Queue->array[Queue->front];
}

// Function to get rear of queue
uint8_t rear(queue *Queue) {
	if (isEmpty(Queue))
		return false;
	return Queue->array[Queue->rear];
}

/*
	 Driver program to test above functions.
	 uint8_t main()
	 {
	 queue* Queue = createQueue(1000);

	 enQueue(Queue, 10);
	 enQueue(Queue, 20);
	 enQueue(Queue, 30);
	 enQueue(Queue, 40);

	 printf("%d deQueued from queue\n\n",
	 deQueue(Queue));

	 printf("Front item is %d\n", front(Queue));
	 printf("Rear item is %d\n", rear(Queue));

	 return 0;
	 }
 */
