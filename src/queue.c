#include "queue.h"
#include "task.h"

#include <stdbool.h>

void queue_init(struct queue *queue) {
	*queue = (struct queue){0};
	pthread_mutex_init(&queue->mutex, NULL);
	pthread_cond_init(&queue->not_empty, NULL);
	pthread_cond_init(&queue->not_full, NULL);
	queue->queue_closed = false;
}

void queue_destroy(struct queue *queue) {
	pthread_mutex_destroy(&queue->mutex);
	pthread_cond_destroy(&queue->not_empty);
	pthread_cond_destroy(&queue->not_full);
}

bool queue_enqueue(struct queue *queue, struct task task) {
	pthread_mutex_lock(&queue->mutex);
	if(queue->queue_closed) {
		pthread_mutex_unlock(&queue->mutex);
		return false;
	}
	pthread_mutex_unlock(&queue->mutex);

	pthread_mutex_lock(&queue->mutex);
	while(queue_is_full(queue)) {
		pthread_cond_wait(&queue->not_full, &queue->mutex);
	}
	// Adding a task to queue
	queue->tasks[queue->rear] = task;
	queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
	queue->current_size++;

	pthread_cond_signal(&queue->not_empty);
	pthread_mutex_unlock(&queue->mutex);
	return true;
}

bool queue_dequeue(struct queue *queue, struct task *task) { 
	pthread_mutex_lock(&queue->mutex); 
	while(queue_is_empty(queue) && !queue->queue_closed) { 
		pthread_cond_wait(&queue->not_empty, &queue->mutex);
	}

	if(queue->current_size == 0 && queue->queue_closed) {
		pthread_mutex_unlock(&queue->mutex);
		return false;
	}
	// Removing a task from queue
	*task = queue->tasks[queue->front];
	queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
	queue->current_size--;
	
	pthread_cond_signal(&queue->not_full);
	pthread_mutex_unlock(&queue->mutex);
	return true;
}

bool queue_is_empty(struct queue *queue) {
	return queue->current_size == 0;
}

bool queue_is_full(struct queue *queue) {
	return queue->current_size == MAX_QUEUE_SIZE;
}
