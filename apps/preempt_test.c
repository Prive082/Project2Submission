#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <uthread.h>

#define LOOP_SIZE 999999999

void thread3(void *arg)
{
	(void)arg;

    for (int i = 0; i < LOOP_SIZE; i++) 
    {
        if (i % 10000000 == 0) 
        {
            printf("thread 3 has counted %d times\n", i);
        }
    }
}

void thread2(void *arg)
{
	(void)arg;

    for (int i = 0; i < LOOP_SIZE; i++) 
    {
        if (i % 10000000 == 0) 
        {
            printf("thread 2 has counted %d times\n", i);
        }
    }
}

void thread1(void *arg)
{
	(void)arg;

    uthread_create(thread2, NULL);
    uthread_create(thread3, NULL);
    
    for (int i = 0; i < LOOP_SIZE; i++) 
    {
        if (i % 10000000 == 0) 
        {
            printf("thread 1 has counted %d times\n", i);
        }
    }
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
