/*
 * circular_queue.h
 *
 *  Created on: 14 de abr de 2022
 *      Author: munin
 */

#ifndef INC_CIRCULAR_QUEUE_H_
#define INC_CIRCULAR_QUEUE_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// A structure to represent a queue
typedef struct  {
	uint8_t front, rear, size;
	unsigned capacity;
	uint8_t* array;
}queue;

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
queue* createQueue(unsigned capacity);
// Queue is full when size becomes
// equal to the capacity
uint8_t isFull(queue* queue);
// Queue is empty when size is 0
uint8_t isEmpty(queue* queue);

// Function to add an item to the queue.
// It changes rear and size
void enQueue(queue* queue, uint8_t item);

// Function to remove an item from queue.
// It changes front and size
uint8_t deQueue(queue* queue);
// Function to get front of queue
uint8_t front(queue* queue);

// Function to get rear of queue
uint8_t rear(queue* queue);

#endif /* INC_CIRCULAR_QUEUE_H_ */
