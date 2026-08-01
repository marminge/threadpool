#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "queue.h"
#include <pthread.h>

#define MAX_QUEUE_SIZE 64

struct thread_pool {
	pthread_t *workers;
	struct queue queue;
	size_t worker_count;
};

bool thread_pool_init(struct thread_pool *thread_pool, size_t workers);

bool thread_pool_submit(struct thread_pool *thread_pool, void (*function)(void *), void *arg);

void thread_pool_shutdown(struct thread_pool *thread_pool);

void thread_pool_destroy(struct thread_pool *thread_pool);

#endif
