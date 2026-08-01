#include <stdio.h>
#include <stdlib.h>
#include "thread_pool.h"

void print_message(void *arg) {
	printf("%s\n", (char *)arg);
}

int main(void) {
	struct thread_pool pool;

	if(!thread_pool_init(&pool, 2)) {
		fprintf(stderr, "Failed to initialize thread pool.\n");
		return EXIT_FAILURE;
	}

	thread_pool_submit(&pool, print_message, "Hello");
	thread_pool_submit(&pool, print_message, "World");

	thread_pool_shutdown(&pool);
	thread_pool_destroy(&pool);
	return 0;
}
