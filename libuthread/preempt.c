#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 * Preemption period in usec is USEC_PER_SEC / HZ
 */
#define USEC_PER_SEC 1000000
#define HZ 100

struct sigaction sa_old;
struct itimerval timer_old;

void timer_handler(int signum)
{
	if (signum == SIGVTALRM)
	{
		uthread_yield();
	}
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigset_t ss;
	sigemptyset(&ss);
	sigaddset(&ss, SIGVTALRM);
	sigprocmask(SIG_BLOCK, &ss, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	sigset_t ss;
	sigemptyset(&ss);
	sigaddset(&ss, SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, &ss, NULL);
}

// since no prior experience with timer
// reference: https://stackoverflow.com/questions/48064940/understanding-struct-itimerval-field-tv-usec
void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
	if (preempt == false)
	{
		return;
	}
	struct sigaction sa;
	struct itimerval timer;

	// set up alarm handler
	memset(&sa, 0, sizeof(sa));
	memset(&timer, 0, sizeof(timer));
	sa.sa_handler = &timer_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, &sa_old);
	// Counts down from 100ms
	timer.it_value.tv_usec = USEC_PER_SEC / HZ;
	// and repeats every time after
	timer.it_interval.tv_usec = USEC_PER_SEC / HZ;
	// start timer and count down when executing
	setitimer(ITIMER_VIRTUAL, &timer, &timer_old);
	
}

void preempt_stop(void)
{

	/* TODO Phase 4 */
	sigaction(SIGVTALRM, &sa_old, NULL);

	setitimer(ITIMER_VIRTUAL, &timer_old, NULL);
}
