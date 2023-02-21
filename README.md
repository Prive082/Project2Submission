
# User Level Thread Library

## Summary

The goal of this project is to understand how thread scheduling and 
syncronization works by implementing the **uthread** library, which allows
users to create multithreaded program by defining threads as individual
functions and passing the function pointers to the uthread library functions.
Its primary functionalities include supporting concurrent thread execution and
avoiding race condition in concurrent programs.

## Implementation

The implementation of the uthread library is listed as below:

1. Use queue datastructure to implement First In First Out (FIFO) scheduling
2. Offer user level functions to create and execute threads concurrently
3. Offer user level functions to yield threads to other waiting threads
4. Include semaphore functions to block thread execution at critical sections
5. Allow preemptive thread yielding at a frequency of 100 Hz

### Queue Implementation

The queue datastructure offers `queue_create()` and 
`queue_destroy(queue_t queue)` to encapsulate queue's memory allocation and
deallocation. The queue is implemented through a linked list consisting of 
individual nodes defined by `struct queueNode`, and a main datastructure
`struct queue` which contains the linked list's length (accessible through the
function `queue_length(queue_t queue)`), head node, and tail node. The pointer
to a queue struct is also defined by the type `queue_t`. 

The FIFO property of the queue is realized by the function
`queue_enqueue(queue_t queue, void *data)`, which inserts the data after the
tail element of the linked list. The
`queue_dequeue(queue_t queue, void **data)` removes the head node of the linked
list and puts its data in the second argument of the function. Through these
two functions, we can make sure that the newly enqueued data will only be
dequeued after all the data that are inserted beforehand has been dequeued,
thus complies with the FIFO property of a queue.

The queue also have functionalities such as 
`queue_delete(queue_t queue, void *data)`, which traverses through the linked
list of the queue and removes the first node that matches with `void *data`.
`queue_iterate(queue_t queue, queue_func_t func)` traverses through the linked
list and apply the user-defined function to each queue node.

### Uthread Library

The uthread library allows users to create concurrent programs through
functions that manages multiple threads.

#### context.c

The implementation of uthread library depends on functions defined in 
context.c, which encapsulate context management functions in the GNU
library into the following key functions:

`uthread_ctx_init(*uctx, *top_of_stack, func, *arg)`
This function initializes the user-level context struct by passing in the
allocated stack pointer from the argument `top_of_stack`. It then set up the
context and execute the function pointed to by `func` with its respective
argument `arg`.

`uthread_ctx_switch(*prev, *next)`
This function switches the thread execution context from the one defined by
`prev` to `next`. The effect of this function will be that the previous thread
will pause its execution while the next thread resumes execution.

#### Data structure

The uthread library uses `struct uthread_tcb` to store needed information about
a thread that's created. The library uses the queue `uthreadQueue` to store all
the threads that are waiting to be executed. The FIFO scheduling order is
implemented by enqueuing the `uthread_tcb` into `uthreadQueue` when a thread is
created, and dequeuing the `uthread_tcb` from `uthreadQueue` when executing the
thread. `uthreadQueue` is made as a global variable because it needs to be
accessed by all other functions in the library.

Similarly, there's also the global variable `currentThread` that stores the
thread that is currently executing. `deleteQueue` is another queue in the
uthread library that stores the threads that have finished execution, which can
be used to free the associated memory of these threads.

#### Functions

`int uthread_create(uthread_func_t func, void *arg)`
This function accepts the pointer to the user defined function `func` along
with the function's argument `arg`, which together consists the code of a
thread. This function will create a `uthread_tcb` for the new thread, schedule
the thread for execution by calling the function `uthread_ctx_init` and store
the thread in `uthreadQueue`. 

`void uthread_yield(void)`
This function yields the currently executing thread by calling the function
`uthread_ctx_switch` to perform context switch between the current thread and
the new thread dequeued from the `uthreadQueue`. While performing the context
switch, the new thread will replace the previous thread as the `currentThread`,
and the previous thread will be enqueued back to the `uthreadQueue`, waiting to
be scheduled in the future.

`void uthread_exit(void)`
This function performs context switch in the same way as `uthread_yield`,
except that the previous thread will be enqueued into `deleteQueue`, since
calling exit means the thread should not be scheduled in the future. 

`int uthread_run(bool preempt, uthread_func_t func, void *arg)`
The execution of this function marks the lifecycle of the multithreaded
program, therefore all other functions in the uthread library should be called
within the threads that will be scheduled during the lifecycle. This function
allocates memory for the `uthreadQueue` and `deleteQueue`, and it creates an
idle thread that marks the execution context of `uthread_run` function itself
and store the thread in the variable `currentThread`. The function then creates
the context for the first thread defined by the arguments `func` and `arg`,
and then it enters a infinite loop in which it will yield to any threads
remaining in `uthreadQueue`. The loop is also responsible for cleaning the
threads in the `deleteQueue`. The function will exit after the threads have
finished execution.
