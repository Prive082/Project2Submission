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
 */
#define HZ 100

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
	struct sigaction sa;
	struct itimerval timer;

	// set up alarm handler
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &timer_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
	if (preempt == true)
	{
		// We multiply because we need 100 HZ in MICROseconds
		// Counts down from 100ms
		timer.it_value.tv_usec = HZ * 1000;
		// and repeats every time after
		timer.it_interval.tv_usec = HZ * 1000;
		// start timer and count down when executing
		setitimer(ITIMER_VIRTUAL, &timer, NULL);

		// busy work
		timer_handler(SIGVTALRM);
	}
	else if (preempt == false)
	{
		return;
	}

	return;
}

void preempt_stop(void)
{
	struct sigaction sigReset;
	struct itimerval timerReset;
	/* TODO Phase 4 */
	sigReset.sa_handler = SIG_DFL;
	sigaction(SIGVTALRM, &sigReset, NULL);

	timerReset.it_value.tv_usec = 0;
	// and repeats every time after
	timerReset.it_interval.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &timerReset, NULL);
}
