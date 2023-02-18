#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore
{
	/* TODO Phase 3 */
	size_t numOfResources;
	queue_t blockQueue;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem;

	sem = (sem_t)malloc(sizeof(struct semaphore));
	if (sem == NULL)
	{
		return NULL;
	}
	sem->numOfResources = count;
	sem->blockQueue = queue_create();
	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
	{
		return -1;
	}
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
	{
		return -1;
	}
	preempt_disable();
	while (sem->numOfResources == 0)
	{
		//printf("semdown here\n");
		struct uthread_tcb *currentThread = uthread_current();
		queue_enqueue(sem->blockQueue, currentThread);
		uthread_block();
	}
	sem->numOfResources--;
	preempt_enable();
	//printf("semcount: %lu\n", sem->numOfResources);
	return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
	{
		return -1;
	}
	preempt_disable();
		void *tempData;
		int result = queue_dequeue(sem->blockQueue, &tempData);
		if (result == 0)
		{
			//printf("semup here\n");
			struct uthread_tcb *convertedData = (struct uthread_tcb *)tempData;
			//printf("semup after\n");
			uthread_unblock(convertedData);
		}
	sem->numOfResources++;
	preempt_enable();
	//printf("semcount: %lu\n", sem->numOfResources);
	return 0;
}
