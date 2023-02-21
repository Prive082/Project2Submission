#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>

sem_t sem;

static void thread3(void *arg)
{
	(void)arg;
    sem_down(sem);
	printf("thread3\n");
}

static void thread2(void *arg)
{
	(void)arg;
    sem_up(sem);
	printf("thread2\n");
    uthread_yield();
    printf("End 2\n");
}

static void thread1(void *arg)
{
	(void)arg;

	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);
    
    sem_down(sem);

	printf("thread1\n");
}

int main(void)
{
	sem = sem_create(0);

	uthread_run(false, thread1, NULL);

	sem_destroy(sem);

	return 0;
}