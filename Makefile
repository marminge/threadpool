CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g -Iinclude
LDFLAGS = -pthread

LIB_SRC = \
	src/queue.c \
	src/thread_pool.c

LIB_OBJ = $(LIB_SRC:.c=.o)

EXAMPLE_SRC = examples/example.c
EXAMPLE_TARGET = examples/example

TEST_SRC = tests/test_thread_pool.c
TEST_TARGET = tests/test_thread_pool

all: $(EXAMPLE_TARGET)

$(EXAMPLE_TARGET): $(EXAMPLE_SRC) $(LIB_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRC) $(LIB_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

example: $(EXAMPLE_TARGET)
	./$(EXAMPLE_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(LIB_OBJ)
	rm -f $(EXAMPLE_TARGET)
	rm -f $(TEST_TARGET)

.PHONY: all example test clean
