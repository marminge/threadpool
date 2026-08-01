#ifndef TASK_H_
#define TASK_H_

struct task {
	void (*function)(void *);
	void *arg;
};

#endif
