#ifndef QUEUE_H_
#define QUEUE_H_

#include "task.h"
#include <pthread.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 64

struct queue {
	struct task tasks[MAX_QUEUE_SIZE];

	int front;
	int rear;
	int current_size;

	pthread_mutex_t mutex;

	pthread_cond_t not_full;
	pthread_cond_t not_empty;

	bool queue_closed;
};

void queue_init(struct queue *queue);

void queue_destroy(struct queue *queue);

bool queue_enqueue(struct queue *queue, struct task task);

bool queue_dequeue(struct queue *queue, struct task *task);

bool queue_is_empty(struct queue *queue);

bool queue_is_full(struct queue *queue);

#endif
