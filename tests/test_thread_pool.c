#include "thread_pool.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void test_init_success(void);

void test_submit_single_task(void);

void test_submit_many_tasks(void);

void test_submit_after_shutdown(void);

void test_shutdown_waits_for_tasks(void);

void test_invalid_arguments(void);

static void print_test(const char *name) {
    printf("Running %-30s", name);
    fflush(stdout);
}

static void test_passed(void) {
    puts("OK");
}

int main(void) {
	print_test("test_init_success");
	test_init_success();
	test_passed();

	print_test("test_submit_single_task");
	test_submit_single_task();
	test_passed();

	print_test("test_submit_many_tasks");
	test_submit_many_tasks();
	test_passed();

	print_test("test_submit_after_shutdown");
	test_submit_after_shutdown();
	test_passed();

	print_test("test_shutdown_waits_for_tasks");
	test_shutdown_waits_for_tasks();
	test_passed();

	print_test("test_invalid_arguments");
	test_invalid_arguments();
	test_passed();

	printf("All the tests passed!\n");
	return 0;
}

void test_init_success(void) {
	struct thread_pool pool;

	assert(thread_pool_init(&pool, 2));

	thread_pool_shutdown(&pool);
	thread_pool_destroy(&pool);
}

struct counter {
	int value;
	pthread_mutex_t mutex;
};

void increment(void *arg) {
	struct counter *counter = arg;
	pthread_mutex_lock(&counter->mutex);
	counter->value++;
	pthread_mutex_unlock(&counter->mutex);
}

void slow_increment(void *arg) {
    struct counter *counter = arg;

    sleep(1);

    pthread_mutex_lock(&counter->mutex);
    counter->value++;
    pthread_mutex_unlock(&counter->mutex);
}

void test_submit_single_task(void) {
	struct thread_pool pool;
	struct counter counter = {
		.value = 0
	};

	assert(pthread_mutex_init(&counter.mutex, NULL) == 0);
	assert(thread_pool_init(&pool, 2));
	assert(thread_pool_submit(&pool, increment, &counter));
	thread_pool_shutdown(&pool);
	thread_pool_destroy(&pool);
	pthread_mutex_destroy(&counter.mutex);
	assert(counter.value == 1);
}

void test_submit_many_tasks(void) {
	struct thread_pool pool;
	struct counter counter = {
		.value = 0
	};

	assert(pthread_mutex_init(&counter.mutex, NULL) == 0);
	assert(thread_pool_init(&pool, 4));
	for(int i = 0; i < 10000; i++) {
		assert(thread_pool_submit(&pool, increment, &counter));
	}
	thread_pool_shutdown(&pool);
	thread_pool_destroy(&pool);
	pthread_mutex_destroy(&counter.mutex);
	assert(counter.value == 10000);
}

void test_submit_after_shutdown(void) {
    struct thread_pool pool;

    assert(thread_pool_init(&pool, 2));

    thread_pool_shutdown(&pool);

    assert(!thread_pool_submit(&pool, increment, NULL));

    thread_pool_destroy(&pool);
}

void test_shutdown_waits_for_tasks(void) {
	struct thread_pool pool;
	struct counter counter = {
		.value = 0
	};

	assert(pthread_mutex_init(&counter.mutex, NULL) == 0);
	assert(thread_pool_init(&pool, 2));
	assert(thread_pool_submit(&pool, slow_increment, &counter));
	thread_pool_shutdown(&pool);
	thread_pool_destroy(&pool);
	pthread_mutex_destroy(&counter.mutex);
	assert(counter.value == 1);
}


void test_invalid_arguments(void) {
	struct thread_pool pool;
	assert(!thread_pool_init(&pool, 0));
	assert(!thread_pool_init(NULL, 2));
	assert(!thread_pool_submit(NULL, increment, NULL));
	assert(!thread_pool_submit(&pool, NULL, NULL));
}
