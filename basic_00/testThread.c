#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *_Thread(void *arg)
{
    (void)arg;
    printf("Thread running!\n");
    return NULL;
}

int main(void)
{
    int retVal;
    pthread_attr_t attr;
    struct sched_param schedParam;
    pthread_t thread;

    retVal = pthread_attr_init(&attr);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_init error %d\n", retVal);
        exit(1);
    }

    retVal = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_setinheritsched error %d\n", retVal);
        exit(1);
    }

    retVal = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_setschedpolicy error %d\n", retVal);
        exit(1);
    }

    schedParam.sched_priority = 1;
    retVal = pthread_attr_setschedparam(&attr, &schedParam);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_setschedparam error %d\n", retVal);
        exit(1);
    }

    retVal = pthread_create(&thread,
                            &attr,
                            _Thread,
                            NULL);
    if (retVal)
    {
        fprintf(stderr, "pthread_create error %d\n", retVal);
        exit(1);
    }

    retVal = pthread_join(thread, NULL);
    if (retVal)
    {
        fprintf(stderr, "pthread_join error %d\n", retVal);
        exit(1);
    }

    printf("main run successfully\n");
    return 0;
}