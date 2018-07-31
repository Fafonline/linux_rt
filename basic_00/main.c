#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#include <termios.h>  //_getch*/
#include <unistd.h>   //_getch*/


struct t_thread
{
    pthread_t thread;
    int id;
    int sched_policy;
    int shced_scope;
    void (*cb)(int);
};


char getch(){

    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }


 void threadGetChar(int id)
 {
    char c='Z';
    printf("[thread %d] Get char...\n",id);
    c = getch();
    printf("[thread %d] ... Got char '%c' \n",id,c);

 }



static void *_Thread(void *arg)
{
    struct t_thread* thread_s = (struct t_thread*)arg;

    printf("Thread (%d) running!\n",thread_s->id);

    char toggle = 1;
    while (1)
    {
        sleep(1);
        printf ("[thread %d] %s\n",thread_s->id,toggle?"Tic...":"Toc...");
        toggle = !toggle;

        if(thread_s->cb != NULL)
        {
            thread_s->cb(thread_s->id);
        }
    }
    return NULL;
}



int CreateThread(struct t_thread* p_thread)
{

    int retVal;

    pthread_attr_t attr;
    struct sched_param schedParam;

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
    
    retVal = pthread_attr_setschedpolicy(&attr, p_thread->sched_policy);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_setschedpolicy error %d\n", retVal);
        exit(1);
    }
    int max_prio, min_prio, mid_prio; 
    max_prio = sched_get_priority_max(p_thread->sched_policy);   
    min_prio = sched_get_priority_min(p_thread->sched_policy);   
    mid_prio = (min_prio + max_prio)/2; 

    printf("[thread %d](Sched policy=%d, Sched Scope = %d, prio: min=%d, max=%d, mid=%d\n",
        p_thread->id,
        p_thread->sched_policy,
        p_thread->shced_scope,
        min_prio,
        max_prio,mid_prio);

    schedParam.sched_priority = mid_prio;

    retVal = pthread_attr_setschedparam(&attr, &schedParam);
    if (retVal)
    {
        fprintf(stderr, "pthread_attr_setschedparam error %d\n", retVal);
        exit(1);
    }

    retVal = pthread_create(&(p_thread->thread),
                            &attr,
                            _Thread,
                            p_thread);
    if (retVal)
    {
        fprintf(stderr, "pthread_create error %d\n", retVal);
        exit(1);
    } 
    return retVal;
}


struct t_thread  list_thread[]={
    {
        0,
        0,
        SCHED_FIFO,
        PTHREAD_SCOPE_PROCESS,
        NULL
    }
    ,{
        0,
        1,
        SCHED_FIFO,
        PTHREAD_SCOPE_PROCESS,
        NULL
    }
#if 1
    ,{
        0,
        2,
        SCHED_FIFO,
        PTHREAD_SCOPE_SYSTEM,
        threadGetChar
    }
#endif
};

int main(void)
{
    int retVal;


    int num_thread = 0;

    num_thread = sizeof(list_thread) / sizeof(struct t_thread);
    
    int i = 0;

    for( i = 0; i< num_thread;i++ )
    {
        retVal = CreateThread ( &(list_thread[i]) );
    }
 
 

    for( i = 0; i< num_thread;i++)
    {
      retVal = pthread_join(list_thread[i].thread, NULL);
    }

    if (retVal)
    {
        fprintf(stderr, "pthread_join error %d\n", retVal);
        exit(1);
    }

    printf("main run successfully\n");
    return 0;
}