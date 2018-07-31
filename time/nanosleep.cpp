#include <stdio.h>
#include<time.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
// File: nanosleep.cpp.template
// Build: gcc -o nanosleep nanosleep.cpp.template -lstdc++ -std=c++11

#define PROG_PROMPT "Run program: <nanosleep>"
#define USEC_PER_SEC		1000000
#define NSEC_PER_SEC		1000000000
#define TIMER_ABSTIME 1


static inline void tsnorm(struct timespec *ts)
{
	while (ts->tv_nsec >= NSEC_PER_SEC) {
		ts->tv_nsec -= NSEC_PER_SEC;
		ts->tv_sec++;
	}
}

void printTime(timespec& p_time)
{
	printf("Time: %lu s,%lu ns\n", p_time.tv_sec,p_time.tv_nsec);
}

int main(int arg, char* argv[])
{
	printf(PROG_PROMPT"\n");

	struct timespec now, next, interval;
    
    int period = 1000000;

	interval.tv_sec = period / USEC_PER_SEC;
	interval.tv_nsec = (period % USEC_PER_SEC) * 1000;

	int ret=1;
	while(1)
	{
    clock_gettime(CLOCK_MONOTONIC, &now);

	next = now;
    
    printf("Now:");
    printTime(now);
    

	next.tv_sec += interval.tv_sec;
	next.tv_nsec += interval.tv_nsec;
	tsnorm(&next);

    printf("Next:");
	printTime(next);

#if 1
		if ((ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL))) {
			if (ret != EINTR)
				warn("clock_nanosleep failed. errno: %d\n", errno);
			goto out;
			}
			printf("tic...\n");
#else
		nanosleep(&interval,NULL);
		printf("tic...\n");
#endif
	}
out:
	return 0;
}
