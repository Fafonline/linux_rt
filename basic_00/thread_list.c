#include <pthread.h>
#include "types.h"
#include <stdio.h>


 void threadGetChar(int id)
 {
    char c='Z';
    printf("[thread %d] Get char...\n",id);
    c = getch();
    printf("[thread %d] ... Got char '%c' \n",id,c);

 }
 void threadSleeps( int id )
 {
    printf ("[thread %d] Sleeps ...\n",id);
    sleep(5);   
 }

struct t_thread  list_thread[3]={
    {
        0,
        0,
        SCHED_OTHER,
        PTHREAD_SCOPE_PROCESS,
        0.5,
        //NULL
        threadGetChar
    }
    ,{
        0,
        1,
        SCHED_RR,
        PTHREAD_SCOPE_PROCESS,
        0.5,
        threadSleeps
    }
#if 1
    ,{
        0,
        2,
        SCHED_OTHER,
        PTHREAD_SCOPE_PROCESS,
        1,
        threadGetChar
    }
#endif
};