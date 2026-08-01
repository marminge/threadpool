#include "thread_pool.h"
#include "task.h"
#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

static void *worker(void *arg) {
	struct thread_pool *thread_pool = (struct thread_pool *)arg;
	struct task task;
	while(queue_dequeue(&thread_pool->queue, &task)) {
		task.function(task.arg);
	}
	return NULL;
}

bool thread_pool_init(struct thread_pool *thread_pool, size_t workers) {
	if (thread_pool == NULL || workers == 0)
		return false;

	*thread_pool = (struct thread_pool){0};
	if(!queue_init(&thread_pool->queue))
		return false;
	thread_pool->worker_count = workers;
	thread_pool->workers = calloc(thread_pool->worker_count, sizeof(pthread_t));

	if(thread_pool->workers == NULL) {
		return false;
	}

	for(size_t i = 0; i < thread_pool->worker_count; i++) {
		if(pthread_create(&thread_pool->workers[i], NULL, worker, thread_pool) != 0) {
			pthread_mutex_lock(&thread_pool->queue.mutex);
			thread_pool->queue.queue_closed = true;
			pthread_cond_broadcast(&thread_pool->queue.not_empty);
			pthread_mutex_unlock(&thread_pool->queue.mutex);
			for(size_t j = 0; j < i; j++)
				pthread_join(thread_pool->workers[j], NULL);
			free(thread_pool->workers);
			thread_pool->workers = NULL;
			thread_pool->worker_count = 0;
			return false;
		}
	}
	return true;
}

bool thread_pool_submit(struct thread_pool *thread_pool, void (*function)(void *), void *arg) {
	if(thread_pool == NULL || function == NULL) {
		return false;
	}

	struct task task = (struct task){function, arg};
	return queue_enqueue(&thread_pool->queue, task);
}

void thread_pool_shutdown(struct thread_pool *thread_pool) {
	pthread_mutex_lock(&thread_pool->queue.mutex);
	thread_pool->queue.queue_closed = true;
	pthread_cond_broadcast(&thread_pool->queue.not_empty);
	pthread_mutex_unlock(&thread_pool->queue.mutex);
	for(size_t i = 0; i < thread_pool->worker_count; i++) {
		pthread_join(thread_pool->workers[i], NULL);
	}
}

void thread_pool_destroy(struct thread_pool *thread_pool) {
	pthread_mutex_destroy(&thread_pool->queue.mutex);
	pthread_cond_destroy(&thread_pool->queue.not_empty);
	pthread_cond_destroy(&thread_pool->queue.not_full);
	free(thread_pool->workers);
	thread_pool->workers = NULL;
}
