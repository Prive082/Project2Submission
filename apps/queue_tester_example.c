#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)                 \
	do                                      \
	{                                       \
		printf("ASSERT: " #assert " ... "); \
		if (assert)                         \
		{                                   \
			printf("PASS\n");               \
		}                                   \
		else                                \
		{                                   \
			printf("FAIL\n");               \
			exit(1);                        \
		}                                   \
	} while (0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");
	fprintf(stderr, "%p\n", &data);
	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void **)&ptr);
	fprintf(stderr, "%p\n", ptr);
	TEST_ASSERT(ptr == &data);
	TEST_ASSERT(queue_length(q)==0 );
}

static void iterator_inc(queue_t q, void *data)
{
	fprintf(stderr, "I'm here\n");
	int *a = (int *)data;
	
	fprintf(stderr, "a: %d\n", *a);
	if (*a == 42)
		queue_delete(q, data);
	else
		*a += 1;
}

void test_iterator(void)
{
	queue_t q;
	int data[] = {42, 2, 3, 4, 5, 42, 6, 7, 8, 9};
	size_t i;

	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);

	/* Increment every item of the queue, delete item '42' */
	printf("length: %d\n", queue_length(q));
	queue_iterate(q, iterator_inc);

	TEST_ASSERT(data[1] == 3);
	printf("Length: %d\n", queue_length(q));
	TEST_ASSERT(queue_length(q) == 9);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_iterator();

	return 0;
}
