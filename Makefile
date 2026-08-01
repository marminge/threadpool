CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g -Iinclude
LDFLAGS = -pthread

TARGET = threadpool

SRC = \
	src/main.c \
	src/queue.c \
	src/thread_pool.c 

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
