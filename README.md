# Thread Pool

A reusable thread pool library written in C using POSIX threads (`pthread`).

The library manages a fixed number of worker threads that execute submitted tasks concurrently. It hides all thread management, synchronization, and task scheduling behind a simple API.

## Features

- Fixed-size worker thread pool
- Thread-safe task submission
- Blocking task queue
- Graceful shutdown
- POSIX threads (`pthread`)
- Written in standard C11

## Project Structure

```
include/
    thread_pool.h


src/
    thread_pool.c
    queue.c
    main.c
```

## Building

```bash
make
```

Run the example program:

```bash
make run
```

Clean generated files:

```bash
make clean
```

## Example

```c
#include <stdio.h>

#include "thread_pool.h"

void print_message(void *arg)
{
    printf("%s\n", (char *)arg);
}

int main(void)
{
    struct thread_pool pool;

    if (!thread_pool_init(&pool, 4))
        return 1;

    thread_pool_submit(&pool, print_message, "Hello");
    thread_pool_submit(&pool, print_message, "World!");

    thread_pool_shutdown(&pool);
    thread_pool_destroy(&pool);

    return 0;
}
```

## API

```c
bool thread_pool_init(struct thread_pool *thread_pool,
                      size_t workers);

bool thread_pool_submit(struct thread_pool *thread_pool,
                        void (*function)(void *),
                        void *arg);

void thread_pool_shutdown(struct thread_pool *thread_pool);

void thread_pool_destroy(struct thread_pool *thread_pool);
```

## Requirements

- GCC or Clang
- POSIX threads (`pthread`)
- C11 compatible compiler

## License

MIT License.
