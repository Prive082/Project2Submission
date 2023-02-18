#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

// implementing a doubly linked list as it has enqueue and dequeue O(1) time
struct queueNode
{
	/* TODO Phase 1 */
	void *data;
	struct queueNode *next;
};

struct queue
{
	int queueLen;
	struct queueNode *head;
	struct queueNode *tail;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q;

	q = (queue_t)malloc(sizeof(struct queue));
	if (q == NULL)
	{
		return NULL;
	}
	q->head = NULL;
	q->tail = NULL;
	q->queueLen = 0;
	return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL || queue->head != NULL)
	{
		return -1;
	}
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if (queue == NULL)
	{
		return -1;
	}

	struct queueNode *tempNode = (struct queueNode *)malloc(sizeof(struct queueNode));

	if (tempNode == NULL)
	{
		return -1;
	}

	tempNode->data = data;
	tempNode->next = NULL;

	if (queue->head == NULL || queue->tail == NULL)
	{
		queue->head = tempNode;
		queue->tail = tempNode;
	}
	else
	{
		queue->tail->next = tempNode;
		queue->tail = tempNode;
	}
	queue->queueLen = queue->queueLen + 1;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	struct queueNode *tempNode;
	/* TODO Phase 1 */
	if (queue == NULL)
	{
		return -1;
	}

	if (queue->head == NULL)
	{
		return -1;
	}
	*data = (queue->head->data);
	tempNode = queue->head;
	queue->head = queue->head->next;
	if (queue->head == NULL)
	{
		queue->tail = NULL;
	}
	free(tempNode);
	queue->queueLen = queue->queueLen - 1;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (data == NULL || queue == NULL)
		return -1;
	struct queueNode* current = queue->head;
	struct queueNode* prev;
	if (current != NULL && current->data == data)
	{
		queue->head = current->next;
		free(current);
		if (queue->head == NULL)
		        queue->tail = NULL;
	}
	else
	{
		while (current != NULL && current->data != data)
		{
			prev = current;
			current = current->next;
		}
		if (current == NULL)
			return -1;
		prev->next = current->next; 
		free(current);
	}
	queue->queueLen--; 
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if (queue == NULL || func == NULL)
	{
		return -1;
	}
	/* TODO Phase 1 */
	struct queueNode *currentItem = queue->head;
	struct queueNode *temp;
	while (currentItem != NULL)
	{
		temp = currentItem->next;
		func(queue, currentItem->data);
		currentItem = temp;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL)
	{
		return -1;
	}
	return queue->queueLen;
}
