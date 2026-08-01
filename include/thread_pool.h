#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "queue.h"
#include <pthread.h>

#define MAX_QUEUE_SIZE 64

/**
 * @brief Represents a thread pool.
 *
 * A thread pool manages a fixed number of worker threads that execute
 * submitted tasks concurrently.
 */
struct thread_pool {
	pthread_t *workers;
	struct queue queue;
	size_t worker_count;
};

/**
 * @brief Initializes a thread pool.
 *
 * Prepares a thread pool for accepting and executing submitted tasks.
 *
 * @param thread_pool Pointer to the thread pool to initialize.
 * @param workers Number of worker threads in the pool.
 *
 * @return true if initialization succeeded, false otherwise.
 */
bool thread_pool_init(struct thread_pool *thread_pool, size_t workers);

/**
 * @brief Submits a task to the thread pool.
 *
 * The task will be executed by the next available worker thread.
 *
 * @param thread_pool Pointer to the thread pool.
 * @param function Function to execute.
 * @param arg Argument passed to the function when it is executed.
 *
 * @return true if the task was successfully submitted,
 *         false if the task could not be submitted.
 */
bool thread_pool_submit(struct thread_pool *thread_pool, void (*function)(void *), void *arg);

/**
 * @brief Stops the thread pool.
 *
 * Prevents new tasks from being accepted, wakes any waiting worker threads,
 * and waits for all workers to finish executing their current tasks before
 * returning.
 *
 * @param thread_pool Pointer to the thread pool.
 */
void thread_pool_shutdown(struct thread_pool *thread_pool);

/**
 * @brief Releases resources owned by the thread pool.
 *
 * This function destroys the synchronization primitives and frees any
 * dynamically allocated memory used by the thread pool.
 *
 * The thread pool must have been shut down before calling this function.
 *
 * @param thread_pool Pointer to the thread pool.
 */
void thread_pool_destroy(struct thread_pool *thread_pool);

#endif
