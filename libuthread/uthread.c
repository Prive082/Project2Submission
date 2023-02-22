#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

#define RUNNING 3
#define READY 4
#define BLOCKED 5
#define FINISHED 6

struct uthread_tcb
{
	/* TODO Phase 2 */
	uthread_ctx_t context;
	int state;
};

queue_t waitingQueue;
queue_t deleteQueue;
struct uthread_tcb *currentThread;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return currentThread;
}

void context_switch(struct uthread_tcb *nextTcb)
{
	struct uthread_tcb *tempCurrentThread = currentThread;
	currentThread = nextTcb;
	currentThread->state = RUNNING;
	uthread_ctx_t *prevCtx = &tempCurrentThread->context;
	uthread_ctx_t *nextCtx = &nextTcb->context;
	uthread_ctx_switch(prevCtx, nextCtx);
}

void freeTcb(struct uthread_tcb *tcb)
{
	free(tcb);
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	void *data;
	preempt_disable();
	queue_dequeue(waitingQueue, &data);
	struct uthread_tcb *nextThread = (struct uthread_tcb *)data;
	currentThread->state = READY;
	queue_enqueue(waitingQueue, currentThread);
	context_switch(nextThread);
	preempt_enable();
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	void *data;
	preempt_disable();
	queue_dequeue(waitingQueue, &data);
	struct uthread_tcb *nextThread = (struct uthread_tcb *)data;
	currentThread->state = FINISHED;
	queue_enqueue(deleteQueue, currentThread);
	context_switch(nextThread);
	preempt_enable();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	preempt_disable();
	void *stackTop = uthread_ctx_alloc_stack();
	struct uthread_tcb *tcb = malloc(sizeof(struct uthread_tcb));
	tcb->state = READY;
	queue_enqueue(waitingQueue, tcb);
	uthread_ctx_init(&tcb->context, stackTop, func, arg);
	preempt_enable();
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	// ininialize thread queue and main thread
	waitingQueue = queue_create();
	deleteQueue = queue_create();
	struct uthread_tcb *idleTcb = malloc(sizeof(struct uthread_tcb));
	currentThread = idleTcb;
	currentThread->state = RUNNING;

	// initialize first thread
	uthread_create(func, arg);
	preempt_start(preempt);
	
	while (1)
	{
		// clean the delete queue
		while (queue_length(deleteQueue) != 0)
		{
			void *data;
			queue_dequeue(deleteQueue, &data);

			struct uthread_tcb *deleteThread = (struct uthread_tcb *)data;
			freeTcb(deleteThread);
		}
		// check if only idle thread is left
		if (queue_length(waitingQueue) == 0)
		{
			queue_destroy(waitingQueue);
			queue_destroy(deleteQueue);
			preempt_stop();
			return 0;
		}
		uthread_yield();
	}
}

void uthread_block(void)
{
	/* TODO Phase 3 */
	// hold thread in blocked waiting queue
	void *data;
	preempt_disable();
	queue_dequeue(waitingQueue, &data);
	struct uthread_tcb *nextThread = (struct uthread_tcb *)data;
	currentThread->state = BLOCKED;
	context_switch(nextThread);
	preempt_enable();
	return;
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	queue_enqueue(waitingQueue, uthread);
	uthread->state = READY;
	// take from waititng queue and reinsert into the waiting queue
	return;
}
